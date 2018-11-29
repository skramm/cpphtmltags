//--- START: TEMPLATE FILE cpphtmltags_2.hh

typedef std::map<En_Attrib,std::vector<En_Httag>> MapAttribs_t;
typedef std::map<En_TagCat,std::vector<En_Httag>> TagCat_t;

typedef std::pair<En_Attrib,std::string>      PairAttribString_t;
typedef std::map<En_Httag, PairAttribString_t> GlobAttribMap_t;

typedef std::pair<std::vector<En_TagCat>,std::vector<En_Httag>> TagContent_t;

typedef std::pair<const En_Httag,std::vector<En_Httag>>  TagAllowedTag_t;
typedef std::pair<const En_Httag,std::vector<En_TagCat>> TagAllowedCat_t;

typedef std::map<En_Httag,std::vector<En_TagCat>> TagContentC_t;
typedef std::map<En_Httag,std::vector<En_Httag>>  TagContentT_t;

//--- END: TEMPLATE FILE cpphtmltags_2.hh
