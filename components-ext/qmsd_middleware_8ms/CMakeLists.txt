if(CONFIG_QMSD_GUI_LVGL_V7)
set(src_file_dir .)
elseif(CONFIG_QMSD_GUI_LVGL_V8)
set(src_file_dir )
endif()

idf_component_register(
    SRC_DIRS ${src_file_dir}
    INCLUDE_DIRS ${src_file_dir} 
    REQUIRES qmsd_board json
)
