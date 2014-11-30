/**
 * \author John Szwast
 */


#pragma once


#include <utility>


namespace szwast
{
	template<typename Key, class Class>
	class MemoizedMember
	{
	public:
		MemoizedMember(Class& instance, Key (Class::*evaluate)() const)
		: m_instance(instance)
		, m_evaluate(evaluate)
		{
		}

		/**
		 * A memoized member should be affiliated with an object.  If that object is being
		 * assigned we want to copy the memoization state, but m_instance shouldn't change.
		 *
		 * Basic exception guarantee: if copying the memoized value throws, then the memoization
		 * will be cleared and recalculated on the next request.
		 */
		MemoizedMember& operator=(const MemoizedMember& r) noexcept(noexcept(m_value = r.m_value))
		{
			try
			{
				m_valid = r.m_valid;
				m_value = r.m_value;
			}
			catch ()
			{
				m_valid = false;
				throw;
			}
			return *this;
		}

		MemoizedMember& operator=(const MemoizedMember&& r)
			noexcept(noexcept(m_value = std::move(r.m_value)))
		{
			try
			{
				m_valid = r.m_valid;
				m_value = std::move(r.m_value);
			}
			catch ()
			{
				m_valid = false;
				throw;
			}
			return *this;
		}

		/**
		 * Key conversion operator.  This is the meat of this class.  This is how the MemoizedMember
		 * behaves like a Key object.  If the value has not yet been calculated, then calculate it.
		 *
		 * @returns	The memoized value.
		 *
		 * Strong exception guarantee:  If the calculation of the value to be memoized fails, then
		 * it will be reattempted next time.
		 */
		operator Key() const noexcept(noexcept((m_instance.*m_evaluate)()))
		{
			if (!m_valid)
			{
				m_value = (m_instance.*m_evaluate)();
				m_valid = true;
			}
			return m_value;
		}

	private:
		mutable bool m_valid{false};
		mutable Key m_value{};
		Class const& m_instance;
		const Key (Class::*m_evaluate)() const;
	};

}	// namespace szwast
