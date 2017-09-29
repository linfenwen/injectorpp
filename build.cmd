rmdir /s /q build
md build
cd build
cmake ../
msbuild /nologo /t:injectorpp injectorpp.sln
cd ..