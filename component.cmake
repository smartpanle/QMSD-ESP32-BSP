add_definitions(-DSDK_8MS_VERSION="v0.8.1")

list(APPEND EXTRA_COMPONENT_DIRS 
    $ENV{QMSD_8MS_PATH}/components
    $ENV{QMSD_8MS_PATH}/components-third-party
    $ENV{QMSD_8MS_PATH}/components-ext
    $ENV{QMSD_8MS_PATH}/components-ext/qmsd_sensor
    $ENV{QMSD_8MS_PATH}/tools
)

if(EXISTS $ENV{QMSD_8MS_PATH}/components-internal)
    list(APPEND EXTRA_COMPONENT_DIRS 
        $ENV{QMSD_8MS_PATH}/components-internal
    )
endif()
