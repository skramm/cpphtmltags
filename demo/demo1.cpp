// This file is part of cpphtmltags

// demo1.cpp

#include "../cpphtmltags.hpp"

using namespace std;
using namespace httag;

int main()
{
//	Httag::setLineFeedMode( LF_Always );
	Httag t1( HT_BR );
	cout << t1;

	cout << Httag( HT_P );
	cout << Httag( HT_P, "first paragraph" );

	Httag t2( HT_P );
	t2.addAttrib( AT_CLASS, "aaa" );
	t2.setContent( "this is a paragraph" );
	cout << t2;

	t2.setContent( 42 );
	cout << t2;

	t2.addAttrib( AT_ID, "tag_id" );
	cout << t2;

	t2.addAttrib( AT_CLASS, "bbb" );
	t2.setContent( "t2: Hello World" );
	cout << t2;
	Httag::setGlobalAttrib( HT_P, AT_STYLE, "globalstyle" );

	Httag t3( HT_P, "t3" );
	cout << t2 << t3;

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
	}

	Httag::printOpenedTags( cout, "before TEST" );

cout << "\nTEST\n";

		std::ostringstream oss;
		{
			Httag t0( oss, HT_P );                        // adding content to a tag
			t0 << "content";
			t0.openTag();
			t0 << ", some more content";
			t0.printTag();
			Httag::printOpenedTags( cout, "A" );
			cout << oss.str() << "\n";
		}
		Httag::printOpenedTags( cout, "B" );

		{
			Httag t0( oss, HT_P );                        // adding content to a tag
			t0 << "some content";
			Httag::printOpenedTags( cout, "C" );
			t0.printTag();
			Httag::printOpenedTags( cout, "D" );
			cout << oss.str() << "\n";
		}
}
