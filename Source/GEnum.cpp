
#include "GEnum.h"
#include <utility>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

int GenerateEnumNames(
	const Options	&	options
)
{
	if( options.generate ) {
		auto output_info = InitOutputFiles(
			options
		);

		for( auto & file : fs::recursive_directory_iterator( options.parse_folder ) ) {
			if( file.is_regular_file() && file.path().extension() == ".h" ) {
				ParseFile( options, file.path(), output_info );
			}
		}

		FinalizeOutputFiles(
			options,
			output_info
		);
	}

	return 0;
}

void PrintError(
	std::string_view			msg
)
{
	#ifdef _WIN32
	HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	GetConsoleScreenBufferInfo( console, &console_info );
	WORD original = console_info.wAttributes;

	SetConsoleTextAttribute( console, 4 << 4 | 15 );
	std::cout << msg;
	SetConsoleTextAttribute( console, original );
	std::cout << std::endl;
	#else

	std::cout << msg << std::endl;
	#endif

	exit( 1 );
}

void PrintHelp(
	const Options	&	options
)
{
	std::cout
		<< "  Description:\n"
		<< "    This tool parses and generates enumerator to string converter functions.\n"
		<< "  \n"
		<< "  Usage:\n"
		<< "    GenerateEnumNames [-option] [<param>] \n"
		<< "    GenerateEnumNames -help\n"
		<< "  \n"
		<< "  Options:\n"
		<< "    -help           This help.\n"
		<< "    -G <path>       Generate from source path, should point to the root folder of\n"
		<< "                      the source.\n"
		<< "    -H <path>       Set destination path, this is the final output where the\n"
		<< "                      header file will be saved to.\n"
		<< "                      Default = \"" << options.header_output.string() << "\".\n"
		<< "    -C <path>       Set destination path, this is the final output where the\n"
		<< "                      source file will be saved to.\n"
		<< "                      Default = \"" << options.source_output.string() << "\".\n"
		<< "    -P <prefix>     Prefix for the function return type, useful for libraries.\n"
		<< "                      -> [prefix] <ret type> [postfix] <funcname>(...)\n"
		<< "                      Default = \"" << options.return_prefix << "\".\n"
		<< "    -O <postfix>    Postfix for the function return type, useful for libraries\n"
		<< "                      -> [prefix] <ret type> [postfix] <funcname>(...)\n"
		<< "                      Default = \"" << options.return_postfix << "\".\n"
		<< "    -R <ret type>   Return type of the converter function.\n"
		<< "                      -> [prefix] <ret type> [postfix] <funcname>(...)\n"
		<< "                      Default = \"" << options.return_type << "\".\n"
		<< "                      Must be convertible from \"char*\".\n"
		<< "    -F <funcname>   Name of the converter function.\n"
		<< "                      -> [prefix] <ret type> [postfix] <funcname>(...)\n"
		<< "                      Default = \"" << options.function_name << "\".\n"
		<< "    -I <include>    Include header file, for return type. Must include\n"
		<< "                      encapsulation, either <> or \"\".\n"
		<< "                      Default = \"" << options.include_file << "\".\n"
		<< "    -E              Include enum names in the returned string.\n"
		<< "                      -> \"[enum name]::<field name>\"\n"
		<< "    -N              Include namespace names in the returned string.\n"
		<< "                      -> \"[namespace name]::<field name>\"\n"
		<< "    -D              Display debug information, might help in case of errors.\n"
		<< "                      Default = \"" << options.debug_prints << "\".\n"
		<< std::endl;
}

void PrintOptions(
	const Options	&	options
)
{
	std::cout
		<< "GenerateEnumNames selected options: \n"
		<< "Parse folder:    " << options.parse_folder << "\n"
		<< "Header output:   " << options.header_output << "\n"
		<< "Source output:   " << options.source_output << "\n"
		<< "Return prefix:   " << options.return_prefix << "\n"
		<< "Return postfix:  " << options.return_postfix << "\n"
		<< "Return type:     " << options.return_type << "\n"
		<< "function name:   " << options.function_name << "\n"
		<< "Generate:        " << options.generate << "\n"
		<< std::endl;
}

void ParseArguments(
	Options			&	options,
	int					argc,
	char			*	argv[]
)
{
	options.header_output	= argv[ 0 ];
	options.header_output.replace_filename( "ToString" );
	options.header_output.replace_extension( ".h" );
	options.source_output	= options.header_output;
	options.source_output.replace_extension( ".cpp" );

	if( argc < 2 ) {
		PrintHelp( options );
		return;
	}

	std::vector<std::string> args( argc - 1 );
	for( int i = 1; i < argc; ++i ) {
		args[ i - 1 ] = argv[ i ];
	}
	auto NextParam = [ &args, &options ]( std::vector<std::string>::iterator & it ) -> std::string
	{
		if( ++it == args.end() ) {
			PrintHelp( options );
			PrintError( "Required parameter not provided. Please see above." );
		}
		return *it;
	};

	auto it = args.begin();
	while( it != args.end() ) {
		std::string arg			= *it;
		std::string arg_lower	= *it;
		std::transform( arg_lower.begin(), arg_lower.end(), arg_lower.begin(), ::tolower );
		if( arg_lower == "-help" ) {
			PrintHelp( options );
			exit( 0 );
		}
		if( arg == "-G" ) {
			options.generate = true;
			options.parse_folder = NextParam( it );
		}
		if( arg == "-H" ) {
			options.header_output = NextParam( it );
		}
		if( arg == "-C" ) {
			options.source_output = NextParam( it );
		}
		if( arg == "-P" ) {
			options.return_prefix = NextParam( it );
		}
		if( arg == "-O" ) {
			options.return_postfix = NextParam( it );
		}
		if( arg == "-R" ) {
			options.return_type = NextParam( it );
		}
		if( arg == "-F" ) {
			options.function_name = NextParam( it );
		}
		if( arg == "-E" ) {
			options.output_enum_name = true;
		}
		if( arg == "-N" ) {
			options.output_namespace_name = true;
		}
		if( arg == "-D" ) {
			options.debug_prints = true;
		}
		++it;
	}
}



OutputInfo InitOutputFiles(
	const Options	&	options
)
{
	OutputInfo output_info;

	output_info.header_path		= options.header_output;
	output_info.header			= std::make_shared<std::ofstream>( options.header_output, std::ofstream::beg | std::ofstream::trunc );
	output_info.source_path		= options.source_output;
	output_info.source			= std::make_shared<std::ofstream>( options.source_output, std::ofstream::beg | std::ofstream::trunc );

	if( !output_info.header->good() || !output_info.header->is_open() ) PrintError( "Cannot create/open output header file." );
	if( !output_info.source->good() || !output_info.source->is_open() ) PrintError( "Cannot create/open output source file." );

	auto & h = *output_info.header;
	auto & c = *output_info.source;

	auto WriteFileTop = [ &options ]( std::ofstream & stream )
	{
		stream
			<< "\n\n"
			<< "///////////////////////////////////////////////////////////////////////////////\n"
			<< "// !! This file was automatically generated, please don't modify manually !! //\n"
			<< "///////////////////////////////////////////////////////////////////////////////\n"
			<< "// \n"
			<< "// Generated by GEnum.\n"
			<< "// More info can be found here: TODO GITHUB PAGE HERE! \n"
			<< "\n";

		if( !options.include_file.empty() ) {
			stream
				<< "#include " << options.include_file << "\n\n";
		}
	};

	WriteFileTop( h );
	WriteFileTop( c );

	return output_info;
}

void FinalizeOutputFiles(
	const Options	&	options,
	OutputInfo		&	output_info
)
{
	auto & header = *output_info.header;
	auto & source = *output_info.source;

	header
		<< "\n"
		<< "// Generator finished successfully.\n";

	source
		<< "\n"
		<< "// Generator finished successfully.\n";
}



void ParseFile(
	const Options	&	options,
	fs::path			path,
	OutputInfo		&	output_info
)
{
	if( options.debug_prints ) {
		std::cout << "Parsing file: " << path << "\n";
	}

	// Read entire file into a string
	std::string file_str;
	{
		auto file_stream = std::ifstream( path, std::ifstream::ate );
		if( !file_stream.good() ) PrintError( "Internal error. A file seemed to have disappeared mid-processing, this should never happen." );
		size_t file_size = file_stream.tellg();
		file_str.reserve( file_size );
		file_stream.seekg( 0 );
		std::string buffer;
		while( std::getline( file_stream, buffer ) )
		{
			file_str += buffer;
			file_str += "\n";	// replacing OS newlines with common newlines
		}
	}

	RemoveComments(
		options,
		file_str
	);

	RemoveCharacterArrays(
		options,
		file_str
	);

	ProcessEnums(
		options,
		file_str,
		output_info
	);
}



void RemoveComments(
	const Options	&	options,
	std::string		&	str
)
{
	if( options.debug_prints ) {
		std::cout << "Processing comments: \n";
	}

	std::vector<std::string_view> comment_begins {
		"//",	// case 0:	// Regular comment begin.
		"/*",	// case 1:	// comment block begin.
	};
	std::vector<std::string_view> newline {
		"\n",
	};
	std::vector<std::string_view> comment_block_end {
		"*/",
	};

	size_t last_search_location = 0;
	while( true ) {
		auto result = FindFirstOf(
			str,
			last_search_location,
			comment_begins
		);
		if( result.word == "" ) return; // All comments should now be gone.
		last_search_location = result.begin;
		switch( result.option ) {
			case 0:
			{
				// Regular comment, look for a newline
				auto r = FindFirstOf( str, result.begin, newline );

				// We always end with a newline so we should never hit this.
				if( r.begin == SIZE_MAX ) PrintError( "Error parsing comments. Internal error." );

				auto cut_range_begin	= result.begin;
				auto cut_range_size		= r.end - result.begin;
				str.erase( cut_range_begin, cut_range_size );
			}
			break;
			case 1:
			{
				// Comment block, look for comment block end
				auto r = FindFirstOf( str, result.begin, comment_block_end );

				// If we don't end up with a comment block end, then the source has an error.
				if( r.begin == SIZE_MAX ) PrintError( "Error parsing comments. Comment block end missing, make sure the source compiles first." );

				auto cut_range_begin	= result.begin;
				auto cut_range_size		= r.end - result.begin;
				str.erase( cut_range_begin, cut_range_size );
			}
			break;
			default:
				PrintError( "Internal error while parsing comments." );
		}
	}
}

void RemoveCharacterArrays(
	const Options	&	options,
	std::string		&	str
)
{
	if( options.debug_prints ) {
		std::cout << "Processing character arrays: \n";
	}

	std::vector<std::string_view> character_array_markings {
		"\"",	// case 0:	// "
		"\\\"",	// case 1:	// \"
	};

	size_t last_search_location					= 0;
	bool character_array_open					= false;
	size_t character_array_opening_location		= 0;
	while( true ) {
		auto result = FindFirstOf(
			str,
			last_search_location,
			character_array_markings
		);
		if( result.word == "" ) break; // All character arrays should now be gone.
		last_search_location = result.begin;
		switch( result.option ) {
			case 0:
			{
				if( character_array_open ) {
					// Closing character array
					character_array_open				= false;
					auto cut_range_begin				= character_array_opening_location;
					auto cut_range_size					= result.end - cut_range_begin;
					str.erase( cut_range_begin, cut_range_size );
					last_search_location				= character_array_opening_location;
				} else {
					// Opening character array
					character_array_open				= true;
					character_array_opening_location	= last_search_location;
					last_search_location				+= 1;
				}
			}
			break;
			case 1:
			{
				// Ignore -> \"
				last_search_location	+= 2;
			}
			break;
			default:
				PrintError( "Internal error while parsing character arrays." );
		}
	}
	if( character_array_open ) {
		// Parsing error or bad file.
		PrintError( "No matching <\"> found for character array." );
	}
}



void ProcessEnums(
	const Options		&	options,
	const std::string	&	str,
	OutputInfo			&	output_info
)
{
	if( options.debug_prints ) {
		std::cout << "Processing enums.\n\n";
	}

	std::vector<std::string_view> keywords {
		"enum",			// case 0:
		"namespace",	// case 1:
		"{",			// case 2:
		"}",			// case 3:
	};
	std::vector<std::string_view> expected_after_namespace {
		"{",
		";",
	};

	std::vector<std::pair<size_t, std::string>>	namespace_stack;
	size_t										read_pos					= 0;
	size_t										current_bracket_level		= 0;

	while( true ) {
		auto result = FindFirstOf(
			str,
			read_pos,
			keywords
		);
		read_pos = result.end;
		if( result.word == "" ) break; // Done.
		switch( result.option ) {
			case 0:	// enum
			{
				// Test both the front and back of "enum" is not a valid C name character,
				// if either of them are, then this "enum" is part of a name for something else.
				if( ( result.begin == 0 || !IsValidCNameCharacter( str[ result.begin - 1 ], false ) ) &&
					!IsValidCNameCharacter( str[ result.end ], false ) ) {

					ProcessSingleEnum(
						options,
						namespace_stack,
						read_pos,
						str,
						output_info
					);
				}
			}
				break;
			case 1:	// namespace
			{
				auto r_name = FindNextCName(
					str,
					read_pos
				);
				auto r_term = FindFirstOf(
					str,
					read_pos,
					expected_after_namespace
				);

				// Check we actually still have file left to parse.
				if( r_name.word == "" ) PrintError( "\"namespace\" had no name." );
				if( r_term.word == "" ) PrintError( "Expected either \"{\" or \";\" after \"namespace\"." );

				// If terminating character was before name
				if( r_term.begin < r_name.begin ) PrintError( "Expected a namespace name before \"{\" or \";\"." );

				if( r_term.word == "{" ) {
					// Opening new namespace, add namespace level
					++current_bracket_level;
					namespace_stack.emplace_back( current_bracket_level, r_name.word );
					read_pos	= r_term.end;
				} else if( r_term.word == ";" ) {
					// Just "using namespace <name>;", doesn't concern us.
					read_pos	= r_term.end;
				} else {
					PrintError( "Namespace \"" + r_name.word + "\": expected \"{\" or \";\" after name." );
				}
			}
				break;
			case 2:	// {
			{
				++current_bracket_level;
			}
				break;
			case 3:	// }
			{
				if( current_bracket_level == 0 ) PrintError( "Bracket level mismatch." );
				if( !namespace_stack.empty() ) {
					auto p = namespace_stack.back();
					if( p.first == current_bracket_level ) {
						namespace_stack.pop_back();
					}
				}
				--current_bracket_level;
			}
				break;
			default:
				break;
		}
	}
	if( current_bracket_level != 0 ) PrintError( "Bracket level mismatch." );
}

void ProcessSingleEnum(
	const Options										&	options,
	const std::vector<std::pair<size_t, std::string>>	&	namespace_stack,
	size_t												&	position,
	const std::string									&	str,
	OutputInfo											&	output_info
)
{
	EnumInfo enum_info;

	// Collect information about the enum
	{
		std::vector<std::string_view> enum_start_symbols {
			"class",	// case 0:	this is an enum class
			":",		// case 1:	underlaying type next
			"{",		// case 2:	enum starts
			";",		// case 3:	this was a declaration only
		};

		bool enum_body_entered = false;
		while( !enum_body_entered ) {
			auto next_name = FindNextCName(
				str,
				position
			);
			auto next_symbol = FindFirstOf(
				str,
				position,
				enum_start_symbols
			);
			// if C compatible name is first then set name, otherwise process symbol.
			// In case of "class" then both name and symbol would be seen at the same
			// place in which case name is not earlier than symbol so a symbol gets
			// processed... Should perhaps add a special rule for this for clarity.
			if( next_name.begin < next_symbol.begin ) {
				if( std::size( enum_info.name ) ) PrintError( "Enum name given twice." );
				enum_info.name	= next_name.word;
				position		= next_name.end;
			} else {
				position = next_symbol.end;
				switch( next_symbol.option ) {
					case 0:		// class
					{
						enum_info.is_class	= true;
					}
						break;
					case 1:		// :
					{
						auto underlaying_type_name = FindNextCName(
							str,
							position
						);
						auto underlaying_type_symbol = FindFirstOf(
							str,
							position,
							enum_start_symbols
						);
						if( underlaying_type_name.word == "" ) PrintError( "Unexpected end of file." );
						if( underlaying_type_symbol.begin < underlaying_type_name.begin ) PrintError( "Missing enum underlaying type." );

						enum_info.underlaying_type	= underlaying_type_name.word;
						position = underlaying_type_name.end;
					}
						break;
					case 2:		// {
					{
						enum_body_entered			= true;
					}
						break;
					case 3:		// ;
					{
						enum_body_entered			= true;
						enum_info.is_declared_only	= true;
					}
						break;
					default:
						// Should never get here.
						PrintError( "Unexpected end of file." );
				}
			}
		}
	}

	if( options.debug_prints ) {
		std::string namespace_str;
		for( auto & n : namespace_stack ) {
			namespace_str += "::" + n.second;
		}
		if( std::size( namespace_stack ) == 0 ) {
			namespace_str = "::";
		}
		std::cout
			<< "Namespace:        \"" << namespace_str << "\"\n"
			<< "Enum name:        \"" << enum_info.name << "\"\n"
			<< "Underlaying type: \"" << enum_info.underlaying_type << "\"\n"
			<< "Is enum class:    \"" << enum_info.is_class << "\"\n"
			<< "Is declared only: \"" << enum_info.is_declared_only << "\"\n";
	}

	std::vector<EnumFieldInfo> fields;
	fields.reserve( 1024 );

	// Causes a bracket counting mismatch if we skip parsing enums without name.
	// We can easily skip this later anyways.
	// if( std::size( enum_info.name ) && !enum_info.is_declared_only ) {
	if( !enum_info.is_declared_only ) {

		std::vector<std::string_view> enum_body_symbols {
			",",	// case 0:	next field
			"=",	// case 1:	setting value to field
			"}",	// case 2:	enum body end
		};

		bool is_next_field		= true;
		bool exiting_enum_body	= false;

		FindResult next_name {};
		FindResult next_symbol {};

		while( !exiting_enum_body ) {
			// Only read the next name if the next symbol is not "=",
			// this prevents us trying to read a value as a name.
			if( next_symbol.option != 1 ) {
				next_name = FindNextCName(
					str,
					position
				);
			} else {
				next_name = {};
			}
			next_symbol = FindFirstOf(
				str,
				position,
				enum_body_symbols
			);
			if( next_name.begin < next_symbol.begin ) {
				if( !is_next_field ) PrintError( "Enum field given twice without comma inbetween." );
				EnumFieldInfo f {};
				f.name		= next_name.word;
				f.value		= "";
				f.disabled	= false;
				fields.push_back( f );
				position		= next_name.end;
				is_next_field	= false;
			} else {
				position		= next_symbol.end;
				switch( next_symbol.option ) {
					case 0:		// ,
					{
						if( is_next_field ) PrintError( "Enum body has two commas without a field name inbetween." );
						is_next_field	= true;
					}
						break;
					case 1:		// =
					{
						if( is_next_field ) PrintError( "Enum body has a comma before \"=\"." );
						next_symbol = FindFirstOf(
							str,
							position,
							enum_body_symbols
						);

						if( position == next_symbol.begin )	PrintError( "Expected something between \"=\" and \",\" in enum field values." );

						auto value = FindNextNonWhitespaceString(
							str.substr( position, next_symbol.begin - position ),
							0
						);
						if( value.word == "" ) PrintError( "Internal error while trying to parse enum field assigned value." );

						fields.back().value	= value.word;
						position			= next_symbol.end;

						switch( next_symbol.option ) {
							case 0:	// ,
								is_next_field			= true;
								break;
							case 1: // =
								PrintError( "Expected \",\" or \"}\" after enum field value, not \"=\"." );
								break;
							case 2: // }
								exiting_enum_body		= true;
								break;
							default:
								PrintError( "Internal error while parsing enum field assigned value." );
								break;
						}
					}
						break;
					case 2:		// }
					{
						exiting_enum_body = true;
					}
						break;
					default:
						break;
				}
			}
		}
	}

	// if an enum has the same value as another either by value or by field name,
	// we need to skip it. So we'll be flagging those entries.
	{
		for( size_t i = 0; i < std::size( fields ); ++i ) {
			for( size_t p = 0; p < i; ++p ) {
				if( fields[ i ].name == fields[ p ].name ||
					fields[ i ].value == fields[ p ].name ) {
					fields[ i ].disabled = true;
					break;
				}
			}
		}
	}

	if( options.debug_prints ) {
		std::cout << "Fields:\n";
		for( auto & f : fields ) {
			std::cout << f.name;
			if( !f.value.empty() ) std::cout << " = " << f.value;
			if( f.disabled ) std::cout << " -> FIELD NOT INCLUDED";
			std::cout << "\n";
		}
	}

	if( !enum_info.is_declared_only && !enum_info.name.empty() ) {
		CreateEnumConverterFunction(
			options,
			namespace_stack,
			enum_info,
			fields,
			output_info
		);
	}

	if( options.debug_prints ) {
		std::cout << "\n\n";
	}
}



void CreateEnumConverterFunction(
	const Options										&	options,
	const std::vector<std::pair<size_t, std::string>>	&	namespace_stack,
	const EnumInfo										&	enum_info,
	const std::vector<EnumFieldInfo>					&	enum_field_info,
	OutputInfo											&	output_info
)
{
	std::string namespace_str;
	for( auto & n : namespace_stack ) {
		namespace_str += n.second + "::";
	}

	auto & header = *output_info.header;
	auto & source = *output_info.source;

	auto WriteFunctionDeclaration =
		[ &options, &enum_info, &namespace_str ] ( std::ofstream & stream )
	{
		if( !options.return_prefix.empty() ) {
			stream << options.return_prefix << " ";
		}
		stream << options.return_type << " ";
		if( !options.return_postfix.empty() ) {
			stream << options.return_postfix << " ";
		}
		stream << options.function_name << "( " << namespace_str << enum_info.name << " enum_value )";
	};

	WriteFunctionDeclaration( header );
	WriteFunctionDeclaration( source );

	header << ";\n";

	source
		<< " {\n"
		<< "\tswitch( enum_value ) {\n";
	for( auto & e : enum_field_info ) {
		if( !e.disabled ) {

			source
				<< "\t\tcase "
				<< namespace_str
				<< enum_info.name << "::" << e.name << ":\n"
				<< "\t\t\treturn \"";

			if( options.output_namespace_name ) {
				source << namespace_str;
			}
			if( options.output_enum_name ) {
				source << enum_info.name << "::";
			}
			source << e.name << "\";\n";
		}
	}
	source
		<< "\t\tdefault:\n"
		<< "\t\t\treturn \"<unknown ";
	if( options.output_namespace_name ) {
		source << namespace_str;
	}
	source << enum_info.name << ">\";\n";
	
	source << "\t};\n";
	source << "};\n";
}



FindResult FindFirstOf(
	const std::string					&	str,
	size_t									position,
	const std::vector<std::string_view>	&	possibilities
)
{
	FindResult result {};

	for( size_t i = 0; i < std::size( possibilities ); ++i ) {
		auto begin = str.find( possibilities[ i ], position );
		if( begin < result.begin ) {
			result.begin	= begin;
			result.end		= std::size( possibilities[ i ] ) + begin;
			result.size		= std::size( possibilities[ i ] );
			result.option	= i;
			result.word		= possibilities[ i ];
		}
	}
	return result;
}



FindResult FindNextNonWhitespaceCharacter(
	const std::string				&	str,
	size_t							position
)
{
	for( size_t i = position; i < std::size( str ); ++i ) {
		if( str[ i ] != NULL && !isspace( str[ i ] ) ) {
			FindResult ret;
			ret.begin	= i;
			ret.end		= i + 1;
			ret.size	= 1;
			ret.option	= SIZE_MAX;
			ret.word	= str[ i ];
			return ret;
		}
	}
	return {};
}



FindResult FindNextNonWhitespaceString(
	const std::string		&	str,
	size_t						position
)
{
	// Loop until we find non-whitespace character
	for( size_t begin = position; begin < std::size( str ); ++begin ) {
		if( str[ begin ] != NULL && !isspace( str[ begin ] ) ) {
			// Found non-whitespace character, now loop until we find a whitespace character.
			for( size_t end = begin; end < std::size( str ); ++end ) {
				if( ( str[ end ] == NULL ) || isspace( str[ end ] ) ) {
					FindResult ret;
					ret.begin	= begin;
					ret.end		= end;
					ret.size	= end - begin;
					ret.option	= SIZE_MAX;
					ret.word	= str.substr( begin, ret.size );
					return ret;
				}
			}
			FindResult ret;
			ret.begin	= begin;
			ret.end		= std::size( str );
			ret.size	= std::size( str ) - begin;
			ret.option	= SIZE_MAX;
			ret.word	= str.substr( begin, ret.size );
			return ret;
		}
	}
	return {};
}



FindResult FindNextCName(
	const std::string			&	str,
	size_t							position
)
{
	for( size_t begin = position; begin < std::size( str ); ++begin ) {
		bool is_first_character = true;
		if( str[ begin ] != NULL && !isspace( int( str[ begin ] ) ) ) {
			if( IsValidCNameCharacter( int( str[ begin ] ), is_first_character ) ) {
				is_first_character = false;
				for( size_t end = begin; end < std::size( str ); ++end ) {
					if( str[ end ] == NULL ||
						isspace( int( str[ end ] ) ) ||
						!IsValidCNameCharacter( int( str[ end ] ), is_first_character ) ) {
						FindResult		result;
						result.begin	= begin;
						result.end		= end;
						result.size		= end - begin;
						result.option	= SIZE_MAX;
						result.word		= str.substr( begin, result.size );
						return result;
					}
				}
				FindResult		result;
				result.begin	= begin;
				result.end		= std::size( str );
				result.size		= std::size( str ) - begin;
				result.option	= SIZE_MAX;
				result.word		= str.substr( begin );
				return result;
			}
		}
	}
	return {};
}



bool IsValidCNameCharacter(
	int		character,
	bool	is_first_character_in_name
)
{
	// Ascii codes in decimals, inclusive
	// Underscore:		95
	// Capital letters:	65 - 90
	// Small letters:	97 - 122
	// Numbers:			48 - 57		If not first character in name

	if( character == 95 )						return true;
	if( character >= 65 && character <= 90 )	return true;
	if( character >= 97 && character <= 122 )	return true;
	if( character >= 48 && character <= 57 ) {
		if( is_first_character_in_name ) {
			PrintError( "First character in a name was a number, check that the code compiles." );
		} else {
			return true;
		}
	}
	return false;
}
