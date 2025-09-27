#add_library(Print print.cxx)
#add_library(MmlUtils mml_utils.cxx)
#add_library(Tabulator tabulator.cxx)
#add_library(TableView table_view.cxx)
#add_library(FormatUtils format_utils.cxx)

add_library(SoftParams soft_param.cxx sort_param.cxx print.cxx mml_utils.cxx tabulator.cxx table_view.cxx format_utils.cxx parameters_fabric.cxx)

#target_link_libraries(TableView PUBLIC FormatUtils Tabulator)
#target_link_libraries(SoftParams PUBLIC FormatUtils Print TableView MmlUtils)
