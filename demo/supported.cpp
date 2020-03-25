// This file is part of cpphtmltags

/**
\file
\brief Prints out the supported tags and attributes

When running, if some elements appear as empty, this means that reference material (file ref/tag_content.ref)
*/

#include "../cpphtmltags.hpp"

int main( int argc, char** argv )
{
	if( argc>1 && std::string(argv[1]) == "-html" )
		httag::Httag::printSupportedHtml( std::cout );
	else
		httag::Httag::printSupported( std::cout );
}
