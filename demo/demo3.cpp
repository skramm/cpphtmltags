// This file is part of cpphtmltags

/**
\file
\brief demo of using macros to open tags. This technique has the advantage of showing the user the location of incorrect usage.
*/

#define HTTAG_SILENT_MODE
#include "../cpphtmltags.hpp"

using namespace std;
using namespace httag;

int main()
{
	HTTAG_OPENTAG( cout, t, HT_P );
	cout << "hi there";
	HTTAG_OPENTAG( cout, t2, HT_H2 );
	HTTAG_CLOSETAG( t );

	HTTAG_OPENTAG_AT( cout, t3, HT_P, AT_CLASS, "abc" );
	cout << "a paragraph";
	HTTAG_CLOSETAG( t3 );

	HTTAG_OPENTAG_C( cout, t4, HT_P, "a paragraph" );
	HTTAG_OPENTAG_C_AT( cout, t5, HT_P, "a paragraph", AT_CLASS, "c1" );

	HTTAG_CLOSETAG( t4 );

}
