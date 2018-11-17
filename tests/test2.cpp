// This file is part of cpphtmltags
// test2.cpp

#include "../cpphtmltags.hpp"

using namespace std;
using namespace htags;

void test2( En_LineFeedMode lfmode )
{
	HTAG::setLineFeedMode( lfmode );

// opening and closing tag
	HTAG t1( cout, HT_H1 );
	t1.openTag();
	cout << "my title 1 !";
	t1.closeTag();

// self closing tag
	{
		HTAG t2( cout, HT_H2 );
		t2.addAttrib( AT_STYLE , "font-size:150%;" );
		t2.openTag();
		cout << "my title 2 !";
	}

// putting attribute inside tag creation
	{
		HTAG t2( cout, HT_H2, AT_ID, "abc" );
		t2.addAttrib( AT_STYLE , "font-size:150%;" );
		t2.openTag();
		cout << "my title 3 !";
	}

	{
		HTAG t2( cout, HT_P, AT_CLASS, "parag" );
		t2.setContent( "this is some text" );
		t2 << " and I can do this, too";
		cout << t2;
		t2.setContent( "other text" );
		cout << t2;
	}
}

int main()
{
	cout << "* LineFeed mode: default\n";
	test2( LF_Default );

	cout << "\n* LineFeed mode: none\n";
	test2( LF_None );

	cout << "\n* LineFeed mode: always\n";
	test2( LF_Always );

	HTAG::setGlobalAttrib( HT_LI, AT_CLASS, "cl1" );
	HTAG li( HT_LI, AT_CLASS, "cl2" );
	li.addAttrib( AT_COLSPAN, 1 );
	cout << li;

}




