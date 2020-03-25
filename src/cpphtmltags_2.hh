//--- START: TEMPLATE FILE cpphtmltags_2.hh

/// Maps attributes to tags that allow them
typedef std::map<En_Attrib,std::vector<En_Httag>> MapAttribs_t;

/// Maps tag categories to tags that belong to it
typedef std::map<En_TagCat,std::vector<En_Httag>> MapTagCat_t;

/// See GlobAttribMap_t
typedef std::pair<En_Attrib,std::string>       PairAttribString_t;

/// Holds for some tags (that have a global attribute) the pair (attribute, string-value)
/** See \link global_attribute */
typedef std::map<En_Httag, PairAttribString_t> GlobAttribMap_t;


/// holds for a given tag what content is allowed inside.
/// See related type AllowedContentMap
struct AllowedContent
{
	std::vector<En_Httag>  _v_allowedTags;
	std::vector<En_Httag>  _v_forbiddenTags;
	std::vector<En_TagCat> _v_allowedCats;
	std::vector<En_TagCat> _v_forbiddenCats;
	bool _isVoid = false;
	bool _isText = false;

/// Type of tag
	enum En_TagType{ TT_NEITHER, TT_VOID, TT_TEXT };

	/// Constructor
	AllowedContent( En_TagType tt=TT_NEITHER )
	{
		if( tt == TT_VOID )
			_isVoid = true;
		if( tt == TT_TEXT )
			_isText = true;
	}
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
	bool isEmpty() const
	{
		if( _isVoid == true )
			return false;
		if( _isText == true )
			return false;
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
};

//--- END: TEMPLATE FILE cpphtmltags_2.hh

