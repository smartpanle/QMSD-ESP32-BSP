idf_component_register( 
    SRC_DIRS mp3player libhelix-mp3/src libhelix-mp3/src/real
    INCLUDE_DIRS mp3player libhelix-mp3/src/pub
    PRIV_INCLUDE_DIRS libhelix-mp3/src/real
    REQUIRES qmsd_utils
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-but-set-variable)
