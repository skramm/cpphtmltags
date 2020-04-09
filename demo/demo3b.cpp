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
	Httag t1( cout, HT_DOCTYPE );
	HTTAG_OPEN( t1 );

	Httag t ( cout, HT_HTML );
	{
		Httag head(  cout, HT_HEAD );
		Httag title( cout, HT_TITLE );

		HTTAG_OPEN( t );
		HTTAG_OPEN( head );
		HTTAG_OPEN( title );
		cout << "my title";
	}
	Httag body( cout, HT_BODY );
	HTTAG_ADDATTRIB( body, AT_STYLE, "color:red;" );
//	HTTAG_ADD( body, "This is the page content" );
	body.setContent( "azeazeaz");
	HTTAG_OPEN( body );

	Httag p( cout, HT_P );
	HTTAG_SET( p, "This is a paragraph" );
	HTTAG_OPEN( p );
}
