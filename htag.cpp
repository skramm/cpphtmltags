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

#include "cpphtmltags.hpp"
//#include "htmlfile.h"
#include <iostream>
#include <cassert>

#ifdef DEBUG
	#include "globals.h"
	#include <iostream>
#endif

std::map<EN_HTAG, std::pair<EN_ATTRIB_TYPE,std::string>> HTAG::_global_attrib;

#if 0
//-----------------------------------------------------------------------------------
template<>
void HTAG::SetContent<int>( const int& content )
{
	std::string s;
	TO_STRING_I( content, s );
	_content = s;
}
template<>
void HTAG::SetContent<size_t>( const size_t& content )
{
	std::string s;
	TO_STRING_I( content, s );
	_content = s;
}

template<>
void HTAG::SetContent<float>( const float& content )
{
	std::string s;
	TO_STRING_F( content, s );
	_content = s;
}
#endif
//-----------------------------------------------------------------------------------
void HTAG::GetTagString()
{
	switch( _tag_en )
	{
		case HT_DIV: _tag = "div"; break;
		case HT_TABLE: _tag = "table"; break;
		case HT_TD: _tag = "td"; break;
		case HT_TR: _tag = "tr"; break;
		case HT_TH: _tag = "th"; break;
		case HT_LI: _tag = "li"; break;
		case HT_UL: _tag = "ul"; break;
		case HT_OL: _tag = "ol"; break;
		case HT_A:  _tag = "a";  break;
		case HT_P:  _tag = "p";  break;
		case HT_H4: _tag = "h4"; break;
		case HT_H3: _tag = "h3"; break;
		case HT_H2: _tag = "h2"; break;
		case HT_H1: _tag = "h1"; break;
		case HT_BR: _tag = "br"; break;
		default: assert(0);
	}
}
//-----------------------------------------------------------------------------------
void HTAG::OpenTag()
{
//	DEBUG_IN;

//#ifdef DEBUG
//	CERR << "opening tag " << _tag << "\n";
//#endif

	assert( _file );
	assert( !_tagIsOpen );

	*_file << '<' << _tag << GetAttribs() << '>';
	_tagIsOpen = true;
	_printAttribs = false;

//	DEBUG_OUT;
}

//-----------------------------------------------------------------------------------
/// Close the tag (this function needs to be calle ONLY for "file" object types
void HTAG::CloseTag( bool linefeed )
{
//	DEBUG_IN;

//#ifdef DEBUG
//	CERR << "closing tag " << _tag << "\n";
//#endif

	assert( _isFileType );
	assert( _file );
	assert( _tagIsOpen );

	*_file << "</" << _tag << '>';

	_tagIsOpen = false;
	DoLineFeed( linefeed );

//	DEBUG_OUT;
}

//-----------------------------------------------------------------------------------
const char*
GetAttribString( EN_ATTRIB_TYPE att )
{
	const char* n=0;
	switch( att )
	{
		case AT_STYLE:    n = "style";    break;
		case AT_COLSPAN:  n = "colspan";  break;
		case AT_ROWSPAN:  n = "rowspan";  break;
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
/// returns a string holding the attributes
std::string HTAG::GetAttribs() const
{
	std::string out;
	if( _printAttribs && _attr_map.size() )
	{
		for( auto it = _attr_map.begin(); it != _attr_map.end(); ++it )
		{
			std::string tmp = GetAttribString( it->first );
			if( it->first != AT_COLSPAN || it->second != "1" )
			{
				out += ' ';
				out += tmp;
				out += "=\"" + it->second + '"';
			}
		}

// check for a global attribute for the current tag
		if( _global_attrib.count(_tag_en) )
		{
			const auto& p = _global_attrib.at(_tag_en);
			out += " " + std::string(GetAttribString( p.first )) + "=\"" + p.second + '"';
		}
	}
	return out;
}

//-----------------------------------------------------------------------------------
/// Add a linefeed, either if requested (argument), either if default behaviour
void HTAG::DoLineFeed( bool linefeed )
{
	if( _forceLineFeed || linefeed )
		*_file << '\n';
	else
	{
		switch( _tag_en )    // the tags here have automatically a linefeed appended
		{
			case HT_DIV:
			case HT_TABLE:
			case HT_TR:
			case HT_UL:
			case HT_OL:
			case HT_LI:
			case HT_H2:
			case HT_H3:
			case HT_H4:
				*_file << '\n';
			default: break;
		}
	}
}

//-----------------------------------------------------------------------------------
/// static member function
void HTAG::SetGlobalAttrib( EN_HTAG tag, EN_ATTRIB_TYPE att, const std::string& value )
{
	_global_attrib[tag] = std::make_pair( att, value );
}
//-----------------------------------------------------------------------------------
/// static member function
void HTAG::ClearGlobalAttribs()
{
	_global_attrib.clear();
}

//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag
void HTAG::AddAttrib( EN_ATTRIB_TYPE attr, const std::string& value )
{
	assert( !_tagIsOpen ); // because if it is open, then we can't add an attribute !

	if( value.empty() ) // empty string => nothing to add
	{
		std::cerr << "warning: asking to add tag attribute '"  << GetAttribString( attr) << "' but string is empty\n";
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
void HTAG::AddAttrib( EN_ATTRIB_TYPE atr, int value )
{
	assert( !_tagIsOpen ); // because if it is open, then we can't add an attribute !

/*	std::string s;
	TO_STRING_I( value, s );
	_attr_map[atr] = s; */
	_attr_map[atr] = std::to_string(value);
}

//-----------------------------------------------------------------------------------
void HTAG::RemoveAttrib( EN_ATTRIB_TYPE attr )
{
	assert( !_tagIsOpen ); // because if it is open, then we can't remove it!

	if( _attr_map.find(attr) == _attr_map.end() )   // check if element is already present or not
		throw "attribute not present !";
	else
		_attr_map.at(attr) = std::string();

}
//-----------------------------------------------------------------------------------
/// Streams into \c s the opening tag (with attributes), the content, and the closing tag
std::ostream& operator << ( std::ostream& s, const HTAG& h )
{
	s << '<' << h._tag;
	s << h.GetAttribs();
	s << '>' << h._content << "</" << h._tag << '>';
	return s;
}
//-----------------------------------------------------------------------------------

