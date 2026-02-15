find_path(EPOXY_INCLUDE_DIR
    NAMES epoxy/gl.h
)

find_library(EPOXY_LIBRARY
    NAMES epoxy
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(epoxy
    REQUIRED_VARS EPOXY_LIBRARY EPOXY_INCLUDE_DIR
)

if(epoxy_FOUND)
    set(epoxy_INCLUDE_DIRS "${EPOXY_INCLUDE_DIR}")
    set(epoxy_LIBRARIES "${EPOXY_LIBRARY}")

    if(NOT TARGET epoxy::epoxy)
        add_library(epoxy::epoxy UNKNOWN IMPORTED)
        set_target_properties(epoxy::epoxy PROPERTIES
            IMPORTED_LOCATION "${EPOXY_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${EPOXY_INCLUDE_DIR}"
        )
    endif()
endif()
