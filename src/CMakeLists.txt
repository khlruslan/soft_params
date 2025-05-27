include_directories(   ${soft_params_SOURCE_DIR}/inc
                       ${soft_params_SOURCE_DIR}/lib
                   )


add_executable(soft_para_diff main.cxx)

target_link_libraries(soft_para_diff PUBLIC FormatUtils SoftParams MmlUtils Tabulator)
