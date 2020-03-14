#include <MemoizedMember.hpp>

#include <iostream>


struct Test
{
	int calc() const { return 42; }
	MemoizedMember<int, Test, &Test::calc> test{*this};
};


int main() {
    Test x;
	std::cout << static_cast<int>(x.test);
}
