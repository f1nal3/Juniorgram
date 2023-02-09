project(Client LANGUAGES CXX)

set(TARGET Client)
set(FOLDER "Client")

add_subdirectory(Client.Core)
add_subdirectory(Client.CLI)
add_subdirectory(Client.Mock)

add_library(${TARGET} INTERFACE)

if (COVERAGE_MODE)
    get_target_property(target_cc_options ${TARGET} COMPILE_OPTIONS)
    get_target_property(target_ll_options ${TARGET} LINK_OPTIONS)

    list(REMOVE_ITEM target_cc_options "--coverage" "-g" "-O0")
    list(REMOVE_ITEM target_ll_options "--coverage" "-lgcov")
    set_property(TARGET ${TARGET} PROPERTY COMPILE_OPTIONS ${target_cc_options})
    set_property(TARGET ${TARGET} PROPERTY LINK_OPTIONS ${target_ll_options})
endif()