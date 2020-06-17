
#include "GEnum.h"

int main( int argc, char * argv[] )
{
	Options options {};

	ParseArguments( options, argc, argv );

	return GenerateEnumNames( options );
}
