/**
 *	\author    John Szwast
 *	\year      2014-2016
 *	\copyright MIT
 */


#pragma once


#include <mutex>
#include <type_traits>
#include <utility>

/**
 * \brief MemoizedMember is a C++ class that caches the result of a computation.
 *
 * \details
 *  If your class `A` has an attribute `b` with a value you want to memoize:
 * 
 * 1. Write a `private`, `const` computation method, `type compute_b() const`.
 * 
 * 2. Add a `MemoizedMember` member using the computation method.
 * 
 * 3. Write a `public` getter for the `MemoizedMember`, `type get_b() const`.
 * 
 * Here is an example memoizing an `int` type attribute.
 * 
 *     class A
 *     {
 *       public:
 *         int get_b() const {return b;}
 *       private:
 *         int compute_b() const;
 *         MemoizedMember<int, A, &A::compute_b> b{*this};
 *     };
 */
template<typename Key, class Class, Key (Class::*evaluate) () const, typename Mutex = std::mutex>
class MemoizedMember
{
  public:
    /**
     * \brief  "Default" constructor
     * \param[in]  instance  The Class instance in which *this is a member
     * \remarks  We require a reference to the Class of which we are a member.
     */
    MemoizedMember (Class const& instance)
    noexcept (std::is_nothrow_default_constructible<Key>::value)
      : m_instance (instance)
    {
    }

    /**
    * \brief  "Copy" constructor
    * \param[in]  instance  The Class instance in which *this is a member
    * \param[in]  r         The source of the copy
    * \remarks  We require a reference to the Class of which we are a member.
    *   We do not want to copy the reference to the class of which `r` is a member.
    */
    MemoizedMember (Class const& instance, const MemoizedMember& r)
    noexcept (std::is_nothrow_copy_constructible<Key>::value)
      : m_instance (instance)
      , m_value (r.m_value)
      , m_valid (r.m_valid)
    {
    }

    /**
    * \brief  "Move" constructor
    * \param[in]  instance  The Class instance in which *this is a member
    * \param[in]  r         The source of the copy
    * \remarks  We require a reference to the Class of which we are a member.
    *   We do not want to copy the reference to the class of which `r` is a member.
    */
    MemoizedMember (Class const& instance, MemoizedMember && r)
    noexcept (std::is_nothrow_move_constructible<Key>::value)
      : m_instance (instance)
      , m_value (std::move (r.m_value))
      , m_valid (r.m_valid)
    {
    }

    MemoizedMember(const MemoizedMember&) = delete;
    MemoizedMember(MemoizedMember&&) = delete;
    ~MemoizedMember() = default;

    /**
     * A memoized member should be affiliated with an object.  If that object is being
     * assigned we want to copy the memoization state, but m_instance shouldn't change.
     *
     * Basic exception guarantee: if copying the memoized value throws, then the memoization
     * will be cleared and recalculated on the next request.
     */
    MemoizedMember& operator= (const MemoizedMember& r)
    {
      std::lock_guard<Mutex> guard(m_lock);

      m_valid = false;
      m_value = r.m_value;
      m_valid = r.m_valid;
      return *this;
    }

    /**
     * A memoized member should be affiliated with an object.  If that object is being
     * (move) assigned we want to move the memoization state, but m_instance shouldn't change.
     *
     * Basic exception guarantee: if moving the memoized value throws, then the memoization
     * will be cleared and recalculated on the next request.
     */
    MemoizedMember& operator= (const MemoizedMember && r)
    {
      std::lock_guard<Mutex> guard(m_lock);

      m_valid = false;
      m_value = std::move (r.m_value);
      m_valid = r.m_valid;
      return *this;
    }

    /**
     * \brief Key conversion operator.
     *
     * \details
     *  This is the meat of this class.  This is how the MemoizedMember behaves like a Key object.
     * If the value has not yet been calculated, then calculate it.
     *
     * \returns	The memoized value.
     *
     * \remarks
     * Strong exception guarantee:  If the calculation of the value to be memoized fails, then
     * it will be reattempted next time.
     */
    operator Key() const
    {
      std::lock_guard<Mutex> guard(m_lock);

      if (!m_valid)
      {
        m_value = (m_instance.*evaluate) ();
        m_valid = true;
      }

      return m_value;
    }

    /**
     * If the owning object is mutated in a way that should change the value of the memoized
     * member, then reset() it.  After reset(), a re-computation will occur the next time the value
     * of the member is requested.
     */
    void reset() noexcept { m_valid = false; }

  private:
    Class const& m_instance;
    mutable Key m_value {};
    mutable bool m_valid {false};
    mutable Mutex m_lock;
};
