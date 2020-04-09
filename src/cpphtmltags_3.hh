//--- START: TEMPLATE FILE cpphtmltags_3.hh

std::string g_lt{ "&lt;" };
std::string g_gt{ "&gt;" };

//-----------------------------------------------------------------------------------
/// Returns true if \c tag belong to category \c cat
inline
bool
tagBelongsToCat( En_Httag tag, En_TagCat cat )
{
	assert( cat != C_DUMMY );
	assert( tag != HT_DUMMY );

	static TagCategory s_tagcat;
	const auto& v_cat = s_tagcat.get().at(cat);

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
/// Returns true if attribute \c attr is allowed for \c tag
inline
bool
attribIsAllowed( En_Attrib attr, En_Httag tag )
{
	assert( tag  != HT_DUMMY );
	assert( attr != AT_DUMMY );

	static MapAttribs s_mapAllowedAttribs;
	if( s_mapAllowedAttribs.get().count(attr) ) // if it is there
	{
		const auto& vec = s_mapAllowedAttribs.get().at(attr);
		auto res = std::find( std::begin( vec ), std::end( vec ), tag );
		if( res == std::end( s_mapAllowedAttribs.get().at(attr) ) )
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
		case HT_COMMENT:
			return true;
		default: break;
	}
	return false;
}

//-----------------------------------------------------------------------------------
/// Holds the list of currently opened tags.
/**
Statically instanciated in Httag, see Httag::p_getOpenedTags()
*/
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
		/// adds \c tag to stack, to keep control of what is opened
		void pushTag( En_Httag tag )
		{
			_v_ot.push_back( tag );
		}
		/// pulls \c tag from stack. In case of error, returns (false,<tag-still-opened>)
		std::pair<bool,En_Httag> pullTag( En_Httag tag )
		{
			assert( _v_ot.size() > 0 );

			if( _v_ot.back() != tag )
				return std::make_pair( false, _v_ot.back() );
			_v_ot.pop_back();
			return std::make_pair( true, HT_DUMMY );
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
/// Helper function for AllowedContentMap::print()
template<typename T>
void
printAllowedContent( std::ostream& f, std::string af, std::string type, std::vector<T> vec )
{
	if( vec.size() )
	{
		f << " -" << af << ' ' << vec.size() << ' ' << type << "(s): ";
		for( const auto& e: vec )
			f << getString(e) << ' ';
		f << '\n';
	}
	else
		f << " -" << af << "/" << type << ": empty\n";
}
//-----------------------------------------------------------------------------------
/// Prints some details about allowed tags content
inline
void
AllowedContentMap::print( std::ostream& f ) const
{
	f << "* List of allowed content in a tag:\n";

	size_t c = 0;
	for( const auto& elem: _map_AllowedContent )
	{
		const auto& s = elem.second;
		f << ++c << ": tag <" << getString(elem.first) << ">:\n";
		printAllowedContent( f, "allows",  "categorie", s._v_allowedCats );
		printAllowedContent( f, "allows",  "tag",       s._v_allowedTags );
		printAllowedContent( f, "forbids", "categorie", s._v_forbiddenCats );
		printAllowedContent( f, "forbids", "tag",       s._v_forbiddenTags );
		f << '\n';
	}
	f << "\n -Nb tags = " << _map_AllowedContent.size() << '\n';
}
//-----------------------------------------------------------------------------------
enum En_UnallowedTag { UT_Undef, UT_ForbiddenTag, UT_ForbiddenCat, UT_NotAllowed, UT_Doctype };


// forward declaration, needed because it gets declared as friend in Httag but is in a sub-namespace
void p_printTable_1( std::ostream&, int id );

// forward declaration, needed because it gets declared as friend in Httag but is in a sub-namespace
std::pair<bool,En_UnallowedTag> tagIsAllowed( En_Httag, En_Httag );

// forward declaration, needed because it gets declared as friend in Httag but is in a sub-namespace
std::ostream& streamTagInFile( std::ostream&, const Httag& );


//-----------------------------------------------------------------------------------
/// Helper function for operator << for 2 tags, stream t2 into t1
/**
 \todo See how we can replace macro HTTAG_FATAL_ERROR with call to p_error()
*/
template<typename T1, typename T2>
T1&
streamTagInTag( T1& t1, T2& t2 )
{
	std::ostringstream oss;

	auto check = priv::tagIsAllowed( t2.getTag(), t1.getTag() );
	if( check.first
#ifdef HTTAG_NO_CHECK              // if "no checking", then no error
		|| true
#endif
	)
		oss << t2;
	else
		HTTAG_FATAL_ERROR(
//		Httag::p_error( priv::ET_NonFatal,
			"tag <"
			+ getString( t2._tag_en )
			+ "> not allowed as content inside tag <"
			+ getString( t1._tag_en )
			+ ">, cause: "
			+ getString(check.second)
		);

	t1._content += oss.str();
	return t1;
}



//-----------------------------------------------------------------------------------
/// Dedicated to hold (statically) run time options, see Httag::setOption()
struct RunTimeOptions
{
	rto::En_LineFeedMode _lineFeedMode = rto::LF_Default;
	rto::En_ErrorMode    _errorMode    = rto::EM_Throw;
	bool _clearContentOnClose = false;
	bool _clearAttribOnClose  = false;
	bool _doContentCheck      = true;
};

//############################
} // namespace priv
//############################



//-----------------------------------------------------------------------------------
/// HTML tag
/**
Main class, see homepage for details
*/
class Httag
{
	template<typename T1,typename T2>
	friend T1& priv::streamTagInTag( T1& tag, T2& t2 );

	friend std::ostream& priv::streamTagInFile( std::ostream&, const Httag& );

	template<typename T>
	friend Httag&        operator << ( Httag&, const T& );

	friend std::ostream& operator << ( std::ostream& stream, const Httag& );
	friend std::ostream& operator << ( std::ostream& stream,       Httag& );

	friend void priv::p_printTable_1( std::ostream&, int id );
	friend std::pair<bool,priv::En_UnallowedTag> priv::tagIsAllowed( En_Httag, En_Httag );

	public:
/// \name Constructors & destructors
///@{
		Httag( std::ostream&, En_Httag );

		template<typename T>
		Httag( std::ostream&, En_Httag, T t=T() );

		template<typename T>
		Httag( std::ostream&, En_Httag, En_Attrib=AT_DUMMY, T attribvalue=T() );

		template<typename T1,typename T2>
		Httag( std::ostream&, En_Httag, T1 content, En_Attrib=AT_DUMMY, T2 attribvalue=T2() );

		Httag( En_Httag );

		template<typename T1>
		Httag( En_Httag, T1 content );
		template<typename T1, typename T2>
		Httag( En_Httag, T1 content, En_Attrib, T2 attribvalue );
		template<typename T>
		Httag( En_Httag, En_Attrib, T attribvalue );
		~Httag();

		std::string to_string() const
		{
			std::ostringstream oss;
			oss << *this;
			return oss.str();
		}
		operator std::string() const
		{
			std::ostringstream oss;
			oss << *this;
			return oss.str();
		}

///@}
		void openTag(   priv::SrcLocation src= priv::SrcLocation() );
		void closeTag(  priv::SrcLocation src= priv::SrcLocation(), bool linefeed=false );
		void closeTag( bool linefeed );

		template<typename T>
		Httag& addAttrib( En_Attrib, T, priv::SrcLocation=priv::SrcLocation() );

		Httag& addAttrib( En_Attrib, priv::SrcLocation=priv::SrcLocation() );

		Httag& removeAttrib( En_Attrib );
		Httag& clearAttribs()
		{
			_attr_map.clear();
			return *this;
		}

		void printTag()
		{
			printTag( std::string() );
		}

		template<typename T>
		void printTag( T );

		En_Httag getTag() const { return _tag_en; }
		bool isOpen() const { return _tagIsOpen; }

/// \name Global attributes handling
///@{
		static void setGlobalAttrib( En_Httag tag, En_Attrib att, const std::string& value );

//		static std::string getGlobalAttrib( En_Httag tag );

		/// Remove the global attribute for \c tag
		static void clearGlobalAttrib( En_Httag tag )
		{
			p_globalAttrib().erase( tag );
		}
		static void clearGlobalAttrib( En_Httag tag, En_Attrib attrib )
		{
			auto& ga = p_globalAttrib();
			if( ga.count(tag) )            // if we have that tag in the map
			{
				auto& atrm = ga[tag];      // then we fetch the associated map,
				if( atrm.count(attrib) )   // check if it holds the key "attrib",
					atrm.erase( attrib );  // and if so, erase it
			}

		}
#if 0
		static void clearGlobalAttrib( En_Attrib attrib )
		{
			auto& gam = globalAttrib();
//			for( auto& pa: gam )
 /// odo finish this !
		}
#endif
		/// Remove all global attributes
		static void clearGlobalAttribs()
		{
			p_globalAttrib().clear();
		}
///@}

/// \name Tag content related functions
///@{
		template<typename T> Httag& addContent( T content );

		template<typename T>
		Httag& setContent( T content )
		{
			clearContent();
			return addContent( content );
		}

		Httag& clearContent()
		{
			_content.clear();
			return *this;
		}

		Httag& clear()
		{
			return clearContent().clearAttribs();
		}
///@}

		template<typename T1,typename T2>
		static void setOption( T1, T2 );

		static void printSupported( std::ostream& );
		template<typename T>
		static void printSupportedHtml( std::ostream&, T );
		static size_t printOpenedTags( std::ostream&, const char* msg=0 );

	private:
		bool p_doLineFeed( bool linefeed=false ) const;
		void p_addAttrib( En_Attrib, std::string, priv::SrcLocation=priv::SrcLocation());
		std::string p_getAttribs() const;

		/// Static accessor on list of opened tags
		static priv::OpenedTags& p_getOpenedTags()
		{
			static priv::OpenedTags s_ot;
			return s_ot;
		}
		static const priv::AllowedContentMap& p_getAllowedContentMap()
		{
			static priv::AllowedContentMap s_allowed_tags;
			return s_allowed_tags;
		}
		static priv::GlobAttribMap_t& p_globalAttrib()
		{
			static priv::GlobAttribMap_t s_global_attrib;
			return s_global_attrib;
		}

		static int& p_getNbErrors()
		{
			static int s_nbErrors;
			return s_nbErrors;
		}
		static void p_error( priv::En_ErrorType, const std::string&, priv::SrcLocation src= priv::SrcLocation() );

		static priv::RunTimeOptions& p_getRunTimeOptions()
		{
			static priv::RunTimeOptions s_runTimeOptions;
			return s_runTimeOptions;
		}

	private:
		En_Httag        _tag_en;
		std::ostream*   _file;
		bool            _isFileType;
		std::string     _content;
		bool            _tagIsOpen     = false;
		std::map<En_Attrib,std::string> _attr_map;
};

//-----------------------------------------------------------------------------------
std::string
getString( const Httag& t )
{
	return getString( t.getTag() );
}

//-----------------------------------------------------------------------------------
void
Httag::p_error(
	priv::En_ErrorType et,
	const std::string& msg,
	priv::SrcLocation  src   ///< location of error, if provided
)
{
	std::ostringstream oss;
	oss << "\n";
	if( src.second )
		oss << " - Location:\n - file=" << src.first << "\n - line=" << src.second << '\n';

	#ifndef HTTAG_SILENT_MODE
		std::cerr << "httag: "
			<< getString(et)
			<< " ("
			<< ++p_getNbErrors()
			<< "), "
			<< msg
			<< oss.str();
	#endif
//	if( __line )
//		std::cerr << "Location:\n - file=" << __file << "\n - line=" << __line << '\n';
	if( Httag::p_getRunTimeOptions()._errorMode == rto::EM_Throw || et==priv::ET_Fatal )
		throw std::runtime_error( msg+ oss.str() );
}

//-----------------------------------------------------------------------------------
///{ Run Time Options settings


template<>
void Httag::setOption<rto::LFMode_t,rto::En_LineFeedMode>( rto::LFMode_t name, rto::En_LineFeedMode value )
{
	p_getRunTimeOptions()._lineFeedMode = value;
}

template<>
void Httag::setOption<rto::IllegalOp_t,rto::En_ErrorMode>( rto::IllegalOp_t name, rto::En_ErrorMode value )
{
	p_getRunTimeOptions()._errorMode = value;
}

template<>
void Httag::setOption<rto::ClearOnClose_t,bool>( rto::ClearOnClose_t name, bool value )
{
	p_getRunTimeOptions()._clearContentOnClose = value;
	p_getRunTimeOptions()._clearAttribOnClose = value;
}

template<>
void Httag::setOption<rto::ClearOnClose_C_t,bool>( rto::ClearOnClose_C_t name, bool value )
{
	p_getRunTimeOptions()._clearContentOnClose = value;
}
template<>
void Httag::setOption<rto::ClearOnClose_A_t,bool>( rto::ClearOnClose_A_t name, bool value )
{
	p_getRunTimeOptions()._clearAttribOnClose = value;
}

/// Default templated function, should never be instanciated
template<typename T1,typename T2>
void setOption( T1 name, T2 value )
{
	static_assert( std::is_same<T1,T1>::value, "should never happen !" );
}
///}
//-----------------------------------------------------------------------------------

//############################
namespace priv {
//############################

std::ostream&
streamTagInFile( std::ostream& s, const Httag& h )
{
	switch( h._tag_en )
	{
		case HT_COMMENT: s << "<!-- "; break;
		case HT_DOCTYPE:
		{
			if( Httag::p_getOpenedTags().size() == 0 )
				s << "<!DOCTYPE html>\n";
			else
				Httag::p_error( priv::ET_NonFatal,
					"<doctype> needs to be the first tag, and you have "
					+ std::to_string( Httag::p_getOpenedTags().size() )
					+ " tags open, last one is <"
					+ getString( Httag::p_getOpenedTags().current() )
					+ ">"
			);
		}
		break;
		default:
			s << '<' << getString( h._tag_en )
				<< h.p_getAttribs() << '>';
	}
	if( !priv::isVoidElement( h.getTag() ) )
	{
		s << h._content;
		if( h._tag_en == HT_COMMENT )
			s << " -->";
		else
			s << "</" << getString( h._tag_en ) << '>';
	}

	return s;
}

//-----------------------------------------------------------------------------------
inline
std::string
getString( En_UnallowedTag cause )
{
	switch( cause )
	{
		case UT_ForbiddenTag:  return "ForbiddenTag";
		case UT_ForbiddenCat:  return "ForbiddenCat";
		case UT_Undef:         return "Undefined";
		case UT_NotAllowed:    return "NotAllowed";
		case UT_Doctype:       return "Doctype";

		default: assert(0);
	}
	return std::string(); // to avoid a build warning
}

//-----------------------------------------------------------------------------------
/// Returns true if \c tag is allowed inside tag \c parent
inline
std::pair<bool,En_UnallowedTag>
tagIsAllowed(
	En_Httag tag,    ///< the tag
	En_Httag parent  ///< parent tag
)
{
	if( tag == HT_DOCTYPE )                          // if we have called this function,
		return std::make_pair(false,UT_Doctype);     // it means there IS a 'parent' tag, thus, we can't have a DOCTYPE !

	const auto& ac = Httag::p_getAllowedContentMap().get( parent ); // allowed content of currently (latest) opened tag

	for( auto e: ac._v_forbiddenTags )
		if( e == tag )
			return std::make_pair(false,UT_ForbiddenTag);

	for( auto cat: ac._v_forbiddenCats )
		if( tagBelongsToCat( tag, cat ) )
			return std::make_pair(false,UT_ForbiddenCat);

	if( ac._v_allowedCats.empty() && ac._v_allowedTags.empty() )
		return std::make_pair(true,UT_Undef);
	else
	{
		for( auto cat: ac._v_allowedCats )
			if( tagBelongsToCat( tag, cat ) )
				return std::make_pair(true,UT_Undef);

		for( auto e: ac._v_allowedTags )
			if( e == tag )
				return std::make_pair(true,UT_Undef);
	}
	return std::make_pair(false,UT_NotAllowed);
}
//-----------------------------------------------------------------------------------
/// Returns true if \c tag is allowed inside current tag chain
inline
std::pair<bool,En_UnallowedTag>
tagIsAllowed(
	En_Httag                 tag,    ///< the tag
	const OpenedTags&        otags  ///< current context
)
{
	return tagIsAllowed( tag, otags.current() ); //, acm );
}

//############################
} // namespace priv
//############################


//-----------------------------------------------------------------------------------
/// Helper function, prints the tags and attributes currently supported
/* See related Httag::printSupportedHtml()
**/
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
Httag::Httag(
	En_Httag tag
) : _tag_en( tag ), _file(0), _isFileType(false)
{
	assert( tag != HT_DUMMY );
}
//-----------------------------------------------------------------------------------
/// generic constructor 2
template<typename T>
Httag::Httag( En_Httag tag, T content ) : Httag( tag )
{
	setContent( content );
}
//-----------------------------------------------------------------------------------
/// generic constructor 3a
template<typename T1, typename T2>
Httag::Httag(
	En_Httag   tag,
	T1         content,
	En_Attrib  attr,
	T2         attribvalue
) : Httag( tag )
{
	setContent( content );

	if( attr != AT_DUMMY )
		addAttrib( attr, attribvalue );
}
//-----------------------------------------------------------------------------------
/// generic constructor 3b
template<typename T>
Httag::Httag(
	En_Httag   tag,
	En_Attrib  attr,
	T          attribvalue
) : Httag( tag )
{
	if( attr != AT_DUMMY )
		addAttrib( attr, attribvalue );
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
	En_Httag       tag,          ///< the html tag id
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
Httag&
Httag::addContent<std::string>( std::string content )
{
	if( priv::isVoidElement( _tag_en ) )
		p_error( priv::ET_NonFatal,
			"attempting to store content '"
			+ content
			+ "' into a void tag <"
			+ getString( _tag_en )
			+ ">"
		);
	else
		_content += content;
	return *this;
}

/// specialization for const char*
template<>
Httag&
Httag::addContent<const char*>( const char* content )
{
	addContent<std::string>( std::string(content) );
	return *this;
}

/// specialization for Httag
template<>
Httag&
Httag::addContent<Httag>( Httag content )
{

#ifndef HTTAG_NO_CHECK
// make sure the tag we what to insert is allowed
	auto res = priv::tagIsAllowed( content.getTag(), getTag() );
	if( !res.first )
		p_error( priv::ET_NonFatal,
			"attempting to insert <"
			+ getString( content.getTag() )
			+ "> as content of <"
			+ getString( getTag() )
			+ ">, which is forbidden (reason: "
			+ getString( res.second )
			+ ")"
		);
#endif
	std::ostringstream oss;
	oss << content;
	addContent<std::string>( oss.str() );
	return *this;
}

/// default implementation
template<typename T>
Httag&
Httag::addContent( T content )
{
	addContent<std::string>( std::to_string(content) );
	return *this;
}
//-----------------------------------------------------------------------------------

//############################
namespace priv {
//############################


bool
isNotEmpty( const char* stuff )
{
	return !std::string(stuff).empty();
}

template<typename T>
bool
isNotEmpty( T stuff )
{
	return !stuff.empty();
}


//############################
}  // namespace priv
//############################

//-----------------------------------------------------------------------------------
/// Prints whole tag:
/**
- opens it (if not already done),
- prints content \c stuff (added to content that might have been already added)
- close tag,

Requirement on type \c T: must be streamable
*/
template<typename T>
void Httag::printTag( T stuff )
{
	if( !isOpen() )
		openTag();

	if( !_content.empty()  || priv::isNotEmpty(stuff) )
	{
		if( !priv::isVoidElement( _tag_en ) )
			*_file << _content << stuff;
		else
			Httag::p_error( priv::ET_NonFatal,
				"attempting to add content '"
				+ _content
				+ "' and '"
				+ stuff
				+ "' to void tag <"
				+ getString(_tag_en)
				+ ">"
			);
	}

	if( !priv::isVoidElement( _tag_en ) )
		closeTag();
	else
	{
		auto retval = p_getOpenedTags().pullTag( _tag_en );
		if( !retval.first )
			Httag::p_error( priv::ET_Fatal,
				"unable to close tag <"
				+ getString(_tag_en)
				+ "> because tag <"
				+ getString(retval.second)
				+ "> still open"
			);
	}
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
/// Open the tag (this function needs to be called ONLY for "file" object types
/*
The (default) arguments are there to be handled by macro HTTAG_OPEN(t)
*/
inline
void
Httag::openTag( priv::SrcLocation src )
{
	if( !_isFileType )
		Httag::p_error( priv::ET_Fatal,
			"object tag <"
			+ getString(_tag_en)
			+ "> is not a \"file type\" object",
			src
		);
	assert( _file );

	if( _tagIsOpen )
		p_error( priv::ET_NonFatal,
			"asked to open tag <"
			+ getString(_tag_en)
			+ "> but was already open",
			src
		);
	else
	{
		if( p_getOpenedTags().size() )
		{
			auto check = priv::tagIsAllowed( _tag_en, p_getOpenedTags() );
			if( !check.first )
				Httag::p_error( priv::ET_NonFatal,
					"attempt to open tag <"
					+ getString(_tag_en)
					+ "> but is not allowed in current context:\n"
					+ p_getOpenedTags().str()
					+ "\n-because: "
					+ getString( check.second ),
					src
				);
		}
		switch( _tag_en )
		{
			case HT_COMMENT: *_file << "<!-- "; break;
			case HT_DOCTYPE: *_file << "<!DOCTYPE html>\n"; break;
			default:
				*_file << '<' << getString(_tag_en) << p_getAttribs() << '>';
		}

	_tagIsOpen = true;

	p_getOpenedTags().pushTag( _tag_en );

	if( priv::hasDefaultLF_Open( _tag_en ) )
		*_file << '\n';
	}
}
//-----------------------------------------------------------------------------------
/// Close the tag (this function needs to be called ONLY for "file" object types
/// \todo maybe we should enforce the above requirement ? Check that
inline
void
Httag::closeTag( bool linefeed )
{
	closeTag( priv::SrcLocation(), linefeed );
}

inline
void
Httag::closeTag( priv::SrcLocation src, bool linefeed )
{
	if( priv::isVoidElement( _tag_en ) )
		if( _isFileType )
			p_error( priv::ET_NonFatal,
				"asked to close tag <"
				+ getString(_tag_en)
				+ "> but is void-element",
				src
			);

	if( !_tagIsOpen )
		p_error( priv::ET_Fatal,
			"tag <"
			+ getString(_tag_en)
			+ ">: asked to close but was already closed",
			src
		);

	if( _isFileType )
	{
		if( _tag_en == HT_COMMENT )
			*_file << " -->";
		else
			*_file << "</" << getString(_tag_en) << '>';

		if( _tagIsOpen )
		{
			auto retval = p_getOpenedTags().pullTag( _tag_en );
			if( !retval.first )
				Httag::p_error( priv::ET_Fatal,
					"unable to close tag <"
					+ getString(_tag_en)
					+ "> because tag <"
					+ getString(retval.second)
					+ "> still open",
					src
				);
		}
	}
	_tagIsOpen = false;

	if( _isFileType )
		if( p_doLineFeed( linefeed ) )
			*_file << '\n';

	const auto& rtopt = p_getRunTimeOptions();
	if( rtopt._clearContentOnClose )      // if option set, clear content
		clearContent();
	if( rtopt._clearAttribOnClose )       // if option set, clear content
		clearAttribs();
}

//-----------------------------------------------------------------------------------
Httag&
operator << ( Httag& tag, Httag& t )
{
	return priv::streamTagInTag( tag, t );
}

Httag&
operator << ( Httag& tag, const Httag& t )
{
	return priv::streamTagInTag( tag, t );
}

//-----------------------------------------------------------------------------------
/// Adding a tag (opening, content, closing) to a string
std::string
operator + ( const std::string& s, const Httag& t )
{
	std::ostringstream oss;
	oss << s << t.to_string();
	return oss.str();
}

//-----------------------------------------------------------------------------------
/// Insert some content into \c tag. Will get printed later
template<typename T>
/*template<
	typename T
	typename std::enable_if<
		!std::is_same<Httag&,T>::value,T
	>::type
>*/
Httag&
operator << ( Httag& tag, const T& str )
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
Httag::setGlobalAttrib( En_Httag tag, En_Attrib att, const std::string& value )
{
	assert( tag != HT_DUMMY );
	assert( att != AT_DUMMY );

//	globalAttrib()[tag] = std::make_pair( att, value );
	p_globalAttrib()[tag][att] = value;
}
//-----------------------------------------------------------------------------------
#if 0
/// Returns the global pair "attribute=string" for \c tag, if any
/// DEPRECATED (no use)
inline
std::string
Httag::getGlobalAttrib( En_Httag tag )
{
	assert( tag != HT_DUMMY );

	if( p_globalAttrib().count(tag) )
		return std::string( getString( p_globalAttrib()[tag].first )) + '=' + p_globalAttrib()[tag].second;
	return std::string();
}
#endif
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag (specialized templated version for \c std::string)
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
template<>
Httag&
Httag::addAttrib<std::string>( En_Attrib attr, std::string value, priv::SrcLocation src )
{
	p_addAttrib( attr, value, src );
	return *this;
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag (specialized templated version for <tt>const char*</tt>)
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
template<>
Httag&
Httag::addAttrib<const char*>( En_Attrib attr, const char* value, priv::SrcLocation src )
{
	p_addAttrib( attr, value, src );
	return *this;
}
//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag (templated generic version)
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
template<typename T>
Httag&
Httag::addAttrib( En_Attrib attr, T value, priv::SrcLocation src )
{
	p_addAttrib( attr, std::to_string(value), src );
	return *this;
}

//-----------------------------------------------------------------------------------
/// Add a boolean attribute (no value)
Httag&
Httag::addAttrib( En_Attrib attr, priv::SrcLocation src )
{
	if( !priv::isBoolAttr( attr ) )
		p_error( priv::ET_NonFatal,
			"attempting to add attribute '"
			+ getString( attr )
			+"' as boolean to tag <"
			+ getString( getTag() )
			+"> but is not boolean",
			src
		);
	else
		p_addAttrib( attr, std::string(), src );
	return *this;
}


//-----------------------------------------------------------------------------------
/// Add an HTML attribute to the tag
/**
If the attribute is already present, then the value will be concatenated to the previous value
*/
inline
void
Httag::p_addAttrib( En_Attrib attr, std::string value, priv::SrcLocation src )
{
	assert( attr != AT_DUMMY );

	if( _tagIsOpen ) // because if it is already opened, then we can't add an attribute !
		p_error( priv::ET_Fatal,
			"unable to add attribute '"
			+ getString(attr) + "' with value '"
			+ value
			+ "' to tag <"
			+ getString( _tag_en )
			+ ">, tag is already opened",
			src
		);

	if( value.empty() && !priv::isBoolAttr(attr) ) // empty string => nothing to add
	{
		p_error( priv::ET_Warning,
			"warning: asking to add tag attribute '"
			+ getString(attr)
			+ "' to tag <"
			+ getString( _tag_en )
			+ ">, but string is empty",
			src
		);
		return;
	}

	if( Httag::p_getRunTimeOptions()._doContentCheck )
		if( !priv::attribIsAllowed( attr, _tag_en ) )
			p_error( priv::ET_NonFatal,
				"attempt to assign attribute '"
				+ getString(attr)
				+ "' to tag <"
				+ getString( _tag_en )
				+  ">: invalid",
				src
			);

// check for unneeded pairs attribute/value
	if( ( attr == AT_COLSPAN && value == "1" ) || ( attr == AT_ROWSPAN && value == "1" ) )
	{
		p_error( priv::ET_Warning,
			"asking to add unnecessary attribute/value: '"
			+ getString(attr)
			+
			"'="
			+ value,
			src
			);
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
inline
Httag&
Httag::removeAttrib( En_Attrib attr )
{
	assert( attr != AT_DUMMY );

	if( _tagIsOpen ) // because if it is open, then we can't remove it!
		p_error( priv::ET_NonFatal,
		"asking to remove attribute on tag <"
		+ getString( _tag_en )
		+ "> but tag is already open"
		);
	else
	{
		auto it = _attr_map.find(attr);
		if( it == _attr_map.end() )   // check if element is already present or not
		{
			p_error( priv::ET_Warning,
				"asked to remove attribute "
				+ getString( attr )
				+ std::string( " of tag <")
				+ getString( _tag_en )
				+ std::string( "> but attribute not present" )
			);
		}
		else
			_attr_map.erase(attr);
	}
	return *this;
}
//-----------------------------------------------------------------------------------
/// Returns a string holding the attributes
inline
std::string
Httag::p_getAttribs() const
{
	const auto& gattr = p_globalAttrib();
	const priv::AttribMap_t* gpatm = 0;
	if( gattr.count(_tag_en) )              // check if there is a global attribute for that tag
		gpatm = &gattr.at(_tag_en);
	std::set<En_Attrib> flags;

	std::string out;
	if( _attr_map.size() )
	{
		for( auto it = _attr_map.begin(); it != _attr_map.end(); ++it )
		{
			out += ' ';
			out += getString( it->first );
			if( !priv::isBoolAttr( it->first) )
			{
				out += "=\"" + it->second;
				if( gpatm )
				{                                          // IF we found a global attribute map for that tag
					if( gpatm->count( it->first ) ) // if that attribute is found in global map for that tag
					{
						flags.insert( it->first );
						out += ' ' + gpatm->at( it->first );
					}
				}
				out += '"';
			}
		}
	}
	else // if no local attributes, then check for global and and the
	{    // ones that have not been added now (this is stored in "flags").
		if( gpatm )
		{
			for( const auto& at: *gpatm )
			{
				if( !flags.count( at.first) )
					out += ' ' + getString( at.first ) + "=\"" + at.second + '\"';
			}
		}
	}

	return out;
}
//-----------------------------------------------------------------------------------
/// Add a linefeed, either if requested (argument), either if default behaviour
inline
bool
Httag::p_doLineFeed( bool linefeed ) const
{
	bool doIt = false;
	switch( p_getRunTimeOptions()._lineFeedMode )
	{
		case rto::LF_Always: doIt = true; break;
		case rto::LF_None: break;
		case rto::LF_Default: doIt = priv::hasDefaultLF_Close( _tag_en ) | linefeed; break;
		default: assert(0);
	}
	return doIt;
}

//-----------------------------------------------------------------------------------
/// Stream tag into file, const version
inline
std::ostream&
operator << ( std::ostream& s, const Httag& h )
{
	return priv::streamTagInFile( s, h );
}

/// Stream tag into file, non-const version
///  \todo test to make sure it gets closed !
inline
std::ostream&
operator << ( std::ostream& s, Httag& h )
{
	priv::streamTagInFile( s, h );

	if( h.p_doLineFeed() )
		s << '\n';

	const auto& rtopt = h.p_getRunTimeOptions();
	if( rtopt._clearContentOnClose )      // if option set, clear content
		h.clearContent();
	if( rtopt._clearAttribOnClose )       // if option set, clear content
		h.clearAttribs();

	return s;
}

//-----------------------------------------------------------------------------------
//############################
namespace priv {
//############################

#ifndef HTTAG_NO_REFERENCE_TABLES
/// Helper function for p_printTable_1()
void
print_A( std::ostream& f, const AllowedContent& ac )
{
	Httag td( f, HT_TD );

	for( auto e: ac._v_allowedCats )
	{
		Httag ta( HT_A, getString( e ), AT_HREF, std::string("#c_") + getString( e ) );
		td << ta << "-";
	}
	f << td;

	for( auto e: ac._v_allowedTags )
	{
		Httag ta( HT_A, getString( e ), AT_HREF, std::string("#t_") + getString( e ) );
		td << ta << "-";
	}
	f << td;

	for( auto e: ac._v_forbiddenCats )
	{
		Httag ta( HT_A, getString( e ), AT_HREF, std::string("#c_") + getString( e ) );
		td << ta << "-";
	}
	f << td;

	for( auto e: ac._v_forbiddenTags )
	{
		Httag ta( HT_A, getString( e ), AT_HREF, std::string("#t_") + getString( e ) );
		td << ta << "-";
	}
	f << td;
}
//-----------------------------------------------------------------------------------
/// Prints HTML table of tag
/** Helper function, called by Httag::printSupportedHtml() */
void
p_printTable_1( std::ostream& f, int id )
{
	Httag table( f, HT_TABLE, AT_ID, "t" + std::to_string(id+1) );
	table.openTag();
	Httag tr( f, HT_TR );

	tr  << Httag( HT_TH,                       AT_ROWSPAN, 2 ).addAttrib( AT_CLASS, "col1" )
		<< Httag( HT_TH, "Tags",               AT_ROWSPAN, 2 ).addAttrib( AT_CLASS, "col2" )
		<< Httag( HT_TH, "Is void",            AT_ROWSPAN, 2 ).addAttrib( AT_CLASS, "col3" )
		<< Httag( HT_TH, "Belongs to",         AT_ROWSPAN, 2 ).addAttrib( AT_CLASS, "col4" )
		<< Httag( HT_TH, "Allowed content",    AT_COLSPAN, 2 ).addAttrib( AT_CLASS, "col5" )
		<< Httag( HT_TH, "Forbidden content",  AT_COLSPAN, 2 ).addAttrib( AT_CLASS, "col6" )
		<< Httag( HT_TH, "Allowed attributes", AT_ROWSPAN, 2 ).addAttrib( AT_CLASS, "col7" );
	tr.printTag();

	tr  << Httag( HT_TH, "Tag category", AT_CLASS, "col5a" )
		<< Httag( HT_TH, "Tags",         AT_CLASS, "col5b" )
		<< Httag( HT_TH, "Tag category", AT_CLASS, "col6a" )
		<< Httag( HT_TH, "Tags",         AT_CLASS, "col6b" );
	tr.printTag();

	std::string ext{"https://www.w3schools.com/tags/tag_"};

	for( size_t i=0; i<HT_DUMMY; i++ )
	{
		Httag tr( f, HT_TR );
		tr.openTag();
		auto tag = static_cast<En_Httag>(i);

		f << Httag( HT_TD, i+1, AT_CLASS, "cent" );

		Httag a( HT_A, g_lt+getString( tag )+g_gt );
		a.addAttrib( AT_HREF, ext + getString( tag ) + ".asp" ).addAttrib( AT_TARGET, "_blank" );
		Httag td2( HT_TD, a, AT_CLASS, "cent" );
		td2.addAttrib( AT_ID, std::string("t_") + getString( tag ) );
		f << td2;

		f << Httag( HT_TD, ( isVoidElement(tag) ? "Y" : "N" ), AT_CLASS, "cent" );

		Httag td( f, HT_TD );
		td.openTag();                                 // column: "belongs to category"
		for( size_t j=0; j<priv::C_DUMMY; j++)
		{
			auto cat = static_cast<priv::En_TagCat>(j);
			if( tagBelongsToCat( tag, cat ) )
			{
				Httag ta( HT_A, getString( cat ), AT_HREF, std::string("#c_") + getString( cat ) );
				f << ta << ',';
			}

		}
		td.closeTag();

	// column "allowed content"
		const auto& acm = Httag::p_getAllowedContentMap();
		print_A( f, acm.get( tag ) );

	// column: "allowed attributes"
		td.openTag();

		size_t nb = 0;                                    // number of attributes allowed in this tag
		for( size_t j=0; j<AT_DUMMY; j++ )
		{
			auto attrib = static_cast<En_Attrib>(j);
			nb += (
				( priv::attribIsAllowed( attrib, tag ) &&  !priv::isGlobalAttr( attrib ) )
				? 1
				: 0
			);
		}
		if( nb )
			f << nb << ": ";

		for( size_t j=0; j<AT_DUMMY; j++)
		{
			auto attrib = static_cast<En_Attrib>(j);
			if( priv::attribIsAllowed( attrib, tag ) && !priv::isGlobalAttr( attrib ) )
			{
				Httag ta( HT_A, getString( attrib ), AT_HREF, std::string("#a_") + getString( attrib ) );
				f << ta << ',';
			}
		}
	}
}
//-----------------------------------------------------------------------------------
/// Prints HTML table of attributes
/** Helper function, called by Httag::printSupportedHtml() */
void
p_printTable_2( std::ostream& f, int id )
{
	Httag table( f, HT_TABLE, AT_ID, "t" + std::to_string(id+1) );
	table.openTag();
	{
		Httag tr( f, HT_TR );
		tr <<  Httag( HT_TH,                 AT_CLASS, "col1" )
			<< Httag( HT_TH, "Attributes",   AT_CLASS, "col2" )
			<< Httag( HT_TH, "Global",       AT_CLASS, "col3" )
			<< Httag( HT_TH, "Boolean",      AT_CLASS, "col4" )
			<< Httag( HT_TH, "Allowed tags", AT_CLASS, "col5" );
		tr.printTag();
	}

	std::string ext{"https://www.w3schools.com/tags/att_"};
	for( size_t i=0; i<AT_DUMMY; i++ )
	{
		Httag tr( f, HT_TR );
		tr.openTag();
		auto attrib = static_cast<En_Attrib>(i);

		f << Httag( HT_TD, i+1, AT_CLASS, "cent" );

		Httag a( HT_A, getString( attrib ) );
		a.addAttrib( AT_HREF, ext + getString( attrib ) + ".asp" ).addAttrib( AT_TARGET, "_blank" );
		f << Httag( HT_TD, a, AT_ID, std::string("a_") + getString( attrib ) );

		if( isGlobalAttr( attrib ) )
			f << Httag( HT_TD, "Y", AT_CLASS, "cent" ) << Httag( HT_TD ) << Httag( HT_TD );
		else
		{
			f << Httag( HT_TD, "N", AT_CLASS, "cent" );
			f << Httag( HT_TD, (isBoolAttr(attrib)?"Y":"N"), AT_CLASS, "cent" );
			Httag td( f, HT_TD );
			td.openTag();

			size_t nb = 0;                                    // number of tags in which attribute is allowed
			for( size_t j=0; j<HT_DUMMY; j++ )
				nb += ( priv::attribIsAllowed( attrib, static_cast<En_Httag>(j) ) ? 1 : 0 );
			f << nb << ": ";

			for( size_t j=0; j<HT_DUMMY; j++ )
			{
				auto tag = static_cast<En_Httag>(j);
				if( priv::attribIsAllowed( attrib, tag ) )
					f << Httag( HT_A, g_lt+getString( tag )+g_gt, AT_HREF, std::string("#t_") + getString( tag ) );
			}
		}
	}
}
//-----------------------------------------------------------------------------------
/// Prints HTML table of tag categories
/** Helper function, called by Httag::printSupportedHtml() */
void
p_printTable_3( std::ostream& f, int id )
{
	Httag table( f, HT_TABLE, AT_ID, "t" + std::to_string(id+1) );
	table.openTag();
	{
		Httag tr( f, HT_TR );
		tr <<  Httag( HT_TH,                       AT_CLASS, "col1" )
			<< Httag( HT_TH, "Tag category",       AT_CLASS, "col2" )
			<< Httag( HT_TH, "Corresponding tags", AT_CLASS, "col3" );
		tr.printTag();
	}
	for( size_t i=0; i<priv::C_DUMMY; i++ )
	{
		Httag tr( f, HT_TR );
		tr.openTag();
		auto cat = static_cast<En_TagCat>(i);
		f << Httag( HT_TD, i+1, AT_CLASS, "cent" )
			<< Httag( HT_TD, getString( cat ), AT_ID, std::string("c_") + getString( cat ) );

		Httag td( f, HT_TD );
		td.openTag();
		for( size_t j=0; j<HT_DUMMY; j++ )
		{
			auto tag = static_cast<En_Httag>(j);
			if( tagBelongsToCat( tag, cat ) )
			{
				Httag ta( HT_A, g_lt+getString( tag )+g_gt, AT_HREF, std::string("#t_") + getString( tag ) );
				f << ta << ',';
			}
		}
	}
}
//-----------------------------------------------------------------------------------
/// Prints HTML table of tag categories
/** Helper function, called by Httag::printSupportedHtml() */
void
p_printTable_4( std::ostream& f, int id )
{
	std::vector<std::vector<En_Httag>> tagSet( priv::C_DUMMY ); // one vector per category

	Httag table( f, HT_TABLE, AT_ID, "t" + std::to_string(id+1) );
	table.openTag();

	Httag tr( f, HT_TR );
	tr << Httag( HT_TH, AT_CLASS, "col1" );

	for( size_t i=0; i<priv::C_DUMMY; i++ )    // line header
	{
		auto cat = static_cast<En_TagCat>(i);
//		tr << Httag( HT_TH, std::to_string(i+1) + Httag(HT_BR).to_string() + getString( cat ) );
		tr << Httag( HT_TH, std::to_string(i+1) + Httag(HT_BR) + getString( cat ) );

		for( size_t j=0; j<HT_DUMMY; j++ )                   // build list of contained tags
			if( tagBelongsToCat( static_cast<En_Httag>(j), cat ) )
				tagSet[i].push_back( static_cast<En_Httag>(j) );
	}
	f << tr;

	for( size_t i=0; i<priv::C_DUMMY-1; i++ )                     // iterating lines (no need for last line, thus "-1")
	{
		tr.openTag();
		auto cat1 = static_cast<En_TagCat>(i);
		f << Httag( HT_TH, std::to_string(i+1) + Httag(HT_BR).to_string() + getString( cat1 ) );
		for( size_t j=0; j<priv::C_DUMMY; j++ )                // iterating columns
		{
			if( i < j )
			{
				std::vector<En_Httag> intersect;
				auto it = std::set_intersection(
					std::begin( tagSet[i] ),
					std::end(   tagSet[i] ),
					std::begin( tagSet[j] ),
					std::end(   tagSet[j] ),
					std::back_inserter( intersect )
				);
				Httag td( HT_TD );
				if( !intersect.empty() )
					td << Httag( HT_STRONG, std::string("#") + std::to_string( intersect.size() ) + ":" ) << Httag( HT_BR );
				for( auto tag: intersect )
					td << Httag( HT_A, g_lt + getString( tag ) + g_gt, AT_HREF, "#t_" + getString( tag ) );
				f << td;
			}
			else
				f << Httag( HT_TD, AT_CLASS, "empty" );

		}
		tr.closeTag();
	}
}
//-----------------------------------------------------------------------------------

#endif // HTTAG_NO_REFERENCE_TABLES

//############################
} // namespace priv end
//############################

//-----------------------------------------------------------------------------------
/// Helper function, prints the tags and attributes currently supported, HTML version
/* See related Httag::printSupported()
**/
//inline
template<typename T>
void
Httag::printSupportedHtml( std::ostream& f, T )
{
#ifdef HTTAG_NO_REFERENCE_TABLES
	f << "Error: build without printing of reference tables enabled\n";
#else
	Httag::setOption( rto::ClearOnClose, true );
	f << Httag( HT_DOCTYPE );
	Httag t( f, HT_HTML );
	t.openTag();

	Httag h( f, HT_HEAD);
	h << Httag( HT_TITLE, "cpphtmltags: supported features" );
	Httag css( HT_LINK, AT_HREF, "supported.css" );
	css.addAttrib( AT_REL, "stylesheet" );
	css.addAttrib( AT_TYPE, "text/css" );
	h << css;
	h.printTag();

	Httag t2( f, HT_BODY );
	t2.openTag();
	f << Httag(	HT_P,
		"This list is automatically generated from reference data, see " ).addContent(
			Httag( HT_A, "home page", AT_HREF, "https://github.com/skramm/cpphtmltags/" ).to_string()
		);

	std::ifstream external( "misc/supported.txt" );
	f << external.rdbuf();

	std::array<std::string,4> titles = {
		"Supported tags and categories",
		"Attribute list",
		"Tag categories",
		"Cross reference of tag categories: common tags"
	};

	std::array<std::function<void(std::ostream&,int)>,4> funcs = {
		priv::p_printTable_1,
		priv::p_printTable_2,
		priv::p_printTable_3,
		priv::p_printTable_4
	};

	Httag ol( f, HT_OL );
	ol.openTag();
	Httag li( f, HT_LI );

	for( int i=0; i<4; i++ )
	{
		li << Httag( HT_A, titles[i], AT_HREF, "#h" + std::to_string(i+1) );
		li.printTag();
	}
	ol.closeTag();

	for( int i=0; i<4; i++ )
	{
		f << Httag( HT_H2, std::to_string(i+1) + " - " + titles[i], AT_ID, "h" + std::to_string(i+1) );
		funcs[i]( f, i );
		std::cout << std::endl; // flush (TMP)
	}

	auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    f << Httag( HT_P, "Generated on " + std::string(ctime(&timenow)) );

#endif  // HTTAG_NO_REFERENCE_TABLES
}
//-----------------------------------------------------------------------------------


//############################
} // namespace httag end
//############################

#endif // HG_CPPHTMLTAGS_HPP

//--- END: TEMPLATE FILE cpphtmltags_3.hh
