
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <stdint.h>

namespace fs = std::filesystem;

struct Options
{
	fs::path						exe_path;
	fs::path						parse_folder;
	fs::path						header_output;
	fs::path						source_output;

	std::string						return_prefix;
	std::string						return_postfix;
	std::string						return_type					= "std::string_view";
	std::string						include_file				= "<string_view>";
	std::string						function_name				= "EnumToString";

	bool							output_enum_name			= false;
	bool							output_namespace_name		= false;
	bool							debug_prints				= false;
	bool							generate					= false;
};

struct InputInfo
{
	fs::path						file_path;
	std::string						str;
	std::vector<bool>				valid;
	size_t							cursor;
};
struct OutputInfo
{
	fs::path						header_path;
	fs::path						source_path;
	std::shared_ptr<std::ofstream>	header;
	std::shared_ptr<std::ofstream>	source;
};

struct EnumInfo
{
	std::string		name;
	std::string		underlaying_type;
	bool			is_class				= false;
	bool			is_declared_only		= false;
};
struct EnumFieldInfo
{
	std::string		name;
	std::string		value;
	bool			disabled;
};

struct FindResult
{
	size_t			begin					= SIZE_MAX;
	size_t			end						= SIZE_MAX;
	size_t			size					= SIZE_MAX;
	size_t			option					= SIZE_MAX;
	std::string		word;
};

// Primary function that handles everything, returns error code
int GenerateEnumNames(
	const Options	&	options
);



// These are internal functions and listings

// If called, prints out an error and exits the program.
void PrintError(
	std::string_view	msg
);

// Print help text on console.
void PrintHelp(
	const Options	&	options
);

// Print selected options on console.
void PrintOptions(
	const Options	&	options
);

// Parse arguments into options.
void ParseArguments(
	Options			&	options,	// in-out
	int					argc,
	char			*	argv[]
);



OutputInfo InitOutputFiles(
	const Options	&	options
);

void FinalizeOutputFiles(
	const Options	&	options,
	OutputInfo		&	output_info
);



// Parses single file from path.
void ParseFile(
	const Options	&	options,
	fs::path			path,
	OutputInfo		&	output_info
);

void ValidateFileCharacters(
	const Options		&	options,
	InputInfo			&	input_info
);

// Process enums
void ProcessEnums(
	const Options		&	options,
	InputInfo			&	input_info,
	OutputInfo			&	output_info
);

void ProcessSingleEnum(
	const Options										&	options,
	const std::vector<std::pair<size_t, std::string>>	&	namespace_stack,
	InputInfo											&	input_info,
	OutputInfo											&	output_info
);

void CreateEnumConverterFunction(
	const Options										&	options,
	const std::vector<std::pair<size_t, std::string>>	&	namespace_stack,
	const EnumInfo										&	enum_info,
	const std::vector<EnumFieldInfo>					&	enum_field_info,
	OutputInfo											&	output_info
);

// Find first occurance of text from a list of possibilities
FindResult FindFirstOf(
	const std::string					&	str,
	const std::vector<bool>				&	valid,
	size_t									position,
	const std::vector<std::string_view>	&	possibilities
);

FindResult FindNextNonWhitespaceCharacter(
	const std::string					&	str,
	const std::vector<bool>				&	valid,
	size_t									position
);

FindResult FindNextNonWhitespaceString(
	const  std::string					&	str,
	const std::vector<bool>				&	valid,
	size_t									position
);

FindResult FindNextCName(
	const std::string					&	str,
	const std::vector<bool>				&	valid,
	size_t									position
);

bool IsValidCNameCharacter(
	int		character,
	bool	is_first_character_in_name
);
