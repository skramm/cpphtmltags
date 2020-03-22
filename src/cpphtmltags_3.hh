//--- START: TEMPLATE FILE cpphtmltags_3.hh

//-----------------------------------------------------------------------------------
/// Returns true if \c tag belong to category \c cat
bool
tagBelongsToCat( En_Httag tag, En_TagCat cat )
{
	assert( cat != C_DUMMY );
	assert( tag != HT_DUMMY );

	static TagCat tagcat;
	const auto& v_cat = tagcat.get().at(cat);

	auto it = std::find(
				std::begin(v_cat),
				std::end(v_cat),
				tag
			);
	if( it == std::end(v_cat) )
		return false;
	return true;
}

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
	assert( attr != AT_DUMMY );

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
inline
bool
attribIsAllowed( En_Attrib attr, En_Httag tag )
{
	assert( tag  != HT_DUMMY );
	assert( attr != AT_DUMMY );

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
/// Returns true if the default behavior for \c tag is to have a line feed after opening
bool
hasDefaultLF_Open( En_Httag tag )
{
	assert( tag != HT_DUMMY );

	switch( tag )
	{
		case HT_HTML:
		case HT_BODY:
		case HT_HEAD:
		case HT_TABLE:
		case HT_TR:
		case HT_UL:
		case HT_OL:
			return true;
		default: break;
	}
	return false;
}

//-----------------------------------------------------------------------------------
/// Returns true if the default behavior for \c tag is to have a line feed after closing
bool
hasDefaultLF_Close( En_Httag tag )
{
	assert( tag != HT_DUMMY );

	switch( tag )
	{
		case HT_HTML:
		case HT_BODY:
		case HT_HEAD:
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
/// holds the list of currently opened tags
class OpenedTags
{
	private:
		std::vector<En_Httag> _v_ot;
	public:
		size_t size() const { return _v_ot.size();  }
		void printOT( std::ostream& f ) const
		{
			for( const auto& e: _v_ot )
				f << e << '-';
		}
		void pushTag( En_Httag tag )
		{
			_v_ot.push_back( tag );
		}
		void pullTag( En_Httag tag )
		{
			assert( _v_ot.size() > 0 );
			if( _v_ot.back() != tag )
				HTTAG_ERROR( std::string( "asking to close tag '") + getString(tag) + "' but tag '" +  getString(_v_ot.back()) + "' still open" );
			_v_ot.pop_back();
		}
//		void print( std::ostream& ) const;
		std::string str() const
		{
			std::ostringstream oss;
			for( const auto t: _v_ot )
				oss << '<' << getString( t ) << '>';
			return oss.str();
		}

		En_Httag current() const
		{
			assert( _v_ot.size() );
			return _v_ot.back();
		}
};
//-----------------------------------------------------------------------------------
/// Returns true if tag is allowed inside tag chain
inline
bool
tagIsAllowed( En_Httag tag, const OpenedTags& ot, const AllowedContentMap& acm )
{
	const auto& ac = acm.getC( ot.current() ); // allowed content of currently (latest) opened tag

	for( auto e: ac.v_forbiddenTags )
		if( e == tag )
			return false;

	for( auto cat: ac.v_forbiddenCats )
		if( tagBelongsToCat( tag, cat ) )
			return false;

	for( auto cat: ac._v_allowedCats )
		if( tagBelongsToCat( tag, cat ) )
			return true;

	for( auto e: ac.v_allowedTags )
		if( e == tag )
			return true;

	return false;
}
//-----------------------------------------------------------------------------------
template<typename T>
void
printAllowedContent( std::ostream& f, std::string af, std::string type, std::vector<T> vec )
{
	if( vec.size() )
	{
		f << "\n -" << af << ' ' << vec.size() << ' ' << type << "(s): ";
		for( const auto& e: vec )
			f << getString(e) << ' ';
	}
}
//-----------------------------------------------------------------------------------
/// Prints some details about allowed tags content
inline
void
AllowedContentMap::print( std::ostream& f ) const
{
	f << "* List of allowed content in a tag:\n";

	size_t c = 0;
	size_t nbEmpty = 0;
	for( const auto& elem: _map_AllowedContent )
	{
		const auto& s = elem.second;
		f << ++c << ": tag <" << getString(elem.first) << '>';
		if( s.isEmpty() )
		{
			nbEmpty++;
			f << " : EMPTY !!!";
		}
		else
		{
			printAllowedContent( f, "allows",  "categorie", s._v_allowedCats );
			printAllowedContent( f, "allows",  "tag",       s.v_allowedTags );
			printAllowedContent( f, "forbids", "categorie", s.v_forbiddenCats );
			printAllowedContent( f, "forbids", "tag",       s.v_forbiddenTags );
		}
		f << '\n';
	}
	f << "\n -Nb empty tags: = " << nbEmpty;
	f << "\n -Nb tags = " << _map_AllowedContent.size() << '\n';
}
//-----------------------------------------------------------------------------------

} // namespace priv

//-----------------------------------------------------------------------------------
/// Line Feed Mode (see manual)
enum En_LineFeedMode
{
	LF_None, LF_Always, LF_Default
};

//-----------------------------------------------------------------------------------
/// HTML tag
/**
Main class, see homepage for details
*/
class Httag
{
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
		static void printSupported(  std::ostream& );
		static size_t printOpenedTags( std::ostream&, const char* msg=0 );

	private:
		void doLineFeed( bool linefeed=false ) const;
		void p_addAttrib( En_Attrib, std::string );
		void p_checkValidFileType( std::string action );
		std::string p_getAttribs() const;

		static priv::OpenedTags& p_getOpenedTags()
		{
			static priv::OpenedTags s_ot;
			return s_ot;
		}
		static priv::AllowedContentMap p_getAllowedContentMap()
		{
			static priv::AllowedContentMap s_allowed_tags;
			return s_allowed_tags;
		}
		static priv::GlobAttribMap_t& globalAttrib()
		{
			static priv::GlobAttribMap_t s_global_attrib;
			return s_global_attrib;
		}
		static En_LineFeedMode& lf_mode()
		{
			static En_LineFeedMode _lf_mode = LF_Default;
			return _lf_mode;
		}
	private:
		En_Httag        _tag_en;
		std::ostream*   _file;
		bool            _isFileType;
		std::string     _content;
//		bool            _printAttribs  = true;
		bool            _tagIsOpen     = false;
		std::map<En_Attrib,std::string> _attr_map;
};

//-----------------------------------------------------------------------------------
/// Helper function, prints the tags and attributes currently supported
inline
void
Httag::printSupported( std::ostream& f )
{
	f << "* Supported tags: " << HT_DUMMY << '\n';
	for( size_t i=0; i<HT_DUMMY; i++ )
		f << getString( static_cast<En_Httag>(i) ) << '-';
	f << '\n';

	f << "* Supported attributes: " << AT_DUMMY << '\n';
	for( size_t i=0; i<AT_DUMMY; i++ )
		f << getString( static_cast<En_Attrib>(i) ) << '-';
	f << '\n';

	f << "* Tag categories: " << priv::C_DUMMY << '\n';
	for( size_t i=0; i<priv::C_DUMMY; i++ )
		f << getString( static_cast<priv::En_TagCat>(i) ) << '-';
	f << '\n';

	auto ac = p_getAllowedContentMap();
	ac.print( f );

#if 0
	auto acC = ac._map_content_C;
	auto nb1 = std::count_if(
		std::begin( acC ), std::end( acC ),
		[]
		( const priv::TagAllowedCat_t& p )
		{
			return static_cast<bool>( p.second.size() );
		}
	);
    f << "* Nb of tags holding an allowed tag category=" << nb1 << "\n";
//	for( const auto& elem: acC )
//		std::cout << getString( elem.first ) << "\n";


	auto acT = ac._map_content_T;
	auto nb2 = std::count_if(
		std::begin( acT ), std::end( acT ),
		[]
		( const priv::TagAllowedTag_t& p )
		{
			return static_cast<bool>( p.second.size() );
		}
	);

    f << "* Nb of tags holding an allowed tag list=" << nb2 << "\n";
#endif
}
//-----------------------------------------------------------------------------------
/// constructor 1
inline
Httag::Httag( En_Httag tag )
	: _tag_en( tag ), _file(0), _isFileType(false)
{
	assert( tag != HT_DUMMY );
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
{
	assert( tag != HT_DUMMY );
}
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
	assert( tag != HT_DUMMY );
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
	assert( tag != HT_DUMMY );
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
	assert( tag != HT_DUMMY );
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
	if( priv::isVoidElement( _tag_en ) )
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

	if( !priv::isVoidElement( _tag_en ) )
		closeTag();
}

//-----------------------------------------------------------------------------------
/// Destructor, automatically closes tag if needed
inline
Httag::~Httag()
{
	if( _tagIsOpen && _isFileType && !priv::isVoidElement( _tag_en ) )
		closeTag();
}
//-----------------------------------------------------------------------------------
inline
size_t
Httag::printOpenedTags( std::ostream& f, const char* msg )
{
	f << "httag: opened tags (#=" << p_getOpenedTags().size() << "):";
	if( msg )
		f << "msg='" << msg << "' ";
	f << p_getOpenedTags().str() << '\n';
	return p_getOpenedTags().size();
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
	{
		if( p_getOpenedTags().size() )
		{
			if( p_getOpenedTags().current() == _tag_en )
				HTTAG_ERROR( std::string("attempt to open tag <") + getString(_tag_en) + "> but currently opened tag is identical" );

			if( !tagIsAllowed( _tag_en, p_getOpenedTags(), p_getAllowedContentMap() ) )
			{
//				std::cerr << "TAG NOT ALLOWED !!!\n";

				HTTAG_ERROR( std::string("attempt to open tag <") + getString(_tag_en) + "> but is not allowed in current context:" + p_getOpenedTags().str() );
			}
		}
		switch( _tag_en )
		{
			case HT_COMMENT:  *_file << "<!-- "; break;
			case HT_DOCTYPE: *_file << "<!DOCTYPE html>\n"; break;
			default:
				*_file << '<' << getString(_tag_en) << p_getAttribs() << '>';
		}

	_tagIsOpen = true;
//	_printAttribs = false;

	p_getOpenedTags().pushTag( _tag_en );

	if( priv::hasDefaultLF_Open( _tag_en ) )
		*_file << '\n';
	}
}
//-----------------------------------------------------------------------------------
/// Close the tag (this function needs to be called ONLY for "file" object types
inline
void
Httag::closeTag( bool linefeed )
{
	p_checkValidFileType( "close" );

	if( priv::isVoidElement( _tag_en ) )
		HTTAG_ERROR( std::string( "asked to close tag <" ) + getString(_tag_en) + "> but is void-element" );

	if( !_tagIsOpen )
		HTTAG_ERROR( std::string( "tag <" ) + getString(_tag_en) + ">: asked to close but was already closed." );

	if( _tag_en == HT_COMMENT )
		*_file << "-->\n";
	else
		*_file << "</" << getString(_tag_en) << '>';

	p_getOpenedTags().pullTag( _tag_en );

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
	assert( tag != HT_DUMMY );
	assert( att != AT_DUMMY );

	globalAttrib()[tag] = std::make_pair( att, value );
}

//-----------------------------------------------------------------------------------
/// Returns the global pair "attribute=string" for \c tag, if any
inline
std::string
Httag::getGlobalAttrib( En_Httag tag )
{
	assert( tag != HT_DUMMY );

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
	assert( attr != AT_DUMMY );

	if( _tagIsOpen ) // because if it is already opened, then we can't add an attribute !
		HTTAG_ERROR( std::string("unable to add attribute '") + getString(attr) + "' with value '" + value + "', tag is already opened." );

	if( value.empty() ) // empty string => nothing to add
	{
		HTTAG_WARNING << "warning: asking to add tag attribute '" << getString(attr) << "' but string is empty\n";
		return;
	}
#ifndef HTTAG_NO_CHECK
	if( !priv::attribIsAllowed( attr, _tag_en ) )
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
	assert( attr != AT_DUMMY );

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
	priv::GlobAttribMap_t& gattr = globalAttrib();  // check is there is a global attribute for that tag
	const priv::PairAttribString_t* gpatst = 0;
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
		case LF_Default: doIt = priv::hasDefaultLF_Close( _tag_en ) | linefeed; break;
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
	switch( h._tag_en )
	{
		case HT_COMMENT: s << "<!-- "; break;
		case HT_DOCTYPE: s << "<!DOCTYPE html>\n"; break;
		default:
			s << '<' << getString( h._tag_en )
				<< h.p_getAttribs() << '>';
	}
	if( !priv::isVoidElement( h.getTag() ) )
	{
		s << h._content;
		if( h._tag_en == HT_COMMENT )
			s << " -->\n";
		else
			s << "</" << getString( h._tag_en ) << '>';
	}

    if( h._isFileType )
		h.doLineFeed();
	return s;
}
//-----------------------------------------------------------------------------------

} // namespace httag end

#endif // HG_CPPHTMLTAGS_HPP

//--- END: TEMPLATE FILE cpphtmltags_3.hh
