// test_A.cpp

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define HTTAG_SILENT_MODE
#include "cpphtmltags.hpp"


using namespace httag;

TEST_CASE( "test1", "[t1]" )
{
	std::cout << "Running tests with catch " << CATCH_VERSION_MAJOR << '.' << CATCH_VERSION_MINOR << '.' << CATCH_VERSION_PATCH << '\n';

	SECTION( "creation of tag and streaming in two steps" ) {
	{
		std::ostringstream oss;
		Httag t1( HT_BR );
		oss << t1;
		CHECK( oss.str() == "<br>" );
	}
	{
		std::ostringstream oss;
		Httag t1( HT_P );
		oss << t1;
		CHECK( oss.str() == "<p></p>" );
	}
	{
		std::ostringstream oss;
		Httag t1( HT_P, AT_CLASS, "myclass" );
		oss << t1;
		CHECK( oss.str() == "<p class=\"myclass\"></p>" );
	}
	{
		std::ostringstream oss1, oss2;                     // with a std::string
		std::string content{ "a paragraph" };
		Httag t1( HT_P, content );
		oss1 << t1;
		CHECK( oss1.str() == "<p>a paragraph</p>" );
		Httag t2( HT_P, "a paragraph" );                     // with a const char*
		oss2 << t2;
		CHECK( oss2.str() == "<p>a paragraph</p>" );
	}
	{
		std::ostringstream oss;
		Httag t1( HT_P, "content", AT_CLASS, "myclass" );
		oss << t1;
		CHECK( oss.str() == "<p class=\"myclass\">content</p>" );
	}
	{
		std::ostringstream oss1, oss2;
		Httag t1( HT_P );
		t1 << "some";
		t1 << " text";
		oss1 << t1;
		CHECK( oss1.str() == "<p>some text</p>" );

		Httag t2( HT_P );
		t2.setContent( "some " );
		t2.addContent( "text" );
		oss2 << t2;
		CHECK( oss1.str() == oss2.str() );
	}

} // section end

	SECTION( "creation and streaming in single step" ) {
	{
		std::ostringstream oss;
		oss << Httag( HT_BR );
		CHECK( oss.str() == "<br>" );
	}
	{
		std::ostringstream oss;
		oss << Httag( HT_P );
		CHECK( oss.str() == "<p></p>" );
	}
	{
		std::ostringstream oss;
		oss << Httag( HT_P, AT_CLASS, "myclass" );
		CHECK( oss.str() == "<p class=\"myclass\"></p>" );
	}
	{
		std::ostringstream oss1, oss2;                     // with a std::string
		std::string content{ "a paragraph" };
		oss1 << Httag( HT_P, content );
		CHECK( oss1.str() == "<p>a paragraph</p>" );

		oss2 << Httag( HT_P, "a paragraph" );                 // with a const char*
		CHECK( oss2.str() == "<p>a paragraph</p>" );
	}
	{
		std::ostringstream oss;
		oss << Httag( HT_P, "content", AT_CLASS, "myclass" );
		CHECK( oss.str() == "<p class=\"myclass\">content</p>" );
	}
} // section end

	SECTION( "iterative adding of attributes" )
	{
		std::ostringstream oss,oss2,oss3;
		Httag t1( HT_P, "content", AT_CLASS, "myclass" );
		t1.addAttrib( AT_STYLE, "width:100%;" );
		oss << t1;
		CHECK( oss.str() == "<p class=\"myclass\" style=\"width:100%;\">content</p>" );

		t1.addAttrib( AT_CLASS, "abc" );
		oss2 << t1;
		CHECK( oss2.str() == "<p class=\"myclass abc\" style=\"width:100%;\">content</p>" );

		Httag::setGlobalAttrib( HT_P, AT_CLASS, "cdef" );
		oss3 << t1;
		CHECK( oss3.str() == "<p class=\"myclass abc cdef\" style=\"width:100%;\">content</p>" );

		CHECK( Httag::getGlobalAttrib( HT_P ) == "class=cdef" );
	}
	SECTION( "iterative adding of attributes 2" )
	{
		std::ostringstream oss1, oss2, oss3, oss4;

		oss1 << Httag( HT_P, "hi there" );
		CHECK( oss1.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes defined before stay

		Httag p3( HT_P, "hi there" );
		oss3 << p3;
		CHECK( oss3.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes stay

		oss2 << Httag( HT_LI, "this is li" );                // but only for the tag it was assigned too, others are unaffected
		CHECK( oss2.str() == "<li>this is li</li>\n" );

		Httag::clearGlobalAttrib( HT_P ); // clear global attribute for <p>
		oss4 << Httag( HT_P, "hi" );
		CHECK( oss4.str() == "<p>hi</p>" );
	}
	SECTION( "iterative adding of attributes 3" )
	{
		std::ostringstream oss;
		Httag p( oss, HT_P, "text" );
		p.addAttrib( AT_CLASS, "abc" );
		p.addAttrib( AT_CLASS, "cde" );
		p.printTag();
		CHECK( oss.str() == "<p class=\"abc cde\">text</p>" );
	}
#if 0
	SECTION( "numerical attributes worls too" )
	{
		std::ostringstream oss;
		Httag t( oss, HT_INPUT );
		t.addAttrib( AT_MIN, 3 );
		t.addAttrib( AT_MAX, 10 );
		t.printTag();
		CHECK( oss.str() == "<p min=\"3\" max=\"10\">text</p>" );
	}
#endif	
}

TEST_CASE( "Error checking", "[t2]" )
{
	SECTION( "Errors-1" )
	{
		CHECK( Httag::printOpenedTags( std::cout ) == 0 );

/// [error_test_1]
		Httag t0a( HT_P );
		CHECK_THROWS( t0a.openTag() ); // cannot open a non-file type tag
/// [error_test_1]

		Httag t0b( HT_P );
		CHECK_THROWS( t0b.closeTag() ); // cannot close a non-file type tag

		Httag t1( std::cout, HT_P );
		CHECK_THROWS( t1.closeTag() ); // cannot close before opening tag

		Httag t2( HT_BR );
		CHECK_THROWS( t2.addContent( "tag cannot have content" ) ); // void elements cannot have content
		CHECK_THROWS( Httag( HT_BR, "content" ) );  // same with constructor

		std::ostringstream oss;
		Httag t3a( oss, HT_H2 );       // checking for
		Httag t3b( oss, HT_STRONG );   // correct opening/closing order
		t3a.openTag();
		t3a << "title";
		t3b.openTag();

		CHECK( Httag::printOpenedTags( std::cout ) == 2 );

		t3b << "paragraph";
		CHECK_THROWS( t3a.closeTag() );
	}

	SECTION( "Errors-2" )
	{
		std::ostringstream oss;
		{
			Httag ta( oss, HT_P );
			Httag tb( oss, HT_P );
			ta.openTag();
			CHECK_THROWS( tb.openTag() );  // cannot open a <p> inside a <p>
		}
		{
			Httag t4( oss, HT_H2 );       // cannot add attributes to an already opened tag
			t4.openTag();
			CHECK_THROWS( t4.addAttrib( AT_CLASS, "abc" ) );
		}
		{
			Httag t( HT_TD );
			CHECK_THROWS( t.addAttrib( AT_DATA, "abc" ) ); // illegal attribute "data" for tag <td>
			CHECK_THROWS( Httag( HT_TD, AT_DATA, "abc" ) ); // same thing with constructor
		}
		{
			Httag p( oss, HT_P );
			p.openTag();
			Httag li( oss, HT_LI );
			CHECK_THROWS( li.openTag() ); // <p><li> is illegal
		}
	}
}

TEST_CASE( "File type tags", "[t3]" )
{
	SECTION( "File type tags 1" )
	{
		std::ostringstream oss, oss1, oss2, oss3, oss4;

		Httag t0( oss, HT_P );                        // adding content to a tag
		t0 << "content";
		oss << t0;
		CHECK( oss.str() == "<p>content</p>" );

		Httag t1( oss1, HT_P );
		t1 << "this ";
		t1 << "is text";
		oss1 << t1;
		CHECK( oss1.str() == "<p>this is text</p>" );
	}
	SECTION( "File type tags 2" )
	{
		std::ostringstream oss, oss1, oss2, oss3, oss4;

		Httag t0( oss, HT_P );                        // adding content to a tag
		t0 << "content";
		oss << t0;
		CHECK( oss.str() == "<p>content</p>" );

		Httag t1( oss1, HT_P );
		oss1 << t1;
		CHECK( oss1.str() == "<p></p>" );

		Httag t2( oss2, HT_P, "content" );
		oss2 << t2;
		CHECK( oss2.str() == "<p>content</p>" );

		Httag t3( oss3, HT_P, "content", AT_CLASS, "abc" );
		oss3 << t3;
		CHECK( oss3.str() == "<p class=\"abc\">content</p>" );

		Httag t4( oss4, HT_P, AT_CLASS, "abc" );
		oss4 << t4;
		CHECK( oss4.str() == "<p class=\"abc\"></p>" );
	}
	CHECK( Httag::printOpenedTags( std::cout ) == 0 );	
}

TEST_CASE( "tag closure", "[t4]" )
{
	SECTION( "self closing tag" )
	{
		{
			std::ostringstream oss;
			Httag t0( oss, HT_P );                        // adding content to a tag
			t0 << "content";
			t0.openTag();
			t0 << ", some more content";
			t0.printTag();
			CHECK( oss.str() == "<p>content, some more content</p>" );
		}
		{
			std::ostringstream oss;
			Httag t0( oss, HT_P );                        // adding content to a tag
			t0 << "some content";
			t0.printTag();
			CHECK( oss.str() == "<p>some content</p>" );
		}
	}
}

TEST_CASE( "test_cat", "[t5]" )
{
	CHECK( priv::tagBelongsToCat( HT_H1,      priv::C_HEADING    ) );
	CHECK( priv::tagBelongsToCat( HT_A,       priv::C_FLOW       ) );
	CHECK( priv::tagBelongsToCat( HT_A,       priv::C_PHRASING   ) );
	CHECK( priv::tagBelongsToCat( HT_ARTICLE, priv::C_SECTIONING ) );
	CHECK( priv::tagBelongsToCat( HT_AUDIO,   priv::C_EMBEDDED   ) );
}

TEST_CASE( "test_ac", "[t6]" ) // testing allowed content in a tag
{
	std::ostringstream oss;
	{
		Httag t1( oss, HT_P );
		Httag t2( oss, HT_H1 );
		Httag t3( oss, HT_STRONG );
		t1.openTag();
		oss << "this is ";
		CHECK_THROWS( t2.openTag() );      // cannot have <h1> inside <p>
		CHECK_NOTHROW( t3.openTag() );     // but <strong> is ok
		oss << "important";
		t3.closeTag();
		oss << ", is it ?";
	}
	CHECK( oss.str() == "<p>this is <strong>important</strong>, is it ?</p>" );
}
TEST_CASE( "test_void", "[t7]" ) // testing void elements
{
	CHECK( priv::isVoidElement(HT_IMG) );
	CHECK( priv::isVoidElement(HT_META) );
	CHECK( priv::isVoidElement(HT_BR) );
	CHECK( priv::isVoidElement(HT_HR) );
}
