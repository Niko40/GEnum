@echo off



set /A build_tests=0

:OptionsLoop
call :PrintOptionsMenu ret, %build_tests%
if %ret%==1 call :ToggleValue build_tests, %build_tests%
if %ret%==2 goto Build
if %ret%==3 goto End
goto OptionsLoop







:Build

call :GetAsOnOffString build_tests_str, %build_tests%


set project="GEnum"
set generator="Visual Studio 16 2019"
set architecture="x64"
set source_dir=..

mkdir build
cd build

cmake -G %generator% -A %architecture% -D CMAKE_BUILD_TYPE=Release -D BUILD_TESTS=%build_tests_str% "%source_dir%"

cd %source_dir%

goto End






:PrintOptionsMenu
call :GetAsOnOffString build_tests_str, %~2

cls
echo:
echo ************************************************************
echo *
echo * Choose from the following list and press enter to select.
echo *
echo * [ 1 ] Build tests: %build_tests_str%
echo *
echo * [ 2 ] Build with current settings
echo * [ 3 ] Exit
echo *
echo ************************************************************
echo:

set /p "%~1=Selection: "
exit /B 0



REM First parameter is return value, second parameter is the original value
:ToggleValue
if %~2 == 0 (set /A "%~1"=1) else (set /A "%~1"=0)
exit /B 0



REM return numerals 0 and 1 as "OFF" and "ON"
REM First parameter is return value, second parameter is the original value
:GetAsOnOffString
if %~2 == 0 (set "%~1=OFF") else (set "%~1=ON")
exit /B 0



:End
REM pause
exit /B %ERRORLEVEL%
