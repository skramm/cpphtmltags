


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

