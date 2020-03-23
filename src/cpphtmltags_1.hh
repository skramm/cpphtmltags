//--- START: TEMPLATE FILE cpphtmltags_1.hh
/*
    Copyright (C) 2018 Sebastien Kramm

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License v3 as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License v3
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
\file cpphtmltags.hpp
\brief a C++11 header-only library for generating html5 valid code from your app

homepage: https://github.com/skramm/cpphtmltags

\todo Clarify usage/need of _printAttribs


Refs:
- attributes:
  - https://www.w3.org/wiki/Html/Attributes/_Global
  - https://www.w3schools.com/tags/ref_attributes.asp
  - https://developer.mozilla.org/en-US/docs/Web/HTML/Attributes

- https://www.w3.org/TR/2011/WD-html-markup-20110113/syntax.html#syntax-elements

- Global attributes:
  - http://w3c.github.io/html-reference/global-attributes.html
  - https://www.w3schools.com/tags/ref_standardattributes.asp
*/

#ifndef HG_CPPHTMLTAGS_HPP
#define HG_CPPHTMLTAGS_HPP

#include <vector>
#include <sstream>
#include <map>
#include <cassert>
#include <iostream>
#include <algorithm>

/*#define HTTAG_PRINT_ERROR_LOCATION( msg, f, l )\
		std::cerr << "\nhttag: error: " \
			<< "\n - file: " << f \
			<< "\n - line: " << l \
			<< "\n - message: " << (msg) \
			<< "\n"
*/
#ifdef HTTAG_SILENT_MODE
	#define HTTAG_SILENT_WARNINGS
	#define HTTAG_SILENT_ERRORS
#endif

//#ifdef HTTAG_SILENT_WARNINGS
//	#define HTTAG_WARNING if(0) std::cerr
//#else

	#define HTTAG_WARNING(msg) \
	{ \
		std::cerr << "\nhttag: Warning: " << (msg) \
			<< "\n - file: " << __FILE__ \
			<< "\n - line: " << __LINE__ \
			<< '\n'; \
	}

/*
//#endif
*/
//#ifdef HTTAG_SILENT_ERRORS
//	#define HTTAG_ERROR( msg ) \
//		{ \
//			throw std::runtime_error( std::string("httag: fatal error: ") + msg ); \
//		}
//#else

#define HTTAG_FATAL_ERROR( msg ) throw std::runtime_error(msg)

/// Version with available File and Line location
#define HTTAG_FATAL_ERROR_FL( msg ) \
	{ \
		std::string err_msg; \
		if( __line != 0 ) \
			err_msg = "\n -file: " + __file + "\n -line: " + std::to_string( __line ) + "\n -"; \
		err_msg += msg; \
		throw std::runtime_error(err_msg); \
	}

/// Open tag \c t
#define HTTAG_OPENTAG( t ) \
	try { \
        t.openTag(  __FILE__, __LINE__ ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (opening tag)" << err.what(); \
	}

/// Close tag \c t
#define HTTAG_CLOSETAG( t ) \
	try { \
        t.closeTag(  __FILE__, __LINE__ ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (closing tag)" << err.what(); \
	}

#define HTTAG_ADD_ATTRIB( t, at, val ) \
	try { \
		t.addAttrib( at, val, __FILE__, __LINE__ ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (adding attribute)" << err.what(); \
	}


namespace httag {


//--- END: TEMPLATE FILE cpphtmltags_1.hh
