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
cd build
cmake ../

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

    %~dp0\build\tests\Debug\injectorpp_test.exe
    if errorlevel 1 (
        set finalErrorLevel=1
        goto exit
    )
)

:exit
cd ..
exit /b %finalErrorLevel%