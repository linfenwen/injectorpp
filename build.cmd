@echo off
set finalErrorLevel=0
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

if "%INJECTOR_BUILD_VS_VERSION%" == "" (
    set injectorBuildVSVersion=Visual Studio 14 2015
) else if "%INJECTOR_BUILD_VS_VERSION%" == "Visual Studio 2013" (
    set injectorBuildVSVersion=Visual Studio 12 2013
) else if "%INJECTOR_BUILD_VS_VERSION%" == "Visual Studio 2015" (
    set injectorBuildVSVersion=Visual Studio 14 2015
) else if "%INJECTOR_BUILD_VS_VERSION%" == "Visual Studio 2017" (
    set injectorBuildVSVersion=Visual Studio 15 2017
)

echo The visual studio version is "%injectorBuildVSVersion%"

cd build

REM x86 Windows
if not exist x86 (
    md x86
)
cd x86
cmake -G "%injectorBuildVSVersion%" ../../
if errorlevel 1 (
    set finalErrorLevel=1
    goto exit
)

msbuild /nologo /t:injectorpp injectorpp.sln
if errorlevel 1 (
    set finalErrorLevel=1
    goto exit
)

if not %skiptests% == 1 (
    msbuild /nologo /t:gmock injectorpp.sln
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )

    msbuild /nologo /t:injectorpp_test injectorpp.sln
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )

    %~dp0\build\x86\tests\Debug\injectorpp_test.exe
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )
)
cd ../

REM x64 Windows
if not exist x64 (
    md x64
)
cd x64
cmake -G "%injectorBuildVSVersion% Win64" ../../
if errorlevel 1 (
    set finalErrorLevel=1
    goto exit
)

msbuild /nologo /t:injectorpp injectorpp.sln
if errorlevel 1 (
    set finalErrorLevel=1
    goto exit
)

if not %skiptests% == 1 (
    msbuild /nologo /t:gmock injectorpp.sln
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )

    msbuild /nologo /t:injectorpp_test injectorpp.sln
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )

    %~dp0\build\x64\tests\Debug\injectorpp_test.exe
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )
)
cd ../..

:exit
cd %~dp0
exit /b %finalErrorLevel%