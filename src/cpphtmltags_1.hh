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

#define HTTAG_PRINT_INFO( msg ) \
		std::cerr << "\nhttag: fatal error: " \
			<< "\n - file: " << __FILE__ \
			<< "\n - line: " << __LINE__ \
			<< "\n - message: " << msg \
			<< "\n";


#ifdef HTTAG_SILENT_MODE
	#define HTTAG_SILENT_WARNINGS
	#define HTTAG_SILENT_ERRORS
#endif

#ifdef HTTAG_SILENT_WARNINGS
	#define HTTAG_WARNING if(0) std::cerr
#else
	#define HTTAG_WARNING if(1) std::cerr << "\nhttag: Warning: "
#endif

#ifdef HTTAG_SILENT_ERRORS
	#define HTTAG_ERROR( msg ) \
		{ \
			throw std::runtime_error( std::string("httag: fatal error: ") + msg ); \
		}
#else
	#define HTTAG_ERROR( msg ) \
		{ \
			HTTAG_PRINT_INFO( msg ); \
			throw std::runtime_error( std::string("httag: fatal error: ") + msg ); \
		}
#endif

/// Open tag \c t
#define HTTAG_OPENTAG( t ) \
	{ \
		try { \
			t.openTag(); \
		} \
		catch( const std::runtime_error& err ) \
		{ \
			std:: cerr << "\nError: cannot open tag <" << getString(t.getTag()) \
				<< ">\n -file: " << __FILE__ \
				<< "\n -line: " << __LINE__ \
				<< "\n -message: " << err.what() << '\n'; \
		} \
	}

/// Close tag \c t
#define HTTAG_CLOSETAG( t ) \
	{ \
		try { \
			t.closeTag(); \
		} \
		catch( const std::runtime_error& err ) \
		{ \
			std:: cerr << "\nError: cannot close tag <" << getString(t.getTag()) \
				<< ">\n -file: " << __FILE__ \
				<< "\n -line: " << __LINE__ \
				<< "\n -message: " << err.what() << '\n'; \
		} \
	}


namespace httag {


//--- END: TEMPLATE FILE cpphtmltags_1.hh