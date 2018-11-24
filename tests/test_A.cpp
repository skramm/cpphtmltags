// test_A.cpp

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define HTAGS_SILENT_MODE
#include "../cpphtmltags.hpp"


using namespace htags;

TEST_CASE( "test1", "[mytest]" )
{
	std::cout << "Running tests with catch " << CATCH_VERSION_MAJOR << '.' << CATCH_VERSION_MINOR << '.' << CATCH_VERSION_PATCH << '\n';

	SECTION( "creation of tag and streaming in two steps" ) {
	{
		std::ostringstream oss;
		HTAG t1( HT_BR );
		oss << t1;
		CHECK( oss.str() == "<br>" );
	}
	{
		std::ostringstream oss;
		HTAG t1( HT_P );
		oss << t1;
		CHECK( oss.str() == "<p></p>" );
	}
	{
		std::ostringstream oss;
		HTAG t1( HT_P, AT_CLASS, "myclass" );
		oss << t1;
		CHECK( oss.str() == "<p class=\"myclass\"></p>" );
	}
	{
		std::ostringstream oss1, oss2;                     // with a std::string
		std::string content{ "a paragraph" };
		HTAG t1( HT_P, content );
		oss1 << t1;
		CHECK( oss1.str() == "<p>a paragraph</p>" );
		HTAG t2( HT_P, "a paragraph" );                     // with a const char*
		oss2 << t2;
		CHECK( oss2.str() == "<p>a paragraph</p>" );
	}
	{
		std::ostringstream oss;
		HTAG t1( HT_P, "content", AT_CLASS, "myclass" );
		oss << t1;
		CHECK( oss.str() == "<p class=\"myclass\">content</p>" );
	}
} // section end

	SECTION( "creation and streaming in single step" ) {
	{
		std::ostringstream oss;
		oss << HTAG( HT_BR );
		CHECK( oss.str() == "<br>" );
	}
	{
		std::ostringstream oss;
		oss << HTAG( HT_P );
		CHECK( oss.str() == "<p></p>" );
	}
	{
		std::ostringstream oss;
		oss << HTAG( HT_P, AT_CLASS, "myclass" );
		CHECK( oss.str() == "<p class=\"myclass\"></p>" );
	}
	{
		std::ostringstream oss1, oss2;                     // with a std::string
		std::string content{ "a paragraph" };
		oss1 << HTAG( HT_P, content );
		CHECK( oss1.str() == "<p>a paragraph</p>" );

		oss2 << HTAG( HT_P, "a paragraph" );                 // with a const char*
		CHECK( oss2.str() == "<p>a paragraph</p>" );
	}
	{
		std::ostringstream oss;
		oss << HTAG( HT_P, "content", AT_CLASS, "myclass" );
		CHECK( oss.str() == "<p class=\"myclass\">content</p>" );
	}
} // section end

	SECTION( "iterative adding of attributes" )
	{
		std::ostringstream oss,oss2,oss3;
		HTAG t1( HT_P, "content", AT_CLASS, "myclass" );
		t1.addAttrib( AT_STYLE, "width:100%;" );
		oss << t1;
		CHECK( oss.str() == "<p style=\"width:100%;\" class=\"myclass\">content</p>" );

		t1.addAttrib( AT_CLASS, "abc" );
		oss2 << t1;
		CHECK( oss2.str() == "<p style=\"width:100%;\" class=\"myclass abc\">content</p>" );

		HTAG::setGlobalAttrib( HT_P, AT_CLASS, "cdef" );
		oss3 << t1;
		CHECK( oss3.str() == "<p style=\"width:100%;\" class=\"myclass abc cdef\">content</p>" );

		CHECK( HTAG::getGlobalAttrib( HT_P ) == "class=cdef" );
	}
	SECTION( "iterative adding of attributes 2" )
	{
		std::ostringstream oss1, oss2, oss3, oss4;

		oss1 << HTAG( HT_P, "hi there" );
		CHECK( oss1.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes stay

		HTAG p3( HT_P, "hi there" );
		oss3 << p3;
		CHECK( oss3.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes stay

		oss2 << HTAG( HT_LI, "this is li" );                // but only for the tag it was assigned too, others are unaffected
		CHECK( oss2.str() == "<li>this is li</li>" );

		HTAG::clearGlobalAttrib( HT_P ); // clear global attribute for <p>
		oss4 << HTAG( HT_P, "hi" );
		CHECK( oss4.str() == "<p>hi</p>" );
	}
}

TEST_CASE( "Error checking", "[mytest]" )
{
	HTAG t0( HT_P );
	CHECK_THROWS( t0.openTag() ); // cannot open a non-file type tag

	HTAG t1( HT_P );
	CHECK_THROWS( t1.closeTag() ); // cannot open a non-file type tag

	HTAG t2( HT_BR );
	CHECK_THROWS( t2.addContent( "tag cannot have content" ) );
}

TEST_CASE( "File type tags", "[mytest]" )
{
	SECTION( "File type tags 1" )
	{
		std::ostringstream oss, oss1, oss2, oss3, oss4;

		HTAG t0( oss, HT_P );                        // adding content to a tag
		t0 << "content";
		oss << t0;
		CHECK( oss.str() == "<p>content</p>" );

		HTAG t1( oss1, HT_P );
		t1 << "this ";
		t1 << "is text";
		oss1 << t1;
		CHECK( oss1.str() == "<p>this is text</p>" );
	}
	SECTION( "File type tags 2" )
	{
		std::ostringstream oss, oss1, oss2, oss3, oss4;

		HTAG t0( oss, HT_P );                        // adding content to a tag
		t0 << "content";
		oss << t0;
		CHECK( oss.str() == "<p>content</p>" );

		HTAG t1( oss1, HT_P );
		oss1 << t1;
		CHECK( oss1.str() == "<p></p>" );

		HTAG t2( oss2, HT_P, "content" );
		oss2 << t2;
		CHECK( oss2.str() == "<p>content</p>" );

		HTAG t3( oss3, HT_P, "content", AT_CLASS, "abc" );
		oss3 << t3;
		CHECK( oss3.str() == "<p class=\"abc\">content</p>" );

		HTAG t4( oss4, HT_P, AT_CLASS, "abc" );
		oss4 << t4;
		CHECK( oss4.str() == "<p class=\"abc\"></p>" );
	}
}

TEST_CASE( "tag closure", "[mytest]" )
{
	SECTION( "self closing tag" )
	{
		std::ostringstream oss0, oss1, oss2, oss3, oss4;
		{
			HTAG t0( oss0, HT_P );                        // adding content to a tag
			t0 << "content";
			t0.openTag();
			t0 << ", some more content";
			t0.printTag();
		}
		CHECK( oss0.str() == "<p>content, some more content</p>" );

		{
			HTAG t0( oss1, HT_P );                        // adding content to a tag
			t0 << "some content";
			t0.printTag();
		}
		CHECK( oss1.str() == "<p>some content</p>" );
	}
}

