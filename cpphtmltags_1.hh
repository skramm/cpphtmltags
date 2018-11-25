// THIS IS A GENERATED FILE, DO NOT EDIT !
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

\todo add targets to Makefile to build demos (test target is done)

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


#define EXPERIMENTAL

#ifdef EXPERIMENTAL
	#include <vector>
#endif

#include <sstream>
#include <map>
#include <cassert>
#include <iostream>
#include <algorithm>

#define HTAG_PRINT_INFO( msg ) \
		std::cerr << "httag: fatal error: " \
			<< "\n - file: " << __FILE__ \
			<< "\n - line: " << __LINE__ \
			<< "\n - message: " << msg \
			<< "\n";


#ifdef HTAGS_SILENT_MODE
	#define HTAGS_SILENT_WARNINGS
	#define HTAGS_SILENT_ERRORS
#endif

#ifdef HTAGS_SILENT_WARNINGS
	#define HTTAGS_WARNING if(0) std::cerr
#else
	#define HTTAGS_WARNING if(1) std::cerr << "httag: Warning: "
#endif

#ifdef HTAGS_SILENT_ERRORS
	#define HTTAGS_ERROR( msg ) \
		{ \
			throw std::runtime_error( "httag: fatal error" ); \
		}
#else
	#define HTTAGS_ERROR( msg ) \
		{ \
			HTAG_PRINT_INFO( msg ); \
			throw std::runtime_error( "httag: fatal error" ); \
		}
#endif

namespace httag {



