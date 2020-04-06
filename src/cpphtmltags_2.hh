//--- START: TEMPLATE FILE cpphtmltags_2.hh


// current namespace: httag::priv

/// Maps attributes to tags that allow them
using MapAttribs_t = std::map<En_Attrib,std::vector<En_Httag>>;

/// Maps tag categories to tags that belong to it
using MapTagCat_t = std::map<En_TagCat,std::vector<En_Httag>>;

/// See GlobAttribMap_t
using AttribMap_t = std::map<En_Attrib,std::string>;

/// Holds for some tags (that have a global attribute) the pair (attribute, string-value)
/** See \ref global_attribute */
using GlobAttribMap_t = std::map<En_Httag, AttribMap_t>;


/// Holds for a given tag what content is allowed inside.
/// See related type AllowedContentMap
struct AllowedContent
{
	std::vector<En_Httag>  _v_allowedTags;
	std::vector<En_Httag>  _v_forbiddenTags;
	std::vector<En_TagCat> _v_allowedCats;
	std::vector<En_TagCat> _v_forbiddenCats;

	void addForbiddenTag( En_Httag en )
	{
		_v_forbiddenTags.push_back( en );
	}
	void addForbiddenCat( En_TagCat en )
	{
		_v_forbiddenCats.push_back( en );
	}
	void addTag( En_Httag en )
	{
		_v_allowedTags.push_back( en );
	}
	void addCat( En_TagCat en )
	{
		_v_allowedCats.push_back( en );
	}
};

/// Error type, used internally, see httag::Httag::p_error()
enum En_ErrorType
{
	ET_NonFatal,
	ET_Fatal,
	ET_Warning
};

inline
std::string
getString( En_ErrorType et )
{
	std::string s;
	switch( et )
	{
		case ET_NonFatal: s="Error";       break;
		case ET_Fatal:    s="Fatal error"; break;
		case ET_Warning:  s="Warning";     break;
		default: assert(0);
	}
	return s;
}

//--- END: TEMPLATE FILE cpphtmltags_2.hh

