include_directories(include)
aux_source_directory(src injectorpp_src)
add_library(injectorpp ${injectorpp_src})