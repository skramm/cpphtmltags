
// build with
// g++ -std=c++11 demo/test_1.cpp -lboost_stacktrace_backtrace -ldl -lbacktrace
// see https://www.boost.org/doc/libs/1_70_0/doc/html/stacktrace/configuration_and_build.html

#include <iostream>
#include <boost/stacktrace.hpp>

void bar()
{
	std::cout << "bar()\n";
//	std::cout << boost::stacktrace::stacktrace();
}

void foo()
{
	std::cout << "foo()\n";
//	std::cout << boost::stacktrace::stacktrace();
	bar();
}
int main()
{
	std::cout << "main()\n";
	foo();
	std::cout << "back in main()\n";

}
