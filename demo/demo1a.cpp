// This file is part of cpphtmltags

// demo1a.cpp

#include "../cpphtmltags.hpp"

using namespace std;
using namespace httag;

int main()
{
	cout << Httag( HT_P, "first paragraph" );           // direct string content

	cout << Httag( HT_COMMENT, "this is a comment" );       // a comment

	Httag t2( HT_P );                                           // first we create the tag,
	t2.addAttrib( AT_CLASS, "aaa" );                            // add an attribute,
	t2.setContent( "this is a paragraph with class 'aaa'" );    // add content,
	cout << t2;                                                 // and stream it

	t2.setContent( 42 );                                        // we can reuse it: keeps the attribute
	cout << t2;

	t2.addAttrib( AT_ID, "tag_id" );                            // and add an 'id' attribute
	cout << t2;

	t2.addAttrib( AT_CLASS, "bbb" );
	t2.setContent( "t2: Hello World of class 'aaa' and 'bbb'" );
	cout << t2;

	Httag::setGlobalAttrib( HT_P, AT_STYLE, "globalstyle" );       // we add a global style

	Httag t3( HT_P, "t3" );
	cout << t2;
	cout << t3;

	cout << "FILE-TYPE TAGS:\n";
	{
		Httag f1( cout, HT_A );
		f1.openTag();
		cout << "hi there 1";
		f1.closeTag( true );
	}

	{
		Httag f1( cout, HT_A, AT_CLASS, "abc" );
		f1.openTag();
		cout << "hi there 2";
	}

	{
		Httag f1( cout, HT_P, AT_CLASS, "cde" );
		f1.openTag();
		cout << "para\n";

		Httag::printOpenedTags( cout );

		Httag f2( cout, HT_LI );           // this triggers an error, you cannot embbed a <li> inside a <p>
		f2.openTag();
		cout << "li-content";
		f2.closeTag();
	}
}
