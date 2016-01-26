memoized-member
===============

MemoizedMember is a C++ class that caches the result of a computation.
Use it when you have a class with an attribute or property
that is (relatively) expensive to compute and not always queried
or queried a lot.

# Usage

If your class `A` has an attribute `b` with a value you want to memoize:

1. Write a `private`, `const` computation method, `type compute_b() const`.

2. Add a `MemoizedMember` member using the computation method.

3. Write a `public` getter for the `MemoizedMember`, `type get_b() const`.

Here is an example memoizing an `int` type attribute.

    class A
    {
      public:
        int get_b() const {return b;}
      private:
        int compute_b() const;
        MemoizedMember<int, A, &A::compute_b> b{*this};
    };

