// test_A.cpp

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#define HTTAG_SILENT_MODE
//#define HTTAG_NO_CHECK
#include "cpphtmltags.hpp"

using namespace httag;

TEST_CASE( "test1", "[t1]" )
{
	std::cout << "Running tests with catch " << CATCH_VERSION_MAJOR << '.' << CATCH_VERSION_MINOR << '.' << CATCH_VERSION_PATCH << '\n';
	Httag::setOption( RTO_LFMode, LF_None );
	Httag::setOption( RTO_ClearOnClose, true );

//	Httag::setLineFeedMode( LF_None );
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
		t1 << " text ";
		t1 << 42;
		oss1 << t1;
		CHECK( oss1.str() == "<p>some text 42</p>" );

		Httag t2( HT_P );
		t2.setContent( "some " );
		t2.addContent( "text " );
		t2.addContent( 42 );
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

SECTION( "tag inside a tag" )
{
	{
		std::ostringstream oss;
		Httag a( HT_A, "a link", AT_HREF, "https://somewhere.com" );
		Httag li( HT_LI, a );
		oss << li;
		CHECK( oss.str() == "<li><a href=\"https://somewhere.com\">a link</a></li>" );
	}
	{
		std::ostringstream oss;
		Httag li( HT_LI );
		li << Httag( HT_A, "a link", AT_HREF, "https://somewhere.com" );
		oss << li;
		CHECK( oss.str() == "<li><a href=\"https://somewhere.com\">a link</a></li>" );
	}
	{
		std::ostringstream oss;
		oss << Httag ( HT_P ).setContent( "say " ).addContent( "hello" );
		CHECK( oss.str() == "<p>say hello</p>" );
	}
}

	SECTION( "iterative adding of attributes" )
	{
		Httag::setOption( RTO_ClearOnClose, false );
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

//		CHECK( Httag::getGlobalAttrib( HT_P ) == "class=cdef" );
	}
	SECTION( "iterative adding of attributes 2" )
	{
		std::ostringstream oss1, oss2, oss3, oss4;

		oss1 << Httag( HT_P, "hi there" );
		CHECK( oss1.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes defined before stay

		{
			std::ostringstream oss;
			Httag p3( oss, HT_P, "hi there" );
			p3.printTag();
			CHECK( oss.str() == "<p class=\"cdef\">hi there</p>" ); // global attributes stay
			oss.str(""); // clear
			p3.clearContent();
			p3.printTag();
			CHECK( oss.str() == "<p class=\"cdef\"></p>" ); // global attributes stay, even when tag emptied
		}

		oss2 << Httag( HT_LI, "this is li" );                // but only for the tag it was assigned too, others are unaffected
		CHECK( oss2.str() == "<li>this is li</li>" );


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
	SECTION( "numerical attributes works too" )
	{
		std::ostringstream oss;
		Httag t( oss, HT_INPUT );
		t.addAttrib( AT_MIN, 3 );
		t.addAttrib( AT_MAX, 10 );

		Httag::setOption( RTO_ClearOnClose, false );

		Httag::setOption( RTO_IllegalOp, EM_Throw );
		CHECK_THROWS( t.printWithContent( "text" ) );   // <input> is a void tag, thus, no content allowed !
		Httag::setOption( RTO_IllegalOp, EM_NoThrow );
		CHECK_NOTHROW( t.printWithContent( "text" ) );   // <input> is a void tag, thus, no content allowed !

		t.printTag();
		CHECK( oss.str() == "<input max=\"10\" min=\"3\">" );
	}
}

TEST_CASE( "Global attributes handling", "[t1b]" )
{
	SECTION( "global attributes only" )
	{
		std::ostringstream oss;
		Httag p( oss, HT_P, "Hi" );

		Httag::setGlobalAttrib( HT_P, AT_CLASS, "c1" );
		p.printTag();
		CHECK( oss.str() == "<p class=\"c1\">Hi</p>" );

		Httag::setGlobalAttrib( HT_P, AT_CLASS, "c2" );        // replace previous global attr
		oss.str("");
		p.printTag();
		CHECK( oss.str() == "<p class=\"c2\">Hi</p>" );

		oss.str("");
		oss << Httag( HT_P, "aaa" );
		CHECK( oss.str() == "<p class=\"c2\">aaa</p>" );          // new created tag also has it

		Httag::setGlobalAttrib( HT_P, AT_DIR, "123" );        // new global attr for <p>

		oss.str("");
		p.printTag();
		CHECK( oss.str() == "<p class=\"c2\" dir=\"123\">Hi</p>" );

		Httag::clearGlobalAttribs();
		oss.str("");
		p.printTag();
		CHECK( oss.str() == "<p>Hi</p>" );          // no more global attributes
	}

	SECTION( "Global attributes mixed with non-global" )
	{
		std::ostringstream oss;
		Httag p( oss, HT_P, "Hi", AT_CLASS, "c1" );         // <p> with class

		Httag::setGlobalAttrib( HT_P, AT_CLASS, "c2" );     // add a global attribute on <p>
		p.printTag();
		CHECK( oss.str() == "<p class=\"c1 c2\">Hi</p>" );  // so we get both

		oss.str("");
		oss << Httag( HT_LI, "inside", AT_CLASS, "c3" );     // but other tags dont have it
		CHECK( oss.str() == "<li class=\"c3\">inside</li>" );

		oss.str("");
		oss << Httag( HT_P, "aaa", AT_CLASS, "c1" );
		CHECK( oss.str() == "<p class=\"c1 c2\">aaa</p>" );    // new created tag also has it

		oss.str("");
		oss << Httag( HT_P, "bbb");
		CHECK( oss.str() == "<p class=\"c2\">bbb</p>" );          // new created tag also has it

		Httag::clearGlobalAttribs();
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
		CHECK_THROWS( t3a.closeTag() ); // cannot close t3a, need to close t3b first
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
		std::ostringstream oss, oss1;

//		Httag t0( oss, HT_P );                        // adding content to a tag
		Httag t0( HT_P );                        // adding content to a tag
		t0 << "content";
		oss << t0;
		CHECK( oss.str() == "<p>content</p>" );
/*
		Httag t1( oss1, HT_P );
		t1 << "this ";
		t1 << "is text";
		oss1 << t1;
		CHECK( oss1.str() == "<p>this is text</p>" );
*/
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
		std::ostringstream oss;
		Httag t0( oss, HT_P );                        // adding content to a tag
		t0 << "content";
		t0.openTag();
		t0 << ", some more content";
		t0.printTag();
		CHECK( oss.str() == "<p>content, some more content</p>" );

		Httag::setOption( RTO_ClearOnClose, true );
		oss.str(""); // clear
		t0.printTag();                       // once printed, the tag keeps its content
		CHECK( oss.str() == "<p>content, some more content</p>" );

		oss.str(""); // clear
		t0.printTag();                       // once printed,
		CHECK( oss.str() == "<p></p>" );     // the tag becomes empty

		oss.str("");
		Httag t1( oss, HT_P );                        // adding content to a tag
		t1 << "some content";
		t1.printTag();
		CHECK( oss.str() == "<p>some content</p>" );
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
//	Httag::setClosingTagClearsContent( true );
//	Httag::setLineFeedMode( LF_None );
	SECTION( "t6-1" )
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

	SECTION( "t6-2" )
	{
		std::ostringstream oss;

		Httag t( HT_DIV, Httag( HT_DIV ) );
		oss << t;
		CHECK( oss.str() == "<div><div></div></div>" );

		oss.str("");
		Httag t2( HT_DIV, Httag( HT_P, "text" ) );
		oss << t2;
		CHECK( oss.str() == "<div><p>text</p></div>" );

		CHECK_THROWS( Httag( HT_P ).setContent( Httag( HT_P ) ) );  // can't put a <p> inside a <p>
		CHECK_THROWS( Httag( HT_P ).setContent( Httag( HT_DIV ) ) );  // can't put a <div> inside a <p>
		CHECK_THROWS( Httag( HT_A ).setContent( Httag( HT_A ) ) );  // can't put a <a> inside a <a>

		Httag t3( HT_DFN );
		CHECK_NOTHROW( t3.setContent( Httag( HT_BR ) ) );  // <br>  allowed inside     <dfn>
		CHECK_THROWS(  t3.setContent( Httag( HT_P ) ) );   // <p>   not allowed inside <dfn>
		CHECK_THROWS(  t3.setContent( Httag( HT_DFN ) ) ); // <dfn> not allowed inside <dfn>
	}
	SECTION( "t6-3" )
	{
		std::ostringstream oss;
		Httag p( oss, HT_P );
		p.openTag();

		Httag li( oss, HT_LI );
		CHECK_THROWS( li.openTag() );  // <li> not allowed in <p>

		CHECK_THROWS( p << Httag( HT_LI ) );  // <li> not allowed in <p>
		oss << p;
	}
}


TEST_CASE( "test_void", "[t7]" ) // testing void elements
{
	CHECK( priv::isVoidElement(HT_IMG) );
	CHECK( priv::isVoidElement(HT_META) );
	CHECK( priv::isVoidElement(HT_BR) );
	CHECK( priv::isVoidElement(HT_HR) );
	CHECK( priv::isVoidElement(HT_INPUT) );

	std::ostringstream oss;
	Httag t( HT_COMMENT );
	t << "something";
	oss << t;
	CHECK( oss.str() == "<!-- something -->" );

	oss.str("");
	Httag t2( oss, HT_COMMENT );
	t2.openTag();
	oss << "else";
	t2.closeTag();
	CHECK( oss.str() == "<!-- else -->" );

	CHECK_THROWS( Httag(HT_DOCTYPE).addContent( "42" ) );
	CHECK_THROWS( Httag(HT_HR).addContent( "42" ) );

	CHECK_THROWS( Httag(HT_DOCTYPE, "42" ) );
	CHECK_THROWS( Httag(HT_HR, "42" ) );

	oss.str("");
	Httag t1(oss, HT_HTML );
	t1.openTag();
	CHECK_THROWS( oss << Httag(HT_DOCTYPE) );  // doctype needs to be the FIRST tag
}

TEST_CASE( "chained functions", "[t8]" )
{
//	Httag::setClosingTagClearsContent( false );
	Httag::setOption( RTO_ClearOnClose, false );

	std::ostringstream oss;
	oss << Httag( HT_P ).addAttrib( AT_CLASS, "aaa" ).addAttrib( AT_ID, "bbb" );
	CHECK( oss.str() == "<p class=\"aaa\" id=\"bbb\"></p>" );

	oss.str( "" );
	oss << Httag( HT_P ).addAttrib( AT_CLASS, "aaa" ).addContent( "text" ).addAttrib( AT_ID, "bbb" );
	CHECK( oss.str() == "<p class=\"aaa\" id=\"bbb\">text</p>" );

	oss.str( "" );
	std::string hello{"hello"};
	Httag p( HT_P );
	p.setContent( "say " ).addContent( hello );
	oss << p;  //
	CHECK( oss.str() == "<p>say hello</p>" );

	SECTION( "t8-b" )
	{
		Httag td1( HT_TD, "text", AT_COLSPAN, 3 );
		Httag td2 = td1;

		oss.str( "" );
		oss << td1;
		CHECK( oss.str() == "<td colspan=\"3\">text</td>" );

		td1.removeAttrib( AT_COLSPAN ).addAttrib( AT_ID, "myid" );
		oss.str( "" );
		oss << td1;
		CHECK( oss.str() == "<td id=\"myid\">text</td>" );

		td2.addAttrib( AT_ID, "myid" ).removeAttrib( AT_COLSPAN );
		oss.str( "" );
		oss << td2;
		CHECK( oss.str() == "<td id=\"myid\">text</td>" );

		td2.clearAttribs().addContent( " more" );
		oss.str( "" );
		oss << td2;
		CHECK( oss.str() == "<td>text more</td>" );
	}
}

TEST_CASE( "clearing content", "[t9]" )
{
	std::ostringstream oss;
	SECTION( "t9-a: file-typetags" )
	{
//		Httag::setClosingTagClearsContent( false );
		Httag::setOption( RTO_ClearOnClose, false );
		Httag p( oss, HT_P, "text", AT_CLASS, "c1" );

		oss.str( "" );
		p.printTag();
		CHECK( oss.str() == "<p class=\"c1\">text</p>" );

		oss.str( "" );
		p.printTag();            // no change
		CHECK( oss.str() == "<p class=\"c1\">text</p>" );

//		Httag::setClosingTagClearsContent( true );
		Httag::setOption( RTO_ClearOnClose, true );

		oss.str( "" );
		p.printTag();
		CHECK( oss.str() == "<p class=\"c1\">text</p>" );

		oss.str( "" );
		p.printTag();            // tag has been cleared
		CHECK( oss.str() == "<p class=\"c1\"></p>" );
	}
	SECTION( "t9-b: normal tags" )
	{
		Httag p1(HT_P, "text");

//		Httag::setClosingTagClearsContent( false );
		Httag::setOption( RTO_ClearOnClose, false );
		oss.str( "" );
		oss << p1;
		CHECK( oss.str() == "<p>text</p>" );
		oss.str( "" );
		oss << p1;
		CHECK( oss.str() == "<p>text</p>" );

//		Httag::setClosingTagClearsContent( true );
		Httag::setOption( RTO_ClearOnClose, true );
		Httag p2(HT_P, "text");
		oss.str( "" );
		oss << p2;
		CHECK( oss.str() == "<p>text</p>" );
		oss.str( "" );
		oss << p2;
		CHECK( oss.str() == "<p></p>" );
	}
}

TEST_CASE( "boolean attributes", "[t10]" )
{
	Httag::setOption( RTO_ClearOnClose, false );
//	Httag::setClosingTagClearsContent( false );
	std::ostringstream oss;

	oss << Httag( HT_INPUT ).addAttrib( AT_CHECKED);
	CHECK( oss.str() == "<input checked>" );

	oss.str("");
	oss << Httag( HT_INPUT ).addAttrib( AT_CHECKED ).addAttrib( AT_CLASS, "abc" );
	CHECK( oss.str() == "<input checked class=\"abc\">" );

	oss.str("");
	oss << Httag( HT_INPUT, AT_CHECKED, "" );
	CHECK( oss.str() == "<input checked>" );
}

TEST_CASE( "constructor as content", "[t11]" )
{
//	Httag::setClosingTagClearsContent( false );
	std::ostringstream oss;

	Httag t( HT_DIV, Httag( HT_DIV, "aaa" ) );
	oss << t;
	CHECK( oss.str() == "<div><div>aaa</div></div>" );

	oss.str("");
	Httag p( HT_P, "This is " + Httag( HT_STRONG, "bold text" ).to_string() );
	oss << p;
	CHECK( oss.str() == "<p>This is <strong>bold text</strong></p>" );

	std::string s =  Httag( HT_STRONG, "bold text" );      // auto conversion to string
	CHECK( s == "<strong>bold text</strong>" );

	s = "This is " + Httag( HT_STRONG, "bold text" );  // so we can write this
	CHECK( s == "This is <strong>bold text</strong>" );

	oss.str("");
	Httag p2( HT_P, ( "This is " + Httag( HT_STRONG, "bold text" ) ) );
	oss << p2;
	CHECK( oss.str() == "<p>This is <strong>bold text</strong></p>" );
}

TEST_CASE( "complete minimal page", "[t12]" )
{
	std::ostringstream oss;
	oss << Httag( HT_DOCTYPE );
	Httag html( HT_HTML );
	Httag head( HT_HEAD );
	Httag body( HT_BODY );
	head << Httag( HT_TITLE, "Title" );
	body << Httag( HT_DIV, AT_ID, "div1" );  // an empty div
	Httag div2( HT_DIV, AT_ID, "div2" );
	div2 << Httag( HT_P, "Some text" );
	body << div2;
	html << head << body;
	oss << html;
	CHECK( oss.str() =="<!DOCTYPE html>\n<html><head><title>Title</title></head><body><div id=\"div1\"></div><div id=\"div2\"><p>Some text</p></div></body></html>" );
}

