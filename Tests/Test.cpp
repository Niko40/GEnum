
#include "../Source/GEnum.h"

// Capabilities:
// - Understands comments and character arrays.
// - Handles namespaces and can also include it in the returned string if desired.

// Limitations:
// - Nameless enums ignored. Limitation of C/C++ in this approach.
// - Enum fields with same value ignored. Limitation of C/C++ in this approach.
// - Does not handle enums that have same name.

// TODOs:
// - Comments and character arrays should not be handled separately.
//   Comments in character arrays should be part of the character array,
//   Likewise character arrays inside comments should be part of comments.
//   Basically just merge these things together.



int main( int argc, char * argv[] )
{
	Options options {};
	ParseArguments(
		options,
		argc,
		argv
	);

	// For testing only.
	options.parse_folder	= TEST_FOLDER;
	options.debug_prints	= true;
	options.generate		= true;

	PrintOptions( options );

	return GenerateEnumNames( options );
}
