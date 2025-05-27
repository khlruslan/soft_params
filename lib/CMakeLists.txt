add_library(FormatUtils format_utils.cxx)
add_library(SoftParams params.cxx param_fabric.cxx param_compare.cxx)
add_library(MmlUtils mml_utils.cxx)
add_library(Tabulator tabulator.cxx)


target_link_libraries(SoftParams PUBLIC FormatUtils MmlUtils)
