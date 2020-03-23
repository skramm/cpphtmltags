// This file is part of cpphtmltags

// demo0.cpp

#include "../cpphtmltags.hpp"

using namespace std;
using namespace httag;

int main()
{
	{                                   // use of self-closing tags: needs some scope handling
		cout << Httag( HT_DOCTYPE );
		Httag tc( HT_COMMENT, "This is a comment" );
		cout << tc;

		Httag html( std::cout, HT_HTML );
		html.openTag();
		{
			Httag head( std::cout, HT_HEAD );
			head.openTag();
			{
				Httag title( std::cout, HT_TITLE, "my title" );
				title.printTag();
			}
		}
		Httag body( std::cout, HT_BODY );
		body.openTag();
	}
	cout << '\n';
	{                                   // manual tag closing
		cout << Httag( HT_DOCTYPE );
		Httag tc( HT_COMMENT );
		tc << "This is a comment";
		cout << tc;

		Httag html(  cout, HT_HTML );
		Httag head(  cout, HT_HEAD );
		Httag title( cout, HT_TITLE, "my title" );
		Httag body(  cout, HT_BODY );
		html.openTag();
		head.openTag();
		std::cout << title;
		head.closeTag();
		body.openTag();
		body.closeTag();
	}
// EXAMPLE 3
	{
		Httag html( HT_HTML );
		Httag head( HT_HEAD );
		Httag body( HT_BODY );
		head << Httag( HT_TITLE, "my title" );
		body << Httag( HT_DIV, AT_ID, "div1" );  // an empty div
		Httag div2( HT_DIV, AT_ID, "div2" );
		div2 << Httag( HT_P, "Some text" );
		body << div2;
		html << head << body;
		std::cout << html;
	}
}
