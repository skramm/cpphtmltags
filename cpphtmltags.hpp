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

#include <vector>
#include <sstream>
#include <map>
#include <cassert>
#include <iostream>
#include <algorithm>

#define HTTAG_PRINT_INFO( msg ) \
		std::cerr << "httag: fatal error: " \
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
	#define HTTAG_WARNING if(1) std::cerr << "httag: Warning: "
#endif

#ifdef HTTAG_SILENT_ERRORS
	#define HTTAG_ERROR( msg ) \
		{ \
			throw std::runtime_error( "httag: fatal error" ); \
		}
#else
	#define HTTAG_ERROR( msg ) \
		{ \
			HTTAG_PRINT_INFO( msg ); \
			throw std::runtime_error( "httag: fatal error" ); \
		}
#endif

namespace httag {



// -------- GENERATED CODE ! --------
// timestamp: 20181127-0837
enum En_Httag
{
	HT_A,
	HT_ABBR,
	HT_ACRONYM,
	HT_ADDRESS,
	HT_APPLET,
	HT_AREA,
	HT_ARTICLE,
	HT_ASIDE,
	HT_AUDIO,
	HT_B,
	HT_BASE,
	HT_BASEFONT,
	HT_BDI,
	HT_BDO,
	HT_BIG,
	HT_BLOCKQUOTE,
	HT_BODY,
	HT_BR,
	HT_BUTTON,
	HT_CANVAS,
	HT_CAPTION,
	HT_CENTER,
	HT_CITE,
	HT_CODE,
	HT_COL,
	HT_COLGROUP,
	HT_DATA,
	HT_DATALIST,
	HT_DD,
	HT_DEL,
	HT_DETAILS,
	HT_DFN,
	HT_DIALOG,
	HT_DIR,
	HT_DIV,
	HT_DL,
	HT_DT,
	HT_EM,
	HT_EMBED,
	HT_FIELDSET,
	HT_FIGCAPTION,
	HT_FIGURE,
	HT_FONT,
	HT_FOOTER,
	HT_FORM,
	HT_FRAME,
	HT_FRAMESET,
	HT_H1,
	HT_H2,
	HT_H3,
	HT_H4,
	HT_H5,
	HT_H6,
	HT_HEAD,
	HT_HEADER,
	HT_HR,
	HT_HTML,
	HT_I,
	HT_IFRAME,
	HT_IMG,
	HT_INPUT,
	HT_INS,
	HT_KBD,
	HT_LABEL,
	HT_LEGEND,
	HT_LI,
	HT_LINK,
	HT_MAIN,
	HT_MAP,
	HT_MARK,
	HT_MENU,
	HT_META,
	HT_METER,
	HT_NAV,
	HT_NOFRAMES,
	HT_NOSCRIPT,
	HT_OBJECT,
	HT_OL,
	HT_OPTGROUP,
	HT_OPTION,
	HT_OUTPUT,
	HT_P,
	HT_PARAM,
	HT_PICTURE,
	HT_PRE,
	HT_PROGRESS,
	HT_Q,
	HT_RP,
	HT_RT,
	HT_RUBY,
	HT_S,
	HT_SAMP,
	HT_SCRIPT,
	HT_SECTION,
	HT_SELECT,
	HT_SMALL,
	HT_SOURCE,
	HT_SPAN,
	HT_STRIKE,
	HT_STRONG,
	HT_STYLE,
	HT_SUB,
	HT_SUMMARY,
	HT_SUP,
	HT_SVG,
	HT_TABLE,
	HT_TBODY,
	HT_TD,
	HT_TEMPLATE,
	HT_TEXTAREA,
	HT_TFOOT,
	HT_TH,
	HT_THEAD,
	HT_TIME,
	HT_TITLE,
	HT_TR,
	HT_TRACK,
	HT_TT,
	HT_U,
	HT_UL,
	HT_VAR,
	HT_VIDEO,
	HT_WBR,

	HT_DUMMY
};

// -------- GENERATED CODE ! --------
// timestamp: 20181127-0837
enum En_Attrib
{
	AT_ACCEPT,
	AT_ACCEPT_CHARSET,
	AT_ACCESSKEY,
	AT_ACTION,
	AT_ALT,
	AT_ASYNC,
	AT_AUTOCOMPLETE,
	AT_AUTOFOCUS,
	AT_AUTOPLAY,
	AT_CHARSET,
	AT_CHECKED,
	AT_CITE,
	AT_CLASS,
	AT_COLS,
	AT_COLSPAN,
	AT_CONTENT,
	AT_CONTENTEDITABLE,
	AT_CONTEXTMENU,
	AT_CONTROLS,
	AT_COORDS,
	AT_DATA,
	AT_DATETIME,
	AT_DEFAULT,
	AT_DEFER,
	AT_DIR,
	AT_DIRNAME,
	AT_DISABLED,
	AT_DOWNLOAD,
	AT_DRAGGABLE,
	AT_DROPZONE,
	AT_ENCTYPE,
	AT_FOR,
	AT_FORM,
	AT_FORMACTION,
	AT_HEADERS,
	AT_HEIGHT,
	AT_HIDDEN,
	AT_HIGH,
	AT_HREF,
	AT_HREFLANG,
	AT_HTTP_EQUIV,
	AT_ID,
	AT_ISMAP,
	AT_KIND,
	AT_LABEL,
	AT_LANG,
	AT_LIST,
	AT_LOOP,
	AT_LOW,
	AT_MAX,
	AT_MAXLENGTH,
	AT_MEDIA,
	AT_METHOD,
	AT_MIN,
	AT_MULTIPLE,
	AT_MUTED,
	AT_NAME,
	AT_NOVALIDATE,
	AT_ONABORT,
	AT_ONAFTERPRINT,
	AT_ONBEFOREPRINT,
	AT_ONBEFOREUNLOAD,
	AT_ONCANPLAY,
	AT_ONCANPLAYTHROUGH,
	AT_ONCUECHANGE,
	AT_ONDURATIONCHANGE,
	AT_ONEMPTIED,
	AT_ONENDED,
	AT_ONERROR,
	AT_ONHASHCHANGE,
	AT_ONLOAD,
	AT_ONLOADEDDATA,
	AT_ONLOADEDMETADATA,
	AT_ONLOADSTART,
	AT_ONOFFLINE,
	AT_ONONLINE,
	AT_ONPAGEHIDE,
	AT_ONPAGESHOW,
	AT_ONPAUSE,
	AT_ONPLAY,
	AT_ONPLAYING,
	AT_ONPOPSTATE,
	AT_ONPROGRESS,
	AT_ONRATECHANGE,
	AT_ONRESET,
	AT_ONRESIZE,
	AT_ONSEARCH,
	AT_ONSEEKED,
	AT_ONSEEKING,
	AT_ONSTALLED,
	AT_ONSTORAGE,
	AT_ONSUBMIT,
	AT_ONSUSPEND,
	AT_ONTIMEUPDATE,
	AT_ONTOGGLE,
	AT_ONUNLOAD,
	AT_ONVOLUMECHANGE,
	AT_ONWAITING,
	AT_OPEN,
	AT_OPTIMUM,
	AT_PATTERN,
	AT_PLACEHOLDER,
	AT_POSTER,
	AT_PRELOAD,
	AT_READONLY,
	AT_REL,
	AT_REQUIRED,
	AT_REVERSED,
	AT_ROWS,
	AT_ROWSPAN,
	AT_SANDBOX,
	AT_SCOPE,
	AT_SELECTED,
	AT_SHAPE,
	AT_SIZE,
	AT_SIZES,
	AT_SPAN,
	AT_SPELLCHECK,
	AT_SRC,
	AT_SRCDOC,
	AT_SRCLANG,
	AT_SRCSET,
	AT_START,
	AT_STEP,
	AT_STYLE,
	AT_TABINDEX,
	AT_TARGET,
	AT_TITLE,
	AT_TRANSLATE,
	AT_TYPE,
	AT_USEMAP,
	AT_VALUE,
	AT_WIDTH,
	AT_WRAP,

	AT_DUMMY
};

// -------- GENERATED CODE ! --------
// timestamp: 20181127-0837
const char*
getString( En_Httag a )
{
	const char* n=0;
	switch( a )
	{
		case HT_A: n = "a"; break;
		case HT_ABBR: n = "abbr"; break;
		case HT_ACRONYM: n = "acronym"; break;
		case HT_ADDRESS: n = "address"; break;
		case HT_APPLET: n = "applet"; break;
		case HT_AREA: n = "area"; break;
		case HT_ARTICLE: n = "article"; break;
		case HT_ASIDE: n = "aside"; break;
		case HT_AUDIO: n = "audio"; break;
		case HT_B: n = "b"; break;
		case HT_BASE: n = "base"; break;
		case HT_BASEFONT: n = "basefont"; break;
		case HT_BDI: n = "bdi"; break;
		case HT_BDO: n = "bdo"; break;
		case HT_BIG: n = "big"; break;
		case HT_BLOCKQUOTE: n = "blockquote"; break;
		case HT_BODY: n = "body"; break;
		case HT_BR: n = "br"; break;
		case HT_BUTTON: n = "button"; break;
		case HT_CANVAS: n = "canvas"; break;
		case HT_CAPTION: n = "caption"; break;
		case HT_CENTER: n = "center"; break;
		case HT_CITE: n = "cite"; break;
		case HT_CODE: n = "code"; break;
		case HT_COL: n = "col"; break;
		case HT_COLGROUP: n = "colgroup"; break;
		case HT_DATA: n = "data"; break;
		case HT_DATALIST: n = "datalist"; break;
		case HT_DD: n = "dd"; break;
		case HT_DEL: n = "del"; break;
		case HT_DETAILS: n = "details"; break;
		case HT_DFN: n = "dfn"; break;
		case HT_DIALOG: n = "dialog"; break;
		case HT_DIR: n = "dir"; break;
		case HT_DIV: n = "div"; break;
		case HT_DL: n = "dl"; break;
		case HT_DT: n = "dt"; break;
		case HT_EM: n = "em"; break;
		case HT_EMBED: n = "embed"; break;
		case HT_FIELDSET: n = "fieldset"; break;
		case HT_FIGCAPTION: n = "figcaption"; break;
		case HT_FIGURE: n = "figure"; break;
		case HT_FONT: n = "font"; break;
		case HT_FOOTER: n = "footer"; break;
		case HT_FORM: n = "form"; break;
		case HT_FRAME: n = "frame"; break;
		case HT_FRAMESET: n = "frameset"; break;
		case HT_H1: n = "h1"; break;
		case HT_H2: n = "h2"; break;
		case HT_H3: n = "h3"; break;
		case HT_H4: n = "h4"; break;
		case HT_H5: n = "h5"; break;
		case HT_H6: n = "h6"; break;
		case HT_HEAD: n = "head"; break;
		case HT_HEADER: n = "header"; break;
		case HT_HR: n = "hr"; break;
		case HT_HTML: n = "html"; break;
		case HT_I: n = "i"; break;
		case HT_IFRAME: n = "iframe"; break;
		case HT_IMG: n = "img"; break;
		case HT_INPUT: n = "input"; break;
		case HT_INS: n = "ins"; break;
		case HT_KBD: n = "kbd"; break;
		case HT_LABEL: n = "label"; break;
		case HT_LEGEND: n = "legend"; break;
		case HT_LI: n = "li"; break;
		case HT_LINK: n = "link"; break;
		case HT_MAIN: n = "main"; break;
		case HT_MAP: n = "map"; break;
		case HT_MARK: n = "mark"; break;
		case HT_MENU: n = "menu"; break;
		case HT_META: n = "meta"; break;
		case HT_METER: n = "meter"; break;
		case HT_NAV: n = "nav"; break;
		case HT_NOFRAMES: n = "noframes"; break;
		case HT_NOSCRIPT: n = "noscript"; break;
		case HT_OBJECT: n = "object"; break;
		case HT_OL: n = "ol"; break;
		case HT_OPTGROUP: n = "optgroup"; break;
		case HT_OPTION: n = "option"; break;
		case HT_OUTPUT: n = "output"; break;
		case HT_P: n = "p"; break;
		case HT_PARAM: n = "param"; break;
		case HT_PICTURE: n = "picture"; break;
		case HT_PRE: n = "pre"; break;
		case HT_PROGRESS: n = "progress"; break;
		case HT_Q: n = "q"; break;
		case HT_RP: n = "rp"; break;
		case HT_RT: n = "rt"; break;
		case HT_RUBY: n = "ruby"; break;
		case HT_S: n = "s"; break;
		case HT_SAMP: n = "samp"; break;
		case HT_SCRIPT: n = "script"; break;
		case HT_SECTION: n = "section"; break;
		case HT_SELECT: n = "select"; break;
		case HT_SMALL: n = "small"; break;
		case HT_SOURCE: n = "source"; break;
		case HT_SPAN: n = "span"; break;
		case HT_STRIKE: n = "strike"; break;
		case HT_STRONG: n = "strong"; break;
		case HT_STYLE: n = "style"; break;
		case HT_SUB: n = "sub"; break;
		case HT_SUMMARY: n = "summary"; break;
		case HT_SUP: n = "sup"; break;
		case HT_SVG: n = "svg"; break;
		case HT_TABLE: n = "table"; break;
		case HT_TBODY: n = "tbody"; break;
		case HT_TD: n = "td"; break;
		case HT_TEMPLATE: n = "template"; break;
		case HT_TEXTAREA: n = "textarea"; break;
		case HT_TFOOT: n = "tfoot"; break;
		case HT_TH: n = "th"; break;
		case HT_THEAD: n = "thead"; break;
		case HT_TIME: n = "time"; break;
		case HT_TITLE: n = "title"; break;
		case HT_TR: n = "tr"; break;
		case HT_TRACK: n = "track"; break;
		case HT_TT: n = "tt"; break;
		case HT_U: n = "u"; break;
		case HT_UL: n = "ul"; break;
		case HT_VAR: n = "var"; break;
		case HT_VIDEO: n = "video"; break;
		case HT_WBR: n = "wbr"; break;

		default: assert(0);
	}
	return n;
}

// -------- GENERATED CODE ! --------
// timestamp: 20181127-0837
const char*
getString( En_Attrib a )
{
	const char* n=0;
	switch( a )
	{
		case AT_ACCEPT: n = "accept"; break;
		case AT_ACCEPT_CHARSET: n = "accept_charset"; break;
		case AT_ACCESSKEY: n = "accesskey"; break;
		case AT_ACTION: n = "action"; break;
		case AT_ALT: n = "alt"; break;
		case AT_ASYNC: n = "async"; break;
		case AT_AUTOCOMPLETE: n = "autocomplete"; break;
		case AT_AUTOFOCUS: n = "autofocus"; break;
		case AT_AUTOPLAY: n = "autoplay"; break;
		case AT_CHARSET: n = "charset"; break;
		case AT_CHECKED: n = "checked"; break;
		case AT_CITE: n = "cite"; break;
		case AT_CLASS: n = "class"; break;
		case AT_COLS: n = "cols"; break;
		case AT_COLSPAN: n = "colspan"; break;
		case AT_CONTENT: n = "content"; break;
		case AT_CONTENTEDITABLE: n = "contenteditable"; break;
		case AT_CONTEXTMENU: n = "contextmenu"; break;
		case AT_CONTROLS: n = "controls"; break;
		case AT_COORDS: n = "coords"; break;
		case AT_DATA: n = "data"; break;
		case AT_DATETIME: n = "datetime"; break;
		case AT_DEFAULT: n = "default"; break;
		case AT_DEFER: n = "defer"; break;
		case AT_DIR: n = "dir"; break;
		case AT_DIRNAME: n = "dirname"; break;
		case AT_DISABLED: n = "disabled"; break;
		case AT_DOWNLOAD: n = "download"; break;
		case AT_DRAGGABLE: n = "draggable"; break;
		case AT_DROPZONE: n = "dropzone"; break;
		case AT_ENCTYPE: n = "enctype"; break;
		case AT_FOR: n = "for"; break;
		case AT_FORM: n = "form"; break;
		case AT_FORMACTION: n = "formaction"; break;
		case AT_HEADERS: n = "headers"; break;
		case AT_HEIGHT: n = "height"; break;
		case AT_HIDDEN: n = "hidden"; break;
		case AT_HIGH: n = "high"; break;
		case AT_HREF: n = "href"; break;
		case AT_HREFLANG: n = "hreflang"; break;
		case AT_HTTP_EQUIV: n = "http_equiv"; break;
		case AT_ID: n = "id"; break;
		case AT_ISMAP: n = "ismap"; break;
		case AT_KIND: n = "kind"; break;
		case AT_LABEL: n = "label"; break;
		case AT_LANG: n = "lang"; break;
		case AT_LIST: n = "list"; break;
		case AT_LOOP: n = "loop"; break;
		case AT_LOW: n = "low"; break;
		case AT_MAX: n = "max"; break;
		case AT_MAXLENGTH: n = "maxlength"; break;
		case AT_MEDIA: n = "media"; break;
		case AT_METHOD: n = "method"; break;
		case AT_MIN: n = "min"; break;
		case AT_MULTIPLE: n = "multiple"; break;
		case AT_MUTED: n = "muted"; break;
		case AT_NAME: n = "name"; break;
		case AT_NOVALIDATE: n = "novalidate"; break;
		case AT_ONABORT: n = "onabort"; break;
		case AT_ONAFTERPRINT: n = "onafterprint"; break;
		case AT_ONBEFOREPRINT: n = "onbeforeprint"; break;
		case AT_ONBEFOREUNLOAD: n = "onbeforeunload"; break;
		case AT_ONCANPLAY: n = "oncanplay"; break;
		case AT_ONCANPLAYTHROUGH: n = "oncanplaythrough"; break;
		case AT_ONCUECHANGE: n = "oncuechange"; break;
		case AT_ONDURATIONCHANGE: n = "ondurationchange"; break;
		case AT_ONEMPTIED: n = "onemptied"; break;
		case AT_ONENDED: n = "onended"; break;
		case AT_ONERROR: n = "onerror"; break;
		case AT_ONHASHCHANGE: n = "onhashchange"; break;
		case AT_ONLOAD: n = "onload"; break;
		case AT_ONLOADEDDATA: n = "onloadeddata"; break;
		case AT_ONLOADEDMETADATA: n = "onloadedmetadata"; break;
		case AT_ONLOADSTART: n = "onloadstart"; break;
		case AT_ONOFFLINE: n = "onoffline"; break;
		case AT_ONONLINE: n = "ononline"; break;
		case AT_ONPAGEHIDE: n = "onpagehide"; break;
		case AT_ONPAGESHOW: n = "onpageshow"; break;
		case AT_ONPAUSE: n = "onpause"; break;
		case AT_ONPLAY: n = "onplay"; break;
		case AT_ONPLAYING: n = "onplaying"; break;
		case AT_ONPOPSTATE: n = "onpopstate"; break;
		case AT_ONPROGRESS: n = "onprogress"; break;
		case AT_ONRATECHANGE: n = "onratechange"; break;
		case AT_ONRESET: n = "onreset"; break;
		case AT_ONRESIZE: n = "onresize"; break;
		case AT_ONSEARCH: n = "onsearch"; break;
		case AT_ONSEEKED: n = "onseeked"; break;
		case AT_ONSEEKING: n = "onseeking"; break;
		case AT_ONSTALLED: n = "onstalled"; break;
		case AT_ONSTORAGE: n = "onstorage"; break;
		case AT_ONSUBMIT: n = "onsubmit"; break;
		case AT_ONSUSPEND: n = "onsuspend"; break;
		case AT_ONTIMEUPDATE: n = "ontimeupdate"; break;
		case AT_ONTOGGLE: n = "ontoggle"; break;
		case AT_ONUNLOAD: n = "onunload"; break;
		case AT_ONVOLUMECHANGE: n = "onvolumechange"; break;
		case AT_ONWAITING: n = "onwaiting"; break;
		case AT_OPEN: n = "open"; break;
		case AT_OPTIMUM: n = "optimum"; break;
		case AT_PATTERN: n = "pattern"; break;
		case AT_PLACEHOLDER: n = "placeholder"; break;
		case AT_POSTER: n = "poster"; break;
		case AT_PRELOAD: n = "preload"; break;
		case AT_READONLY: n = "readonly"; break;
		case AT_REL: n = "rel"; break;
		case AT_REQUIRED: n = "required"; break;
		case AT_REVERSED: n = "reversed"; break;
		case AT_ROWS: n = "rows"; break;
		case AT_ROWSPAN: n = "rowspan"; break;
		case AT_SANDBOX: n = "sandbox"; break;
		case AT_SCOPE: n = "scope"; break;
		case AT_SELECTED: n = "selected"; break;
		case AT_SHAPE: n = "shape"; break;
		case AT_SIZE: n = "size"; break;
		case AT_SIZES: n = "sizes"; break;
		case AT_SPAN: n = "span"; break;
		case AT_SPELLCHECK: n = "spellcheck"; break;
		case AT_SRC: n = "src"; break;
		case AT_SRCDOC: n = "srcdoc"; break;
		case AT_SRCLANG: n = "srclang"; break;
		case AT_SRCSET: n = "srcset"; break;
		case AT_START: n = "start"; break;
		case AT_STEP: n = "step"; break;
		case AT_STYLE: n = "style"; break;
		case AT_TABINDEX: n = "tabindex"; break;
		case AT_TARGET: n = "target"; break;
		case AT_TITLE: n = "title"; break;
		case AT_TRANSLATE: n = "translate"; break;
		case AT_TYPE: n = "type"; break;
		case AT_USEMAP: n = "usemap"; break;
		case AT_VALUE: n = "value"; break;
		case AT_WIDTH: n = "width"; break;
		case AT_WRAP: n = "wrap"; break;

		default: assert(0);
	}
	return n;
}

// -------- GENERATED CODE ! --------
// timestamp: 20181127-0837
/// Conveniency typedef
typedef std::map<En_Attrib,std::vector<En_Httag>> MapAttribs_t;
/// Private class, holds map of allowed attributes
struct MapAttribs
{
	MapAttribs_t _map;
	const MapAttribs_t& get() {
		return _map;
	}
	MapAttribs()
	{
		_map[AT_ACCEPT]	=	 { HT_INPUT };
		_map[AT_ACCEPT_CHARSET]	=	 { HT_FORM };
		_map[AT_ACTION]	=	 { HT_FORM };
		_map[AT_ALT]	=	 { HT_AREA, HT_IMG, HT_INPUT };
		_map[AT_ASYNC]	=	 { HT_SCRIPT };
		_map[AT_AUTOCOMPLETE]	=	 { HT_FORM, HT_INPUT };
		_map[AT_AUTOFOCUS]	=	 { HT_BUTTON, HT_INPUT, HT_SELECT, HT_TEXTAREA };
		_map[AT_AUTOPLAY]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_CHARSET]	=	 { HT_META, HT_SCRIPT };
		_map[AT_CHECKED]	=	 { HT_INPUT };
		_map[AT_CITE]	=	 { HT_BLOCKQUOTE, HT_DEL, HT_INS, HT_Q };
		_map[AT_COLS]	=	 { HT_TEXTAREA };
		_map[AT_COLSPAN]	=	 { HT_TD, HT_TH };
		_map[AT_CONTENT]	=	 { HT_META };
		_map[AT_CONTROLS]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_COORDS]	=	 { HT_AREA };
		_map[AT_DATA]	=	 { HT_OBJECT };
		_map[AT_DATETIME]	=	 { HT_DEL, HT_INS, HT_TIME };
		_map[AT_DEFAULT]	=	 { HT_TRACK };
		_map[AT_DEFER]	=	 { HT_SCRIPT };
		_map[AT_DIRNAME]	=	 { HT_INPUT, HT_TEXTAREA };
		_map[AT_DISABLED]	=	 { HT_BUTTON, HT_FIELDSET, HT_INPUT, HT_OPTGROUP, HT_OPTION, HT_SELECT, HT_TEXTAREA };
		_map[AT_DOWNLOAD]	=	 { HT_A, HT_AREA };
		_map[AT_ENCTYPE]	=	 { HT_FORM };
		_map[AT_FOR]	=	 { HT_LABEL, HT_OUTPUT };
		_map[AT_FORM]	=	 { HT_BUTTON, HT_FIELDSET, HT_INPUT, HT_LABEL, HT_METER, HT_OBJECT, HT_OUTPUT, HT_SELECT, HT_TEXTAREA };
		_map[AT_FORMACTION]	=	 { HT_BUTTON, HT_INPUT };
		_map[AT_HEADERS]	=	 { HT_TD, HT_TH };
		_map[AT_HEIGHT]	=	 { HT_CANVAS, HT_EMBED, HT_IFRAME, HT_IMG, HT_INPUT, HT_OBJECT, HT_VIDEO };
		_map[AT_HIGH]	=	 { HT_METER };
		_map[AT_HREF]	=	 { HT_A, HT_AREA, HT_BASE, HT_LINK };
		_map[AT_HREFLANG]	=	 { HT_A, HT_AREA, HT_LINK };
		_map[AT_HTTP_EQUIV]	=	 { HT_META };
		_map[AT_ISMAP]	=	 { HT_IMG };
		_map[AT_KIND]	=	 { HT_TRACK };
		_map[AT_LABEL]	=	 { HT_TRACK, HT_OPTION, HT_OPTGROUP };
		_map[AT_LIST]	=	 { HT_INPUT };
		_map[AT_LOOP]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_LOW]	=	 { HT_METER };
		_map[AT_MAX]	=	 { HT_INPUT, HT_METER, HT_PROGRESS };
		_map[AT_MAXLENGTH]	=	 { HT_INPUT, HT_TEXTAREA };
		_map[AT_MEDIA]	=	 { HT_A, HT_AREA, HT_LINK, HT_SOURCE, HT_STYLE };
		_map[AT_METHOD]	=	 { HT_FORM };
		_map[AT_MIN]	=	 { HT_INPUT, HT_METER };
		_map[AT_MULTIPLE]	=	 { HT_INPUT, HT_SELECT };
		_map[AT_MUTED]	=	 { HT_VIDEO, HT_AUDIO };
		_map[AT_NAME]	=	 { HT_BUTTON, HT_FIELDSET, HT_FORM, HT_IFRAME, HT_INPUT, HT_MAP, HT_META, HT_OBJECT, HT_OUTPUT, HT_PARAM, HT_SELECT, HT_TEXTAREA };
		_map[AT_NOVALIDATE]	=	 { HT_FORM };
		_map[AT_ONABORT]	=	 { HT_AUDIO, HT_EMBED, HT_IMG, HT_OBJECT, HT_VIDEO };
		_map[AT_ONAFTERPRINT]	=	 { HT_BODY };
		_map[AT_ONBEFOREPRINT]	=	 { HT_BODY };
		_map[AT_ONBEFOREUNLOAD]	=	 { HT_BODY };
		_map[AT_ONCANPLAY]	=	 { HT_AUDIO, HT_EMBED, HT_OBJECT, HT_VIDEO };
		_map[AT_ONCANPLAYTHROUGH]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONCUECHANGE]	=	 { HT_TRACK };
		_map[AT_ONDURATIONCHANGE]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONEMPTIED]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONENDED]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONERROR]	=	 { HT_AUDIO, HT_BODY, HT_EMBED, HT_IMG, HT_OBJECT, HT_SCRIPT, HT_STYLE, HT_VIDEO };
		_map[AT_ONHASHCHANGE]	=	 { HT_BODY };
		_map[AT_ONLOAD]	=	 { HT_BODY, HT_IFRAME, HT_IMG, HT_INPUT, HT_LINK, HT_SCRIPT, HT_STYLE };
		_map[AT_ONLOADEDDATA]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONLOADEDMETADATA]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONLOADSTART]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONOFFLINE]	=	 { HT_BODY };
		_map[AT_ONONLINE]	=	 { HT_BODY };
		_map[AT_ONPAGEHIDE]	=	 { HT_BODY };
		_map[AT_ONPAGESHOW]	=	 { HT_BODY };
		_map[AT_ONPAUSE]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONPLAY]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONPLAYING]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONPOPSTATE]	=	 { HT_BODY };
		_map[AT_ONPROGRESS]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONRATECHANGE]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONRESET]	=	 { HT_FORM };
		_map[AT_ONRESIZE]	=	 { HT_BODY };
		_map[AT_ONSEARCH]	=	 { HT_INPUT };
		_map[AT_ONSEEKED]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONSEEKING]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONSTALLED]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONSTORAGE]	=	 { HT_BODY };
		_map[AT_ONSUBMIT]	=	 { HT_FORM };
		_map[AT_ONSUSPEND]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONTIMEUPDATE]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONTOGGLE]	=	 { HT_DETAILS };
		_map[AT_ONUNLOAD]	=	 { HT_BODY };
		_map[AT_ONVOLUMECHANGE]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_ONWAITING]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_OPEN]	=	 { HT_DETAILS };
		_map[AT_OPTIMUM]	=	 { HT_METER };
		_map[AT_PATTERN]	=	 { HT_INPUT };
		_map[AT_PLACEHOLDER]	=	 { HT_INPUT, HT_TEXTAREA };
		_map[AT_POSTER]	=	 { HT_VIDEO };
		_map[AT_PRELOAD]	=	 { HT_AUDIO, HT_VIDEO };
		_map[AT_READONLY]	=	 { HT_INPUT, HT_TEXTAREA };
		_map[AT_REL]	=	 { HT_A, HT_AREA, HT_LINK };
		_map[AT_REQUIRED]	=	 { HT_INPUT, HT_SELECT, HT_TEXTAREA };
		_map[AT_REVERSED]	=	 { HT_OL };
		_map[AT_ROWS]	=	 { HT_TEXTAREA };
		_map[AT_ROWSPAN]	=	 { HT_TD, HT_TH };
		_map[AT_SANDBOX]	=	 { HT_IFRAME };
		_map[AT_SCOPE]	=	 { HT_TH };
		_map[AT_SELECTED]	=	 { HT_OPTION };
		_map[AT_SHAPE]	=	 { HT_AREA };
		_map[AT_SIZE]	=	 { HT_INPUT, HT_SELECT };
		_map[AT_SIZES]	=	 { HT_IMG, HT_LINK, HT_SOURCE };
		_map[AT_SPAN]	=	 { HT_COL, HT_COLGROUP };
		_map[AT_SRC]	=	 { HT_AUDIO, HT_EMBED, HT_IFRAME, HT_IMG, HT_INPUT, HT_SCRIPT, HT_SOURCE, HT_TRACK, HT_VIDEO };
		_map[AT_SRCDOC]	=	 { HT_IFRAME };
		_map[AT_SRCLANG]	=	 { HT_TRACK };
		_map[AT_SRCSET]	=	 { HT_IMG, HT_SOURCE };
		_map[AT_START]	=	 { HT_OL };
		_map[AT_STEP]	=	 { HT_INPUT };
		_map[AT_TARGET]	=	 { HT_A, HT_AREA, HT_BASE, HT_FORM };
		_map[AT_TYPE]	=	 { HT_BUTTON, HT_EMBED, HT_INPUT, HT_LINK, HT_MENU, HT_OBJECT, HT_SCRIPT, HT_SOURCE, HT_STYLE };
		_map[AT_USEMAP]	=	 { HT_IMG, HT_OBJECT };
		_map[AT_VALUE]	=	 { HT_BUTTON, HT_INPUT, HT_LI, HT_OPTION, HT_METER, HT_PROGRESS, HT_PARAM };
		_map[AT_WIDTH]	=	 { HT_CANVAS, HT_EMBED, HT_IFRAME, HT_IMG, HT_INPUT, HT_OBJECT, HT_VIDEO };
		_map[AT_WRAP]	=	 { HT_TEXTAREA };
	}
};



//-----------------------------------------------------------------------------------
/// A global attribute can be used in \b any html tag
/**
for a list, see: https://www.w3schools.com/tags/ref_standardattributes.asp
\todo Is this function needed, really ? Deprecate it ? (anyhow, needs some updating)
*/
inline
bool
isGlobalAttr( En_Attrib attr )
{
	switch( attr )
	{
		case AT_CLASS:
		case AT_STYLE:
		case AT_LANG:
		case AT_TITLE:
		case AT_HIDDEN:
		case AT_ID:
			return true;
		default:
			return false;
	}
	return false;
}

//-----------------------------------------------------------------------------------
/// Returns true if attribute \c attr is allowed for \c tag
/**
Refs:
*/
inline
bool
attribIsAllowed( En_Attrib attr, En_Httag tag )
{
	static MapAttribs mapAllowedAttribs;
	if( mapAllowedAttribs.get().count(attr) ) // if it is there
	{
		const auto& vec = mapAllowedAttribs.get().at(attr);
		auto res = std::find( std::begin( vec ), std::end( vec ), tag );
		if( res == std::end( mapAllowedAttribs.get().at(attr) ) )
			return false;
	}
	return true;
}
//-----------------------------------------------------------------------------------
/// Returns true if the tag must be closed (i.e. is not a void-elements)
/**
Example: \c <br>: no need to close, vs \c <p>, that need to be closed)
*/
inline
bool
isVoidElement( En_Httag tag )
{
	switch( tag )
	{
		case HT_INPUT:
		case HT_IMG:
		case HT_HR:
		case HT_BR:
			return true;
		default:
			return false;
	}
	return false; // to avoid a compiler warning
}

/// Line Feed Mode (see manual)
enum En_LineFeedMode
{
	LF_None, LF_Always, LF_Default
};

typedef std::pair<En_Attrib,std::string>      PairAttribString_t;
typedef std::map<En_Httag, PairAttribString_t> GlobAttribMap_t;

//-----------------------------------------------------------------------------------
/// HTML tag
/**
Main class, see homepage for details
*/
class Httag
{
	friend class MapAttribs;
	template<typename T>
	friend Httag&         operator << ( Httag&         tag,    const T& );
	friend std::ostream& operator << ( std::ostream& stream, const Httag& );

	public:
/// \name Constructors & destructor
///@{
		Httag( std::ostream& f, En_Httag );

		template<typename T>
		Httag( std::ostream& f, En_Httag, T t=T() );

		template<typename T>
		Httag( std::ostream& f, En_Httag, En_Attrib=AT_DUMMY, T attribvalue=T() );

		template<typename T1,typename T2>
		Httag( std::ostream& f, En_Httag, T1 content, En_Attrib=AT_DUMMY, T2 attribvalue=T2() );

		Httag( En_Httag );

		template<typename T1>
		Httag( En_Httag, T1 content );
		template<typename T1, typename T2>
		Httag( En_Httag, T1 content, En_Attrib, T2 attribvalue );
		template<typename T>
		Httag( En_Httag, En_Attrib, T attribvalue );
		~Httag();
///@}
		void openTag();
		void closeTag( bool linefeed=false );
		template<typename T> void addAttrib( En_Attrib, T );
		void removeAttrib( En_Attrib );
//		void PrintAttributes( bool b ) { _printAttribs = b; }

		void printTag();
		template<typename T> void printWithContent( T );

		En_Httag getTag() const { return _tag_en; }
/// \name Global attributes handling
///@{
		static void setGlobalAttrib( En_Httag tag, En_Attrib att, const std::string& value );
		static std::string getGlobalAttrib( En_Httag tag );

		/// Remove the global attribute for \c tag
		/**
			ref: see https://en.cppreference.com/w/cpp/container/map/erase
		*/
		static void clearGlobalAttrib( En_Httag tag )
		{
			globalAttrib().erase( tag );
		}
		/// Remove all global attributes
		static void clearGlobalAttribs()
		{
			globalAttrib().clear();
		}
///@}

/// \name Tag content related functions
///@{
		template<typename T> void addContent( T content );
		template<typename T> void setContent( T content );
		void clearContent() { _content.clear(); }
///@}

		static void setLineFeedMode( En_LineFeedMode mode )
		{
			lf_mode() = mode;
		}
		static void printSupported( std::ostream& );

	private:
		void doLineFeed( bool linefeed=false ) const;
		void p_addAttrib( En_Attrib, std::string );
		void p_checkValidFileType( std::string action );
		std::string p_getAttribs() const;

		static std::vector<En_Httag>& openedTags()
		{
			static std::vector<En_Httag> s_opened_tags;
			return s_opened_tags;
		}

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
		En_Httag         _tag_en;
		std::ostream*   _file;
		bool            _isFileType;
		std::string     _content;
//		bool            _printAttribs  = true;
		bool            _tagIsOpen     = false;
		std::map<En_Attrib,std::string> _attr_map;
};

//-----------------------------------------------------------------------------------
/// helper function, prints the tags and attributes currently supported
inline
void
Httag::printSupported( std::ostream& f )
{
	f << "* Supported tags: " << HT_DUMMY;
	for( size_t i=0; i<HT_DUMMY; i++ )
		f <<  "\n - " << getString( static_cast<En_Httag>(i) );
	f << '\n';

	f << "* Supported attributes: " << AT_DUMMY;
	for( size_t i=0; i<AT_DUMMY; i++ )
		f <<  "\n - " << getString( static_cast<En_Attrib>(i) );
	f << '\n';
}

//-----------------------------------------------------------------------------------
/// constructor 1
inline
Httag::Httag( En_Httag tag )
	: _tag_en( tag ), _file(0), _isFileType(false)
{
}

//-----------------------------------------------------------------------------------
/// generic constructor 2 (used for string data type)
template<typename T>
Httag::Httag( En_Httag tag, T content ) : Httag( tag )
{
	_content = std::to_string( content );
}

/// constructor 2: specialization for string
template<>
inline
Httag::Httag( En_Httag tag, std::string content ) : Httag( tag )
{
	_content = content;
}

/// constructor 2: specialization for string
template<>
inline
Httag::Httag( En_Httag tag, const char* content ) : Httag( tag )
{
	_content = content;
}
//-----------------------------------------------------------------------------------
/// generic constructor 3a
template<typename T1, typename T2>
Httag::Httag(
	En_Httag    tag,
	T1         content,
	En_Attrib  attr,
	T2         attribvalue
	) : Httag( tag )
{
	setContent( content );

	if( attr != AT_DUMMY )
		_attr_map[attr] = attribvalue;
//		addAttrib( att, attribvalue );
}
//-----------------------------------------------------------------------------------
/// generic constructor 3b
template<typename T>
Httag::Httag(
	En_Httag    tag,
	En_Attrib  attr,
	T          attribvalue
	) : Httag( tag )
{
	if( attr != AT_DUMMY )
		_attr_map[attr] = attribvalue;
//		addAttrib( att, attribvalue );
}
//-----------------------------------------------------------------------------------
#if 1
/// generic constructor B1 (for file output)
/**
\todo: why is this constructor necessary ? The other constructor with default args should be sufficient
*/
inline
Httag::Httag(
	std::ostream& f,           ///< the file into where it will be written
	En_Httag       tag          ///< the html tag id
	) : _tag_en( tag ), _file(&f), _isFileType(true)
{}
#endif
//-----------------------------------------------------------------------------------
/// generic constructor B2 (for file output)
template<typename T>
Httag::Httag(
	std::ostream& f,           ///< the file into where it will be written
	En_Httag       tag,          ///< the html tag id
	T             content
	) : _tag_en( tag ), _file(&f), _isFileType(true)
{
	setContent( content );
}

//-----------------------------------------------------------------------------------
/// generic constructor B3 (for file output)
template<typename T>
Httag::Httag(
	std::ostream&  f,            ///< the file into where it will be written
	En_Httag        tag,          ///< the html tag id
	En_Attrib      att,          ///< (opt.) the tag's attribute id
	T              attribvalue   ///< (opt.) the attribute value
	) : _tag_en( tag ), _file(&f), _isFileType(true)
{
	if( att != AT_DUMMY )
		addAttrib( att, attribvalue );
}
//-----------------------------------------------------------------------------------
/// generic constructor B4 (for file output)
template<typename T1, typename T2>
Httag::Httag(
	std::ostream&  f,            ///< the file into where it will be written
	En_Httag        tag,          ///< the html tag id
	T1             content,      ///< tag content
	En_Attrib      att,          ///< (opt.) the tag's attribute id
	T2             attribvalue   ///< (opt.) the attribute value
	) : _tag_en( tag ), _file(&f), _isFileType(true)
{
	if( att != AT_DUMMY )
		addAttrib( att, attribvalue );
	setContent( content );
}
//-----------------------------------------------------------------------------------
/// specialization for std::string
template<>
void
Httag::addContent<std::string>( std::string content )
{
	if( isVoidElement( _tag_en ) )
		HTTAG_ERROR( std::string("attempting to store content '") + content + "' into a void-element tag '" + getString( _tag_en ) + '\'' );
	_content += content;
}
#if 1
/// specialization for const char*
template<>
void
Httag::addContent<const char*>( const char* content )
{
	addContent<std::string>( std::string(content) );
}
#endif
/// default implementation
template<typename T>
void
Httag::addContent( T content )
{
	addContent<std::string>( std::to_string(content) );
}
//-----------------------------------------------------------------------------------
/// specialization for const char*
template<>
void
Httag::setContent<const char*>( const char* content )
{
	clearContent();
	addContent<std::string>( std::string(content) );
}
/// specialization for std::string
template<>
void
Httag::setContent<std::string>( std::string content )
{
	clearContent();
	addContent<std::string>( content );
}
/// default implementation
template<typename T>
void
Httag::setContent( T content )
{
	clearContent();
	addContent<std::string>( std::to_string(content) );
}
//-----------------------------------------------------------------------------------
void Httag::printTag()
{
	printWithContent( "" );
}
//-----------------------------------------------------------------------------------
template<typename T>
void Httag::printWithContent( T c )
{
	openTag();
	if( !_content.empty() )
		*_file << _content;
	*_file << c;
//	_printAttribs = false;
	closeTag();
}

//-----------------------------------------------------------------------------------
/// Destructor, automatically closes tag if needed
inline
Httag::~Httag()
{
	if( _tagIsOpen && _isFileType )
		closeTag();
}

//-----------------------------------------------------------------------------------
void
Httag::p_checkValidFileType( std::string action )
{
	if( !_isFileType )
		HTTAG_ERROR( std::string("object tag '") + getString(_tag_en) + "' is not a \"file type\" object." );

	if( !_file )
		HTTAG_ERROR( std::string("object tag '") + getString(_tag_en) + "': asked to " + action + " but file not available" );

#if 0
	if( !_file->is_open() )
		HTTAG_ERROR << "tag '" << getString(_tag_en) << "': asked to '" << action << "' but file is closed.\n";
#endif
}
//-----------------------------------------------------------------------------------
/// Open the tag (this function needs to be called ONLY for "file" object types
inline
void
Httag::openTag()
{
	p_checkValidFileType( "open" );
	if( _tagIsOpen )
	{
		HTTAG_WARNING << "tag '" << getString(_tag_en) << "': asked to open but was already open.\n";
	}
	else
		*_file << '<' << getString(_tag_en) << p_getAttribs() << '>';
	_tagIsOpen = true;
//	_printAttribs = false;
	openedTags().push_back( _tag_en );
}
//-----------------------------------------------------------------------------------
/// Close the tag (this function needs to be called ONLY for "file" object types
inline
void
Httag::closeTag( bool linefeed )
{
	p_checkValidFileType( "close" );

	if( !_tagIsOpen )
		HTTAG_ERROR( std::string( "tag '" ) + getString(_tag_en) + "': asked to close but was already closed." );
	*_file << "</" << getString(_tag_en) << '>';

	assert( openedTags().size() > 0 );
	if( openedTags().back() != _tag_en )
		HTTAG_ERROR( std::string( "asking to close tag '") + getString(_tag_en) + "' but tag '" +  getString(openedTags().back()) + "' still open" );

	openedTags().pop_back();

	_tagIsOpen = false;
	doLineFeed( linefeed );
}
//-----------------------------------------------------------------------------------
/// Insert some content into the tag, that will get printed later
template<typename T>
Httag&
operator << ( Httag& tag, const T& str )
{
//	assert( !tag._isFileType );
	std::ostringstream oss;
	oss << tag._content << str;
	tag._content = oss.str();
	return tag;
}
//-----------------------------------------------------------------------------------
/// static member function
inline
void
Httag::setGlobalAttrib( En_Httag tag, En_Attrib att, const std::string& value )
{
	globalAttrib()[tag] = std::make_pair( att, value );
}

//-----------------------------------------------------------------------------------
/// Returns the global pair "attribute=string" for \c tag, if any
inline
std::string
Httag::getGlobalAttrib( En_Httag tag )
{
	if( globalAttrib().count(tag) )
		return std::string( getString( globalAttrib()[tag].first )) + '=' + globalAttrib()[tag].second;
	return std::string();
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag (templated generic version)
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
template<typename T>
void
Httag::addAttrib( En_Attrib attr, T value )
{
	p_addAttrib( attr, std::to_string(value) );
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag (specialized templated version for \c std::string)
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
template<>
void
Httag::addAttrib<std::string>( En_Attrib attr, std::string value )
{
	p_addAttrib( attr, value );
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag (specialized templated version for <tt>const char*</tt>)
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
template<>
void
Httag::addAttrib<const char*>( En_Attrib attr, const char* value )
{
	p_addAttrib( attr, value );
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
inline
void
Httag::p_addAttrib( En_Attrib attr, std::string value )
{
	if( _tagIsOpen ) // because if it is already opened, then we can't add an attribute !
		HTTAG_ERROR( std::string("unable to add attribute '") + getString(attr) + "' with value '" << value << "', tag is already opened." );

	if( value.empty() ) // empty string => nothing to add
	{
		HTTAG_WARNING << "warning: asking to add tag attribute '" << getString(attr) << "' but string is empty\n";
		return;
	}
#ifndef HTTAG_NO_CHECK
	if( !attribIsAllowed( attr, _tag_en ) )
		HTTAG_ERROR( std::string( "attempt to assign attribute '") + getString(attr) + "' to tag '" + getString( _tag_en )+  "': invalid with html5" );
#endif

// check for unneeded pairs attribute/value
		if( attr == AT_COLSPAN && value == "1" )
		{
			HTTAG_WARNING << "asking to add unnecessary attribute/value: '" << getString(attr) << "'=" << value << '\n';
			return;
		}
		if( attr == AT_ROWSPAN && value == "1" )
		{
			HTTAG_WARNING << "asking to add unnecessary attribute/value: '" << getString(attr) << "'=" << value << '\n';
			return;
		}

	if( _attr_map.find(attr) != _attr_map.end() )   // check if element is already present or not
	{
		std::ostringstream oss;
		oss << _attr_map[attr] << ' ' << value;
		_attr_map[attr] = oss.str();
	}
	else
		_attr_map[attr] = value;
}
//-----------------------------------------------------------------------------------
/// Remove attribute
/**
\todo instead of storing an empty string, remove the element (see map ref)
*/
inline
void
Httag::removeAttrib( En_Attrib attr )
{
//	if( _tagIsOpen ); // because if it is open, then we can't remove it!
//		HTTAG_ERROR( "asking to remove attribute on open tag" );

	if( _attr_map.find(attr) == _attr_map.end() )   // check if element is already present or not
	{
		HTTAG_WARNING << "asked to remove attribute "
			<< getString( attr )
			<< " to tag " << getString( _tag_en )
			<< " but attribute no present.\n";
	}
	else
		_attr_map.at(attr) = std::string();
}
//-----------------------------------------------------------------------------------
/// Returns a string holding the attributes
inline
std::string
Httag::p_getAttribs() const
{
	GlobAttribMap_t& gattr = globalAttrib();  // check is there is a global attribute for that tag
	const PairAttribString_t* gpatst = 0;
	if( gattr.count(_tag_en) )
		gpatst = &gattr.at(_tag_en);

	std::string out;
//	if( _printAttribs && _attr_map.size() )
	if( _attr_map.size() )
	{
		for( auto it = _attr_map.begin(); it != _attr_map.end(); ++it )
		{
			out += ' ';
			out += getString( it->first );
			out += "=\"" + it->second;
			if( gpatst )
			{                                          // IF we found a global attribute for that tag
				if( it->first == gpatst->first )       // then add its value
					out += ' '+ gpatst->second;
				}
			out += '"';
		}
	}
	else // if no local attributes, then check for global
	{
		if( gpatst )
			out += std::string(" ") + getString( gpatst->first ) + "=\"" + gpatst->second + '\"';
	}

// check for a global attribute for the current tag
/*
		GlobAttribMap_t& gattr = globalAttrib();
		if( gattr.count(_tag_en) )
		{
			const auto& p = gattr.at(_tag_en);
			out += " " + std::string(getString( p.first )) + "=\"" + p.second + '"';
		}
	}
*/
	return out;
}
//-----------------------------------------------------------------------------------
/// Returns true if the default behavior for \c tag is to have a line feed after
bool
hasDefaultLineFeed( En_Httag tag )
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
Httag::doLineFeed( bool linefeed ) const
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
operator << ( std::ostream& s, const Httag& h )
{
	s << '<' << getString( h._tag_en )
		<< h.p_getAttribs()
		<< '>';
	if( !isVoidElement( h.getTag() ) )
		s << h._content << "</" << getString( h._tag_en ) << '>';
    if( h._isFileType )
		h.doLineFeed();
	return s;
}
//-----------------------------------------------------------------------------------

} // namespace httag end

#endif // HG_CPPHTMLTAGS_HPP

