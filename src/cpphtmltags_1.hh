//--- START: TEMPLATE FILE cpphtmltags_1.hh
/*
    Copyright (C) 2018-2020 Sebastien Kramm

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
#include <set>
#include <sstream>
#include <map>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#ifndef HTTAG_NO_REFERENCE_TABLES
	#include <functional>
	#include <chrono>
	#include <ctime>
#endif


#define HTTAG_ADD_ERROR_LOCATION( in, out ) \
	{ \
		out = (std::string(in) \
		+ "\n -file: " \
		+ __FILE__ \
		+ "\n -line: " \
		+ std::to_string(__LINE__) \
		+ "\n -func: " \
		+ __FUNCTION__ \
		+ "()\n"); \
	}

#define HTTAG_FATAL_ERROR( msg ) \
	{ \
		std::string out; \
		HTTAG_ADD_ERROR_LOCATION( msg, out ); \
		throw std::runtime_error( out ); \
	}

/// Version with available File and Line location
#define HTTAG_FATAL_ERROR_FL( msg ) \
	{ \
		std::string out; \
		if( __line != 0 ) \
			out = msg + "\n -file: " + __file + "\n -line: " + std::to_string( __line ) + "\n -"; \
		else \
			HTTAG_ADD_ERROR_LOCATION( msg, out ); \
		throw std::runtime_error(out); \
	}


//-----------------------------------------------------------------------------------

/**
\defgroup macroapi Macro API
\brief These macros can be used in place of standard API, see
<a href="md_docs_manual.html#macro"> here</a>
*/


/// Open tag \c t, see \ref manual.md#macro
/** \ingroup macroapi */
#define HTTAG_OPEN( t ) \
	try { \
        t.openTag( std::make_pair(__FILE__, __LINE__) ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (HTTAG_OPEN) " << err.what(); \
	}

/// Close tag \c t
/** \ingroup macroapi */
#define HTTAG_CLOSE( t ) \
	try { \
        t.closeTag( std::make_pair(__FILE__, __LINE__) ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (HTTAG_CLOSE) " << err.what(); \
	}

/// Add attribute
/** \ingroup macroapi */
#define HTTAG_ADDATTRIB( t, at, val ) \
	try { \
		t.addAttrib( at, val, std::make_pair(__FILE__, __LINE__) ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (HTTAG_ADDATTRIB) " << err.what(); \
	}

/// Add content \c val to the tag \c t
/** \ingroup macroapi */
#define HTTAG_ADD( t, val ) \
	try { \
		t.addContent( val, std::make_pair(__FILE__, __LINE__) ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (HTTAG_ADD) " << err.what(); \
	}

/// Set content \c val to the tag \c t (erases previous content)
/** \ingroup macroapi */
#define HTTAG_SET( t, val ) \
	try { \
		t.setContent( val, std::make_pair(__FILE__, __LINE__) ); \
	} \
	catch( const std::runtime_error& err ) \
	{ \
		std::cerr << "\nhttag error: (HTTAG_SET) " << err.what(); \
	}

//############################
namespace httag {
//############################

// forward declaration
class Httag;


//############################
/// Holds all Run Time Options types, see httag::Httag::setOption()
namespace rto {
//############################

//-----------------------------------------------------------------------------------
///@{ Option settings related, see priv::RunTimeOptions and Httag::setOption()
enum IllegalOp_t    { IllegalOp };
enum LFMode_t       { LFMode};
enum ClearOnClose_t { ClearOnClose };
enum ClearOnClose_C_t { ClearContentOnClose };
enum ClearOnClose_A_t { ClearAttribOnClose };
///@}

/// Line Feed Mode. See [manual](md_manual.html#linefeed)
enum En_LineFeedMode
{
	LF_None, LF_Always, LF_Default
};

enum En_ErrorMode { EM_Throw, EM_NoThrow }; //, T_Default };

//############################
} // namespace rto
//############################


//--- END: TEMPLATE FILE cpphtmltags_1.hh

