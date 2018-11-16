#include "cpphtmltags.hpp"
#include <iostream>

using namespace std;

int main()
{
	HTAG t1( HT_BR );
	cout << t1;

	HTAG t2( HT_P );
	cout << t2;

	t2.AddAttrib( AT_CLASS, "aaa" );
	cout << t2;

	t2.AddAttrib( AT_ID, "tag_id" );
	cout << t2;

	t2.AddAttrib( AT_CLASS, "bbb" );
	t2.setContent( "Hello World" );
	cout << t2;

}
