@echo off
set skiptests=0
if not "%~1"=="" (
    if "%~1" == "/f" (
        echo Deleting build folder...
        rmdir build /s /q 
    ) else if "%~1" == "/skiptests" (
        set skiptests=1
    )
)

if not "%~2"=="" (
    if "%~2" == "/f" (
        echo Deleting build folder...
        rmdir build /s /q 
    ) else if "%~2" == "/skiptests" (
        set skiptests=1
    )
) 

if not exist build (
    echo Creating build folder...
    md build
)
cd build
cmake ../

msbuild /nologo /t:injectorpp injectorpp.sln

if not %skiptests% == 1 (
    msbuild /nologo /t:gmock injectorpp.sln
    msbuild /nologo /t:injectorpp_test injectorpp.sln
    %~dp0\build\tests\Debug\injectorpp_test.exe
)
cd ..