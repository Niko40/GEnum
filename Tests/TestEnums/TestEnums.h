
#include <iostream>
#include <stdint.h>

enum BasicEnum
{
	BE_RED,
	BE_GREEN,
	BE_BLUE,
};

enum class EnumClass
{
	EC_RED,
	EC_GREEN,
	EC_BLUE,
};

enum EnumWithUnderlayingType : uint32_t
{
	EWUT_RED,
	EWUT_GREEN,
	EWUT_BLUE,
};

enum class EnumClassWithUnderlayingType : char
{
	ECWUT_RED,
	ECWUT_GREEN,
	ECWUT_BLUE,
};

// Using enum classes from now on

enum class EnumWithAssignedValues
{
	RED		= 1,
	GREEN	= 2,
	BLUE	= 4,
};

enum class SingleLineEnum { RED, GREEN, BLUE };

enum class SingleLineEnumWithAssignedValues{ RED = 8, GREEN = 16, BLUE = 32 };

enum class EnumWithFirstBracketOnFirstLine {
	RED,
	GREEN,
	BLUE,
};

enum class EnumWithLastBracketOnLine
{
	RED,
	GREEN,
	BLUE, };

enum class EnumWithFirstAndLastBracketsOnLine {
	RED,
	GREEN,
	BLUE, };

enum class EnumWithAssignedValuesFromPreviousEntries
{
	RED				= 1,
	GREEN			= 2,
	BLUE			= 4,
	RED_DOUBLE		= RED,
	GREEN_DOUBLE	= GREEN,
	BLUE_DOUBLE		= BLUE,
};

enum class EnumWithFirstAndLastBracketsOnLine
{
	RED,
	GREEN,
	BLUE,
};

// Testing comments, should not see this "enum" here.
enum class EnumWithCommentsAroundIt
{
	RED,
	GREEN,
	BLUE,
};
// comment 2

/*
Testing comments, should not see this "enum" here.
*/
enum class EnumWithCommentBlocksArounIt
{
	RED,
	GREEN,
	BLUE,
};
/*
Testing comment block outside enum
*/

/* Starting comment block.
// /* <- should not effect anything.
// This should close the comment block even though we're in a line comment already -> */

enum class EnumWithCommentsInsideIt
{
	RED,
	// GREEN,
	BLUE,
	// CYAN,
	MAGENTA,
	YELLOW,
	BLACK,
};

enum class EnumWithCommentBlocksInsideIt
{
	RED,
	/* GREEN, */
	BLUE,
	/*
	CYAN,
	MAGENTA,
	*/
	YELLOW,
	BLACK,
};

enum class EnumWithCommentBlocksRangeTestInsideIt
{
	RED,
	GREEN,
	BLUE,
	/* Start comments here
	CYAN,
	// MAGENTA,
	// <- start a new comment on this line. This should also close the comment inside a line comment -> */
	YELLOW,
	BLACK,
};

enum class EnumWithCommentBlocksInValues
{
	RED			= 1,
	GREEN		= 2, /*test*/
	BLUE		= 4 /*test*/,
	CYAN		= /*test*/ 8,
	MAGENTA		= /*test*/16,
	YELLOW		/*test*/ = 32,
	BLACK		/*test*/= 64,
	WHITE		/*test*/
	= 64,
	PINK		/*test*/=
	128,
	LILAC		/*test*/=
	128,
};

namespace ns {
	enum class EnumInsideNamespace
	{
		RED,
		GREEN,
		BLUE,
	};

	namespace ns2 {
		enum class EnumInsideTwoNamespaces
		{
			RED,
			GREEN,
			BLUE,
		};
	}

	enum class EnumInsideNamespace2
	{
		RED,
		GREEN,
		BLUE,
	};
}

enum class EnumAfterNamespaces
{
	RED,
	GREEN,
	BLUE,
};

enum class enum_StartWithLowerCaseEnum
{
	RED,
	GREEN,
	BLUE,
};

enum class Include_enum_InTheMiddle
{
	RED,
	GREEN,
	BLUE,
};

enum class IncludeEnumAtTheEnd_enum
{
	RED,
	GREEN,
	BLUE,
};

enum /*class*/ EnumWithCommentBlockAroundClass
{
	EWCBAC_RED,
	EWCBAC_GREEN,
	EWCBAC_BLUE,
};

enum
	class
	MultilineEnum
{
	RED,
	GREEN,
	BLUE,
}
;

// Nameless enum
enum
{
	NAMELESS1_RED,
	NAMELESS1_GREEN,
	NAMELESS1_BLUE,
};

// Nameless enum with underlaying type
enum : uint32_t
{
	NAMELESS2_RED,
	NAMELESS2_GREEN,
	NAMELESS2_BLUE,
};

// Nameless enum with underlaying type without spaces
enum:uint32_t
{
	NAMELESS3_RED,
	NAMELESS3_GREEN,
	NAMELESS3_BLUE,
};

enum class EnumClassDeclaredOnlyShouldBeIgnored;
enum class EnumClassDeclaredOnlyWithUnderlayingTypeShouldBeIgnored : uint32_t;

enum class CrammedTogetherEnum:uint32_t{RED,GREEN,BLUE};



// Testing character arrays
std::string character_array_1	= "Test string 1";
std::string character_array_2	= "Test string 2 with \"some\"extra";
std::string character_array_3	= "Test string 3 with extra \"enum\" in the middle. Shouldn't see this enum...";
std::string character_array_4	= "Test string 4 with single extra \" in the middle.";
std::string character_array_5	= "Test string 5 which "
"is multiline.";

enum class EnumClassAfterCharacterArrays
{
	RED,
	GREEN,
	BLUE,
};


using namespace std;

enum class EnumClassAfterUsingNamespace
{
	RED,
	GREEN,
	BLUE,
};
