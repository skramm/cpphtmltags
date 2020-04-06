// This file is part of cpphtmltags

/**
\file
\brief demo of using macros to open tags. This technique has the advantage of showing the user the location of incorrect usage.
*/

//#define HTTAG_SILENT_MODE
#include "../cpphtmltags.hpp"

using namespace std;
using namespace httag;

int main()
{
	Httag p( cout, HT_P );
	Httag h2( cout, HT_H2 );
	HTTAG_OPENTAG( p );
	cout << "hi there";
	HTTAG_OPENTAG( h2 ); // invalid: cannot open a <h2> inside <p>
	HTTAG_CLOSETAG( p );

	Httag p2( cout, HT_P, AT_CLASS, "abc" );
	HTTAG_OPENTAG( p2 );
	cout << "a paragraph";
	HTTAG_CLOSETAG( p2 );
/*
	HTTAG_OPENTAG_C( cout, t4, HT_P, "a paragraph" );
	HTTAG_OPENTAG_C_AT( cout, t5, HT_P, "a paragraph", AT_CLASS, "c1" );

	HTTAG_CLOSETAG( t4 );
*/
}
