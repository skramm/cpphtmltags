// test2.cpp

#include "cpphtmltags.hpp"

using namespace std;
using namespace httags;

int main()
{
	HTAG t1( HT_BR );
	cout << t1;

	HTAG t2( HT_P );
	cout << t2;

	t2.addAttrib( AT_CLASS, "aaa" );
	t2.setContent( "this is a paragraph" );
	cout << t2;

	t2.setContent( 42 );
	cout << t2;

	t2.addAttrib( AT_ID, "tag_id" );
	cout << t2;

	t2.addAttrib( AT_CLASS, "bbb" );
	t2.setContent( "Hello World" );
	cout << t2 << '\n';
	HTAG::setGlobalAttrib( HT_P, AT_STYLE, "globalstyle" );

	HTAG t3( HT_P );
	cout << t2 << '\n';
	cout << t3 << '\n';

	{
		HTAG f1( cout, HT_A );
		f1.openTag();
		cout << "hi there";
		f1.closeTag( true );
	}

	{
		HTAG f1( cout, HT_A, AT_CLASS, "abc" );
		f1.openTag();
		cout << "hi there";
	}


}
