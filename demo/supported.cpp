// This file is part of cpphtmltags

/**
\file
\brief Prints out the supported tags and attributes

When running, if some elements appear as empty, this means that reference material (file ref/tag_content.ref)
*/

#include "../cpphtmltags.hpp"

int main()
{
	httag::Httag::printSupported( std::cout );
}
