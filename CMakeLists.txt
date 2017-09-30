cmake_minimum_required (VERSION 2.6)
project (injectorpp)

if (MSVC)
set( MSVC_INCREMENTAL_YES_FLAG "")
    add_definitions(-D_UNICODE)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO" )
    SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /INCREMENTAL:NO" )
endif (MSVC)

include_directories(src/windows/include)

add_subdirectory(src)
add_subdirectory(tests)