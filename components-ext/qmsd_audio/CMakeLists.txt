idf_component_register( 
    SRC_DIRS mp3player libhelix-mp3 libhelix-mp3/real
    INCLUDE_DIRS mp3player libhelix-mp3/pub
    PRIV_INCLUDE_DIRS libhelix-mp3/real
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-but-set-variable)
