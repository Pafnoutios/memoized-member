/**
 * \author John Szwast
 */


#include <exception>

#include "gtest/gtest.h"

#include "MemoizedMember.hpp"


/**
 * This struct is used for testing the exception safety of MemoizedMember.  It has flags that
 * will trigger the throwing of an exception on the next copy, move, or eval.  The flags are
 * cleared when they trigger, so they will only cause one exception each.
 */
struct ValueMightThrow
{
  ValueMightThrow() = default;
  ValueMightThrow(int x) : value(x) {}
  ValueMightThrow(ValueMightThrow const&) = default;
  ValueMightThrow(ValueMightThrow&&) = default;

  ValueMightThrow& operator=(ValueMightThrow const& r)
  {
    might_throw(throw_next_copy);
    value = r.value;
    return *this;
  }

  ValueMightThrow& operator=(ValueMightThrow&& r)
  {
    might_throw(throw_next_move);
    value = r.value;
    return *this;
  }

  friend bool operator==(ValueMightThrow const& l, ValueMightThrow const& r)
  {
    return l.value == r.value;
  }

  static ValueMightThrow eval()
  {
    eval_called = true;
    might_throw(throw_next_eval);
    return ValueMightThrow(next_eval_value);
  }

  static void might_throw(bool& flag)
  {
    if (flag)
    {
      flag = false;
      throw std::exception();
    }
  }

  static void clear()
  {
    throw_next_copy = false;
    throw_next_move = false;
    throw_next_eval = false;
    eval_called = false;
    next_eval_value = 0;
  }

  int value;

  static bool throw_next_copy;
  static bool throw_next_move;
  static bool throw_next_eval;
  static bool eval_called;
  static int next_eval_value;
};

bool ValueMightThrow::throw_next_copy = false;
bool ValueMightThrow::throw_next_move = false;
bool ValueMightThrow::throw_next_eval = false;
bool ValueMightThrow::eval_called = false;
int ValueMightThrow::next_eval_value = 0;


struct MemoHolder
{
  ValueMightThrow eval() const
  {
    return ValueMightThrow::eval();
  }

  MemoizedMember<ValueMightThrow, MemoHolder, &MemoHolder::eval> value{*this};
};


TEST(GeneralMemoization, EvalNotCalledUntilEvaluated)
{
  ValueMightThrow::clear();

  MemoHolder x;
  ValueMightThrow::next_eval_value = 4;
  EXPECT_FALSE(ValueMightThrow::eval_called);
  EXPECT_EQ(ValueMightThrow(4), x.value);
  EXPECT_TRUE(ValueMightThrow::eval_called);
}


TEST(GeneralMemoization, EvalNotCalledAgain)
{
  ValueMightThrow::clear();

  MemoHolder x;
  (void)static_cast<ValueMightThrow>(x.value);
  EXPECT_TRUE(ValueMightThrow::eval_called);
  ValueMightThrow::eval_called = false;
  (void)static_cast<ValueMightThrow>(x.value);
  EXPECT_FALSE(ValueMightThrow::eval_called);
}


TEST(ExceptionSafety, ThrowingOnCopy)
{
  MemoHolder x;
  MemoHolder y;

  // Seed starter values into x and y.
  ValueMightThrow::next_eval_value = 4;
  ASSERT_EQ(4, x.value);
  ValueMightThrow::next_eval_value = 5;
  ASSERT_EQ(5, y.value);

  // A good copy
  x = y;
  EXPECT_EQ(y.value, x.value);

  //y.value.value = 8;
  ADD_FAILURE() << "This test is not complete.";
}
