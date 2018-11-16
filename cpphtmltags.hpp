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

// cpphtmltags.hpp

#ifndef HG_CPPHTMLTAGS_HPP
#define HG_CPPHTMLTAGS_HPP


//#include <type_traits> // for static_assert

#include <sstream>
#include <map>


#ifdef DEBUG
	#include <iostream>
#endif


//-----------------------------------------------------------------------------------
/// HTML tag identifier
enum EN_HTAG
{
	HT_DIV,
	HT_TABLE,
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
	HT_BR
};

//-----------------------------------------------------------------------------------
inline
bool
tagMustClose( EN_HTAG tag )
{
	static std::map<EN_HTAG,bool> g_maptagtype = {
	{ HT_DIV, 1 },
	{ HT_TABLE, 1 },
	{ HT_TD,  1 },
	{ HT_TH,  1 },
	{ HT_TR,  1 },
	{ HT_LI,  1 },
	{ HT_UL,  1 },
	{ HT_OL,  1 },
	{ HT_A,   0 },
	{ HT_P,   1 },
	{ HT_H1,  1 },
	{ HT_H2,  1 },
	{ HT_H3,  1 },
	{ HT_H4,  1 },
	{ HT_BR,  0 }
	};
	return g_maptagtype[tag];
}

//-----------------------------------------------------------------------------------
/// HTML tag attribute identifier
enum EN_ATTRIB_TYPE
{
	AT_STYLE,
	AT_COLSPAN,
	AT_ROWSPAN,
	AT_CLASS,
	AT_HREF,
	AT_ONCLICK,
	AT_TITLE,
	AT_ID,
	AT_DOWNLOAD,
	AT_START,

	AT_DUMMY      ///< only used as a default value
};

//enum EN_HTAG_ACTION { HTAG_OPEN, HTAG_CLOSE };


//class FileHTML;

//-----------------------------------------------------------------------------------
/// HTML tag
/**
This class can be used in two ways:
- either as a regular object, for example:
\code
	HTAG tag( f, HT_LI );
	tag.OpenTag();
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
//	friend FileHTML&     operator << ( FileHTML&     stream, const HTAG& );

	public:
/// \name Constructors & destructor
///{
		HTAG( std::ostream& f, EN_HTAG );

		template<typename T>
		HTAG( std::ostream& f, EN_HTAG, EN_ATTRIB_TYPE att, const T& attribvalue );
		HTAG( EN_HTAG );

		template<typename T1>
		HTAG( EN_HTAG, T1 content );
		template<typename T1, typename T2>
		HTAG( EN_HTAG, const T1& content, EN_ATTRIB_TYPE att, const T2& attribvalue );
		~HTAG();
///}
		void OpenTag();
		void CloseTag( bool linefeed=false );
		void AddAttrib( EN_ATTRIB_TYPE, const std::string& );
		void RemoveAttrib( EN_ATTRIB_TYPE );
		void AddAttrib( EN_ATTRIB_TYPE atr, int value );
		void PrintAttributes( bool b ) { _printAttribs = b; }
		void SetLineBreak( bool b )    { _forceLineFeed = b; }
		template<typename T> void PrintWithContent( const T& );

		static void SetGlobalAttrib( EN_HTAG tag, EN_ATTRIB_TYPE att, const std::string& value );
		static void ClearGlobalAttribs();

	private:
		void DoLineFeed( bool linefeed );
		void GetTagString();
		std::string GetAttribs() const;
		template<typename T> void SetContent( const T& content );

	private:
		EN_HTAG     _tag_en;
		std::ostream* _file;
		bool        _isFileType;
		std::string _tag;
		std::string _content;
		std::map<EN_ATTRIB_TYPE,std::string> _attr_map;
		bool _printAttribs  = true;
		bool _forceLineFeed = false;
		bool _tagIsOpen     = false;

		static std::map<EN_HTAG, std::pair<EN_ATTRIB_TYPE,std::string>> _global_attrib;

#ifdef TESTMODE
	KUT_CLASS_DECLARE
#endif
};

//-----------------------------------------------------------------------------------
/// constructor 1
inline
HTAG::HTAG( EN_HTAG tag )
	: _tag_en( tag ), _file(0), _isFileType(false)
{
	GetTagString();
}

//-----------------------------------------------------------------------------------
/// generic constructor 2 (used for string data type)
template<typename T>
HTAG::HTAG( EN_HTAG tag, T content ) : HTAG( tag )
{
	GetTagString();
	_content = std::to_string( content );
}

/// constructor 2: specialisation for string
template<>
inline
HTAG::HTAG( EN_HTAG tag, std::string content ) : HTAG( tag )
{
	GetTagString();
	_content = content;
}

#if 0
//-----------------------------------------------------------------------------------
/// constructor 2: specialisation 1 (int type)
/**
\todo fix this
*/
template<>
inline
HTAG::HTAG( EN_HTAG tag, int content ) : HTAG( tag )
{
	GetTagString();
	std::string s;
//	TO_STRING_I( content, s );
	_content = std::to_string( content );
}

/// constructor 2: specialisation 2 (unsigned int type)
/**
\todo fix this
*/
template<>
inline
HTAG::HTAG( EN_HTAG tag, size_t content ) : HTAG( tag )
{
	GetTagString();
	std::string s;
//	TO_STRING_I( content, s );
	_content = s;
}

/// constructor 2: specialisation 3 (float type)
/**
\todo fix this
*/
template<>
inline
HTAG::HTAG( EN_HTAG tag, float content ) : HTAG( tag )
{
	GetTagString();
	std::string s;
//	TO_STRING_F( content, s );
	_content = s;
}

/// constructor 2: specialisation 4 (double type)
template<>
inline
HTAG::HTAG( EN_HTAG tag, double content ) : HTAG( tag )
{
	GetTagString();
	std::string s;
	TO_STRING_F( content, s );
	_content = s;
}
#endif

//-----------------------------------------------------------------------------------
/// generic constructor 3
template<typename T1, typename T2>
HTAG::HTAG(
	EN_HTAG        tag,
	const T1&      content,
	EN_ATTRIB_TYPE att,
	const T2&      attribvalue )
	 : HTAG( tag )
{
	GetTagString();
	SetContent( content );

	if( att != AT_DUMMY )
		AddAttrib( att, attribvalue );
}
//-----------------------------------------------------------------------------------
/// default implementation
template<typename T>
void HTAG::SetContent( const T& content )
{
	_content = content;
}

template<> void HTAG::SetContent<int>(    const int&    content );
template<> void HTAG::SetContent<size_t>( const size_t& content );
template<> void HTAG::SetContent<float>(  const float&  content );

//-----------------------------------------------------------------------------------
/// generic constructor 4 (for file output)
inline
HTAG::HTAG(
	std::ostream& f,            ///< the file into where it will be written
	EN_HTAG       tag )         ///< the html tag id
	: _tag_en( tag ), _file(&f), _isFileType(true)
{
	GetTagString();
}

//-----------------------------------------------------------------------------------
/// generic constructor 5 (for file output)
template<typename T>
HTAG::HTAG(
	std::ostream&  f,            ///< the file into where it will be written
	EN_HTAG        tag,          ///< the html tag id
	EN_ATTRIB_TYPE att,          ///< (opt.) the tag's attribute id
	const T&       attribvalue ) ///< (opt.) the attribute value
	: _tag_en( tag ), _file(&f), _isFileType(true)
{
	GetTagString();
	AddAttrib( att, attribvalue );
}

//-----------------------------------------------------------------------------------
template<typename T>
void HTAG::PrintWithContent( const T& c )
{
	OpenTag();
	_file << c;
	_printAttribs = false;
	CloseTag();
}

//-----------------------------------------------------------------------------------
/// Destructor, automatically closes tag if needed
inline
HTAG::~HTAG()
{
//#ifdef DEBUG
//	std::cerr << "destroying tag " << _tag << "\n";
//#endif
	if( _tagIsOpen && _isFileType )
		CloseTag();
}
//-----------------------------------------------------------------------------------
/// insert some content into the tag, that will get printed later
template<typename T>
HTAG& operator << ( HTAG& tag, const T& str )
{
	std::ostringstream oss;
	oss << tag._content << str;
	tag._content = oss.str();
	return tag;
}

//-----------------------------------------------------------------------------------

#endif // HG_HTAG_H

