// test_A.cpp

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define HTTAG_SILENT_MODE
#include "../cpphtmltags.hpp"


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
		CHECK( oss1.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes stay

		Httag p3( HT_P, "hi there" );
		oss3 << p3;
		CHECK( oss3.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes stay

		oss2 << Httag( HT_LI, "this is li" );                // but only for the tag it was assigned too, others are unaffected
		CHECK( oss2.str() == "<li>this is li</li>" );

		Httag::clearGlobalAttrib( HT_P ); // clear global attribute for <p>
		oss4 << Httag( HT_P, "hi" );
		CHECK( oss4.str() == "<p>hi</p>" );
	}
}

TEST_CASE( "Error checking", "[t2]" )
{
	SECTION( "Errors-1" )
	{
	CHECK( Httag::printOpenedTags( std::cout ) == 0 );

	Httag t0a( HT_P );
	CHECK_THROWS( t0a.openTag() ); // cannot open a non-file type tag

	Httag t0b( HT_P );
	CHECK_THROWS( t0b.closeTag() ); // cannot close a non-file type tag

	Httag t1( std::cout, HT_P );
	CHECK_THROWS( t1.closeTag() ); // cannot close before opening tag

	Httag t2( HT_BR );
	CHECK_THROWS( t2.addContent( "tag cannot have content" ) );


	std::ostringstream oss;
	Httag t3a( oss, HT_H2 );       // checking for
	Httag t3b( oss, HT_STRONG );        // correct opening/closing order
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
		Httag ta( oss, HT_P );
		Httag tb( oss, HT_P );
		ta.openTag();
		CHECK_THROWS( tb.openTag() );

		Httag t4( oss, HT_H2 );       // cannot add attributes to an already opened tag
		t4.openTag();
		CHECK_THROWS( t4.addAttrib( AT_CLASS, "abc" ) );
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

