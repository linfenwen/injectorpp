cmake_minimum_required (VERSION 2.6)
project (injectorpp)

if (MSVC)
    add_definitions(-D_UNICODE)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif (MSVC)

include_directories(src/windows/include)

add_subdirectory(src)
add_subdirectory(tests)