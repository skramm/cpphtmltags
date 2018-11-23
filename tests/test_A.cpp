// test_A.cpp

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../cpphtmltags.hpp"

using namespace htags;

TEST_CASE( "test1", "[mytest]" )
{
	SECTION( "creation of tag and streaming in two steps" )
	{
		std::ostringstream oss;
		HTAG t1( HT_BR );
		oss << t1;
		REQUIRE( oss.str() == "<br>" );
	}
	{
		std::ostringstream oss;
		HTAG t1( HT_P );
		oss << t1;
		REQUIRE( oss.str() == "<p></p>" );
	}
	{
		std::ostringstream oss;
		HTAG t1( HT_P, AT_CLASS, "myclass" );
		oss << t1;
		REQUIRE( oss.str() == "<p class=\"myclass\"></p>" );
	}
	{
		std::ostringstream oss1, oss2;                     // with a std::string
		std::string content{ "a paragraph" };
		HTAG t1( HT_P, content );
		oss1 << t1;
		REQUIRE( oss1.str() == "<p>a paragraph</p>" );
		HTAG t2( HT_P, "a paragraph" );                     // with a const char*
		oss2 << t2;
		REQUIRE( oss2.str() == "<p>a paragraph</p>" );
	}

	SECTION( "creation and streaming in single step" )
	{
		std::ostringstream oss;
		oss << HTAG( HT_BR );
		REQUIRE( oss.str() == "<br>" );
	}
	{
		std::ostringstream oss;
		oss << HTAG( HT_P );
		REQUIRE( oss.str() == "<p></p>" );
	}
	{
		std::ostringstream oss;
		oss << HTAG( HT_P, AT_CLASS, "myclass" );
		REQUIRE( oss.str() == "<p class=\"myclass\"></p>" );
	}
	{
		std::ostringstream oss1, oss2;                     // with a std::string
		std::string content{ "a paragraph" };
		oss1 << HTAG( HT_P, content );
		REQUIRE( oss1.str() == "<p>a paragraph</p>" );

		oss2 << HTAG( HT_P, "a paragraph" );                 // with a const char*
		REQUIRE( oss2.str() == "<p>a paragraph</p>" );
	}
}

