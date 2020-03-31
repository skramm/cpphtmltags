//--- START: TEMPLATE FILE cpphtmltags_2.hh

/// Maps attributes to tags that allow them
using MapAttribs_t = std::map<En_Attrib,std::vector<En_Httag>>;

/// Maps tag categories to tags that belong to it
using MapTagCat_t = std::map<En_TagCat,std::vector<En_Httag>>;

/// See GlobAttribMap_t
using AttribMap_t = std::map<En_Attrib,std::string>;

/// Holds for some tags (that have a global attribute) the pair (attribute, string-value)
/** See \link global_attribute */
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
#if 0
	bool isEmpty() const
	{
#if 1
		if( _isVoid == true )
			return false;
		if( _isText == true )
			return false;
#else
		if( isVoidElement(tt) )
			return false;
#endif

		if( _v_allowedTags.size() )
			return false;
		if( _v_allowedCats.size() )
			return false;
		if( _v_forbiddenTags.size() )
			return false;
		if( _v_forbiddenCats.size() )
			return false;
		return true;
	}
#endif
};

//--- END: TEMPLATE FILE cpphtmltags_2.hh

