// This file is part of cpphtmltags
// demo2a.cpp

#include "../cpphtmltags.hpp"

using namespace std;
using namespace httag;

void test2( rto::En_LineFeedMode lfmode )
{
	Httag t1( cout, HT_H1 );    // opening and closing tag
	t1.openTag();
	cout << "my title 1 !";
	t1.closeTag();

	{                               // self closing tag
		Httag t2( cout, HT_H2 );
		t2.addAttrib( AT_STYLE , "font-size:150%;" );
		t2.openTag();
		cout << "my title 2 !";
	}
	{                         // putting attribute inside tag creation
		Httag t2( cout, HT_H3, AT_ID, "abc" );
		t2.addAttrib( AT_STYLE , "font-size:200%;" );
		t2.openTag();
		cout << "my title 3 !";
	}
	{
		Httag t2( cout, HT_P, AT_CLASS, "parag" );
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
	test2( rto::LF_Default );

	cout << "\n* LineFeed mode: none\n";
	test2( rto::LF_None );

	cout << "\n* LineFeed mode: always\n";
	test2( rto::LF_Always );

	Httag::setGlobalAttrib( HT_LI, AT_CLASS, "cl1" );
	Httag li( HT_LI, AT_CLASS, "cl2" );
	li.addAttrib( AT_COLSPAN, 2 );
	cout << li;

}




