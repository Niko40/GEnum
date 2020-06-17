
# GEnum

GEnum is a simple tool to generate enum to string functions automatically from the source files.

Every approach of converting enums to strings in C++ has their strenghts and drawbacks, this one is useful when using a library that uses plain enums with huge enum values. (Specifically I wrote this tool for Vulkan C API enumerators in mind.)

#### Capabilities
- Filter out comments and character arrays.
- Understands namespaces.
- Choose return type, return type prefix, return type postfix and function name.

#### Limitations
- Nameless enums ignored. Limitation of C/C++ in this approach.
- Enum fields with same value ignored. Limitation of C/C++ in this approach.
- Does not handle enums that have same name.

## Compiling

Standard CMake, for Visual Studio 2019 just run the Build_VisualStudio2019.bat batch file.

## Usage

This is an command line tool that parses through every file in a given folder. Run the program in console to see help.

```
  Usage:
    GenerateEnumNames [-option] [<param>]
    GenerateEnumNames -help

  Options:
    -help           This help.
    -G <path>       Generate from source path, should point to the root folder of
                      the source.
    -H <path>       Set destination path, this is the final output where the
                      header file will be saved to.
                      Default = "D:\Projektit\C++\GEnum - Backup\build\Bin\Debug\ToString.h".
    -C <path>       Set destination path, this is the final output where the
                      source file will be saved to.
                      Default = "D:\Projektit\C++\GEnum - Backup\build\Bin\Debug\ToString.cpp".
    -P <prefix>     Prefix for the function return type, useful for libraries.
                      -> [prefix] <ret type> [postfix] <funcname>(...)
                      Default = "".
    -O <postfix>    Postfix for the function return type, useful for libraries
                      -> [prefix] <ret type> [postfix] <funcname>(...)
                      Default = "".
    -R <ret type>   Return type of the converter function.
                      -> [prefix] <ret type> [postfix] <funcname>(...)
                      Default = "std::string_view".
                      Must be convertible from "char*".
    -F <funcname>   Name of the converter function.
                      -> [prefix] <ret type> [postfix] <funcname>(...)
                      Default = "EnumToString".
    -I <include>    Include header file, for return type. Must include
                      encapsulation, either <> or "".
                      Default = "<string_view>".
    -E              Include enum names in the returned string.
                      -> "[enum name]::<field name>"
    -N              Include namespace names in the returned string.
                      -> "[namespace name]::<field name>"
    -D              Display debug information, might help in case of errors.
                      Default = "0".
```

## Contributing

All contributions welcome. Small bug fix pull requests will be accepted immediately, larger ones might need some discussions first.

## License
[MIT](LICENSE.md)

Copyright (c) 2020 Niko Kauppi
