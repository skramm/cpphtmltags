
#include <string>
#include <iostream>

struct A
{
	int val=42;
};

std::string&
operator + ( std::string& s, const A& a )
{
	s += std::to_string(a.val);
	return s;
}
int main()
{

	A a;
	std::string txt("aaa ");
	std::string t2 = txt + a;
	std::cout << t2 << "\n";

}
