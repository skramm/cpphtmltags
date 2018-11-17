/*
    Copyright (C) 2018 Sebastien Kramm

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
\file cpphtmltags.hpp
\brief a C++11 header-only library for generating html5 valid code from your app

homepage: https://github.com/skramm/cpphtmltags
*/

#ifndef HG_CPPHTMLTAGS_HPP
#define HG_CPPHTMLTAGS_HPP


//#include <type_traits> // for static_assert

#include <sstream>
#include <map>
#include <cassert>
#include <iostream>


#define HTTAGS_WARNING std::cerr << "htags: Warning: "
#define HTTAGS_ERROR   std::cerr << "htags: fatal error: "

namespace htags {

//-----------------------------------------------------------------------------------
/// HTML tag identifier
enum EN_HTAG
{
	HT_HEAD,
	HT_BODY,
	HT_SCRIPT,
	HT_LINK,
	HT_TITLE,
	HT_INPUT,
	HT_FORM,
	HT_STYLE,
	HT_TABLE,
	HT_CAPTION,
	HT_SPAN,
	HT_DIV,
	HT_IMG,
	HT_TD,
	HT_TH,
	HT_TR,
	HT_LI,
	HT_UL,
	HT_OL,
	HT_A,
	HT_P,
	HT_H1,
	HT_H2,
	HT_H3,
	HT_H4,
	HT_HR,
	HT_BR,

	HT_DUMMY
};
//-----------------------------------------------------------------------------------
inline
const char*
getTagString( EN_HTAG tag )
{
	const char* n=0;
	switch( tag )
	{
		case HT_HEAD:    n = "head";    break;
		case HT_BODY:    n = "body";    break;
		case HT_SCRIPT:  n = "script";  break;
		case HT_LINK:    n = "link";    break;
		case HT_TITLE:   n = "title";   break;
		case HT_INPUT:   n = "input";   break;
		case HT_FORM:    n = "form";    break;
		case HT_STYLE:   n = "style";   break;
		case HT_TABLE:   n = "table";   break;
		case HT_CAPTION: n = "caption"; break;
		case HT_SPAN:    n = "span";    break;
		case HT_DIV: n =  "div"; break;
		case HT_IMG: n =  "img"; break;
		case HT_TD:  n =  "td";  break;
		case HT_TR:  n =  "tr";  break;
		case HT_TH:  n =  "th";  break;
		case HT_LI:  n =  "li";  break;
		case HT_UL:  n =  "ul";  break;
		case HT_OL:  n =  "ol";  break;
		case HT_A:   n =  "a";   break;
		case HT_P:   n =  "p";   break;
		case HT_H4:  n =  "h4";  break;
		case HT_H3:  n =  "h3";  break;
		case HT_H2:  n =  "h2";  break;
		case HT_H1:  n =  "h1";  break;
		case HT_HR:  n =  "hr";  break;
		case HT_BR:  n =  "br";  break;

		default: assert(0);
	}
	return n;
}

//-----------------------------------------------------------------------------------
/// HTML tag attribute identifier
/**
Only a subset here.

Full list: https://developer.mozilla.org/en-US/docs/Web/HTML/Attributes
*/
enum EN_ATTRIB
{
	AT_STYLE,
	AT_COLSPAN,
	AT_ROWSPAN,

	AT_ACTION,
	AT_METHOD,
	AT_MEDIA,
//	AT_NAME,   // deprecated in html5
	AT_CONTENT,
	AT_VALUE,
	AT_REL,
	AT_TYPE,

	AT_CLASS,
	AT_HREF,
	AT_ONCLICK,
	AT_TITLE,
	AT_ID,
	AT_DOWNLOAD,
	AT_START,

	AT_DUMMY      ///< only used as a default value
};
//-----------------------------------------------------------------------------------
inline
const char*
getAttribString( EN_ATTRIB att )
{
	const char* n=0;
	switch( att )
	{
		case AT_STYLE:    n = "style";    break;
		case AT_COLSPAN:  n = "colspan";  break;
		case AT_ROWSPAN:  n = "rowspan";  break;

		case AT_ACTION:   n = "action";   break;
		case AT_MEDIA:    n = "media";    break;
		case AT_METHOD:   n = "method";   break;
//		case AT_NAME:     n = "name";     break;  // deprecated in html5
		case AT_CONTENT:  n = "content";  break;
		case AT_VALUE:    n = "value";    break;
		case AT_REL:      n = "rel";      break;
		case AT_TYPE:     n = "type";     break;

		case AT_CLASS:    n = "class";    break;
		case AT_HREF:     n = "href";     break;
		case AT_ONCLICK:  n = "onclick";  break;
		case AT_TITLE:    n = "title";    break;
		case AT_ID:       n = "id";       break;
		case AT_DOWNLOAD: n = "download"; break;
		case AT_START:    n = "start";    break;

		default: assert(0);
	}
	return n;
}
//-----------------------------------------------------------------------------------
/// Returns true if the tag must be closed (i.e. is not a void-elements)
/**
Example: \c <br>: no need to close, vs \c <p>, that need to be closed)
For a complete list, see
https://www.w3.org/TR/2011/WD-html-markup-20110113/Overview.html#syntax-elements-toc
*/
inline
bool
tagMustClose( EN_HTAG tag )
{
	switch( tag )
	{
		case HT_INPUT:
		case HT_IMG:
		case HT_HR:
		case HT_BR:
			return false;
		default:
			return true;
	}
}

/// Line Feed Mode (see manual)
enum En_LineFeedMode
{
	LF_None, LF_Always, LF_Default
};

typedef std::map<EN_HTAG, std::pair<EN_ATTRIB,std::string>> GlobAttribMap_t;

//-----------------------------------------------------------------------------------
/// HTML tag
/**
This class can be used in two ways:
- either as a regular object, for example:
\code
	HTAG tag( f, HT_LI );
	tag.openTag();
	f << ... some content
	tag.Closetag();
\endcode
This form is identified by having the output file (of type FileHTML) as first argument of the constructor

- either as an object build in-place that gets streamed, for example, this:
\code
	f << HTAG( HT_LI, "content" );
\endcode
will produce:
\code
	<li>content</li>
\endcode

This form is identified by having as first argument of constructor a value of type EN_HTAG

These two constructors both allow as third and fourth argument an HTML attribute type and value. For example, this:
\code
	f << HTAG( HT_LI, "content", AT_CLASS, "abc" );
\endcode
will produce:
\code
	<li class="abc">content</li>
\endcode

*/
class HTAG
{
	template<typename T>
	friend HTAG&         operator << ( HTAG&         tag,    const T& );
	friend std::ostream& operator << ( std::ostream& stream, const HTAG& );

	public:
/// \name Constructors & destructor
///{
		HTAG( std::ostream& f, EN_HTAG );

		template<typename T>
		HTAG( std::ostream& f, EN_HTAG, EN_ATTRIB, const T& attribvalue );

		HTAG( EN_HTAG );

		template<typename T1>
		HTAG( EN_HTAG, T1 content );
		template<typename T1, typename T2>
		HTAG( EN_HTAG, const T1& content, EN_ATTRIB, const T2& attribvalue );
		template<typename T>
		HTAG( EN_HTAG, EN_ATTRIB, const T& attribvalue );
		~HTAG();
///}
		void openTag();
		void closeTag( bool linefeed=false );
		void addAttrib( EN_ATTRIB, std::string );
		void addAttrib( EN_ATTRIB, int );
		void removeAttrib( EN_ATTRIB );
		void PrintAttributes( bool b ) { _printAttribs = b; }
		template<typename T> void PrintWithContent( const T& );

		EN_HTAG getTag() const { return _tag_en; }
/// \name Global attributes handling
///{
		static void setGlobalAttrib( EN_HTAG tag, EN_ATTRIB att, const std::string& value );
		static void clearGlobalAttribs()
		{
			globalAttrib().clear();
		}
///}
		template<typename T> void setContent( T content );
		static void setLineFeedMode( En_LineFeedMode mode )
		{
			lf_mode() = mode;
		}
		static void printSupported( std::ostream& );

	private:
		void doLineFeed( bool linefeed=false ) const;
		bool checkValid( std::string action );
		std::string getAttribs() const;
		static GlobAttribMap_t& globalAttrib()
		{
			static GlobAttribMap_t s_global_attrib;
			return s_global_attrib;
		}
		static En_LineFeedMode& lf_mode()
		{
			static En_LineFeedMode _lf_mode = LF_Default;
			return _lf_mode;
		}
	private:
		EN_HTAG         _tag_en;
		std::ostream*   _file;
		bool            _isFileType;
		std::string     _content;
		bool            _printAttribs  = true;
		bool            _tagIsOpen     = false;
		std::map<EN_ATTRIB,std::string> _attr_map;
};

//-----------------------------------------------------------------------------------
/// helper function, prints the tags and attributes currently supported
inline
void
HTAG::printSupported( std::ostream& f)
{
	f << "* Supported tags: " << HT_DUMMY;
	for( size_t i=0; i<HT_DUMMY; i++ )
		f <<  "\n - " << getTagString( static_cast<EN_HTAG>(i) );
	f << '\n';

	f << "* Supported attributes: " << AT_DUMMY;
	for( size_t i=0; i<AT_DUMMY; i++ )
		f <<  "\n - " << getAttribString( static_cast<EN_ATTRIB>(i) );
	f << '\n';
}

//-----------------------------------------------------------------------------------
/// constructor 1
inline
HTAG::HTAG( EN_HTAG tag )
	: _tag_en( tag ), _file(0), _isFileType(false)
{
}

//-----------------------------------------------------------------------------------
/// generic constructor 2 (used for string data type)
template<typename T>
HTAG::HTAG( EN_HTAG tag, T content ) : HTAG( tag )
{
	_content = std::to_string( content );
}

/// constructor 2: specialisation for string
template<>
inline
HTAG::HTAG( EN_HTAG tag, std::string content ) : HTAG( tag )
{
	_content = content;
}

/// constructor 2: specialisation for string
template<>
inline
HTAG::HTAG( EN_HTAG tag, const char* content ) : HTAG( tag )
{
	_content = content;
}
//-----------------------------------------------------------------------------------
/// generic constructor 3a
template<typename T1, typename T2>
HTAG::HTAG(
	EN_HTAG        tag,
	const T1&      content,
	EN_ATTRIB      att,
	const T2&      attribvalue
	) : HTAG( tag )
{
	setContent( content );

	if( att != AT_DUMMY )
		addAttrib( att, attribvalue );
}
//-----------------------------------------------------------------------------------
/// generic constructor 3b
template<typename T>
HTAG::HTAG(
	EN_HTAG    tag,
	EN_ATTRIB  att,
	const T&   attribvalue
	) : HTAG( tag )
{
	if( att != AT_DUMMY )
		addAttrib( att, attribvalue );
}
//-----------------------------------------------------------------------------------
/// generic constructor 4 (for file output)
inline
HTAG::HTAG(
	std::ostream& f,            ///< the file into where it will be written
	EN_HTAG       tag )         ///< the html tag id
	: _tag_en( tag ), _file(&f), _isFileType(true)
{
}

//-----------------------------------------------------------------------------------
/// generic constructor 5 (for file output)
template<typename T>
HTAG::HTAG(
	std::ostream&  f,            ///< the file into where it will be written
	EN_HTAG        tag,          ///< the html tag id
	EN_ATTRIB att,          ///< (opt.) the tag's attribute id
	const T&       attribvalue ) ///< (opt.) the attribute value
	: _tag_en( tag ), _file(&f), _isFileType(true)
{
	addAttrib( att, attribvalue );
}

//-----------------------------------------------------------------------------------
/// default implementation
template<typename T>
void
HTAG::setContent( T content )
{
	_content = std::to_string(content);
}

/// specialization for std::string
template<>
void
HTAG::setContent<const std::string&>( const std::string& content )
{
	_content = content;
}

/// specialization for const char*
template<>
void
HTAG::setContent<const char*>( const char* content )
{
	_content = content;
}


//-----------------------------------------------------------------------------------
template<typename T>
void HTAG::PrintWithContent( const T& c )
{
	openTag();
	_file << c;
	_printAttribs = false;
	closeTag();
}

//-----------------------------------------------------------------------------------
/// Destructor, automatically closes tag if needed
inline
HTAG::~HTAG()
{
	if( _tagIsOpen && _isFileType )
		closeTag();
}

//-----------------------------------------------------------------------------------
bool
HTAG::checkValid( std::string action )
{
	if( !_isFileType )
	{
		HTTAGS_ERROR << "tag " << getTagString(_tag_en) << " is not a \"file type\" tag.\n";
		return false;
	}
	if( !_file )
	{
		HTTAGS_ERROR << "tag " << getTagString(_tag_en) << ": asked to " << action << " but file not available.\n";
		return false;
	}
#if 0
	if( !_file->is_open() )
	{
		HTTAGS_ERROR << "tag " << getTagString(_tag_en) << ": asked to " << action << " but file is closed.\n";
		return false;
	}
#endif
	return true;
}
//-----------------------------------------------------------------------------------
inline
void
HTAG::openTag()
{
	assert( checkValid( "open" ) );
	if( _tagIsOpen )
		HTTAGS_WARNING << "tag " << getTagString(_tag_en) << ": asked to open but was already open.\n";
	else
		*_file << '<' << getTagString(_tag_en) << getAttribs() << '>';
	_tagIsOpen = true;
	_printAttribs = false;
}

//-----------------------------------------------------------------------------------
/// Close the tag (this function needs to be called ONLY for "file" object types
inline
void
HTAG::closeTag( bool linefeed )
{
	assert( checkValid( "close" ) );
	if( !_tagIsOpen )
		HTTAGS_WARNING << "tag " << getTagString(_tag_en) << ": asked to close but was already closed.\n";
	else
		*_file << "</" << getTagString(_tag_en) << '>';

	_tagIsOpen = false;
	doLineFeed( linefeed );
}

//-----------------------------------------------------------------------------------
/// Insert some content into the tag, that will get printed later
template<typename T>
HTAG&
operator << ( HTAG& tag, const T& str )
{
	std::ostringstream oss;
	oss << tag._content << str;
	tag._content = oss.str();
	return tag;
}
//-----------------------------------------------------------------------------------
/// static member function
inline
void
HTAG::setGlobalAttrib( EN_HTAG tag, EN_ATTRIB att, const std::string& value )
{
	globalAttrib()[tag] = std::make_pair( att, value );
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag
inline
void
HTAG::addAttrib( EN_ATTRIB attr, std::string value )
{
	assert( !_tagIsOpen ); // because if it is open, then we can't add an attribute !

	if( value.empty() ) // empty string => nothing to add
	{
		std::cerr << "warning: asking to add tag attribute '" << getAttribString(attr) << "' but string is empty\n";
		return;
	}

	if( _attr_map.find(attr) != _attr_map.end() )   // check if element is already present or not
	{
		if( attr != AT_CLASS && attr != AT_STYLE )
			throw std::runtime_error( "Can't add attribute that is not AT_CLASS or AT_STYLE" );//, value );
		else
		{
			std::ostringstream oss;
			oss << _attr_map[attr] << ' ' << value;
			_attr_map[attr] = oss.str();
		}
	}
	else
		_attr_map[attr] = value;
//	std::cout << "_attr_map[attr]=" << _attr_map[attr] << '\n';
}
//-----------------------------------------------------------------------------------
inline
void
HTAG::addAttrib( EN_ATTRIB atr, int value )
{
	assert( !_tagIsOpen ); // because if it is open, then we can't add an attribute !

	_attr_map[atr] = std::to_string(value);
}
//-----------------------------------------------------------------------------------
inline
void
HTAG::removeAttrib( EN_ATTRIB attr )
{
	assert( !_tagIsOpen ); // because if it is open, then we can't remove it!

	if( _attr_map.find(attr) == _attr_map.end() )   // check if element is already present or not
		HTTAGS_WARNING << "asked to remove attribute "
			<< getAttribString( attr )
			<< " to tag " << getTagString( _tag_en )
			<< " but attribute no present.\n";
	else
		_attr_map.at(attr) = std::string();
}
//-----------------------------------------------------------------------------------
/// Returns a string holding the attributes
inline
std::string
HTAG::getAttribs() const
{
	GlobAttribMap_t& gattr = globalAttrib();
	if( gattr.count(_tag_en) )
	{
		const auto& pair1 = gattr.at(_tag_en);
//		out += " " + std::string(getAttribString( p.first )) + "=\"" + p.second + '"';
	}


	std::string out;
	if( _printAttribs && _attr_map.size() )
	{
		for( auto it = _attr_map.begin(); it != _attr_map.end(); ++it )
		{
			std::string tmp = getAttribString( it->first );
			if( it->first != AT_COLSPAN || it->second != "1" )
			{
				out += ' ';
				out += tmp;
				out += "=\"" + it->second + '"';
			}
		}

// check for a global attribute for the current tag
		GlobAttribMap_t& gattr = globalAttrib();
		if( gattr.count(_tag_en) )
		{
			const auto& p = gattr.at(_tag_en);
			out += " " + std::string(getAttribString( p.first )) + "=\"" + p.second + '"';
		}
	}
	return out;
}
//-----------------------------------------------------------------------------------
/// returns true if the default behavior for \c tag is to have a line feed after
bool
hasDefaultLineFeed( EN_HTAG tag )
{
	switch( tag )
	{
		case HT_TITLE:
		case HT_FORM:
		case HT_DIV:
		case HT_SPAN:
		case HT_TABLE:
		case HT_TR:
		case HT_UL:
		case HT_OL:
		case HT_LI:
		case HT_H1:
		case HT_H2:
		case HT_H3:
		case HT_H4:
			return true;
		default: break;
	}
	return false;
}
//-----------------------------------------------------------------------------------
/// Add a linefeed, either if requested (argument), either if default behaviour
inline
void
HTAG::doLineFeed( bool linefeed ) const
{
	bool doIt = false;
	switch( lf_mode() )
	{
		case LF_Always: doIt = true; break;
		case LF_None: break;
		case LF_Default: doIt = hasDefaultLineFeed( _tag_en ) | linefeed; break;
		default: assert(0);
	}

	if( doIt )
		*_file << '\n';
}

//-----------------------------------------------------------------------------------
/// Streams into \c s the opening tag (with attributes), the content, and the closing tag
inline
std::ostream&
operator << ( std::ostream& s, const HTAG& h )
{
	s << '<' << getTagString( h._tag_en )
		<< h.getAttribs()
		<< '>';
	if( tagMustClose( h.getTag() ) )
		s << h._content << "</" << getTagString( h._tag_en ) << '>';
    if( h._isFileType )
		h.doLineFeed();
	return s;
}
//-----------------------------------------------------------------------------------

} // namespace htags end

#endif // HG_HTAG_H

