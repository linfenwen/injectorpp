@echo off
if not "%~1"=="" (
    set force=%1
    if "%force%" == "/f" (
        echo Deleting build folder...
        rmdir build /s /q 
    )
)

if not exist build (
    echo Creating build folder...
    md build
)
cd build
cmake ../
msbuild /nologo /t:injectorpp injectorpp.sln
cd ..