set(TARGET Kernel.Exec)
set(OUTPUT_NAME ${PROJECT}.Kernel.Exec)

set(SOURCE_FILES
    MainKernel.cpp)

if(WIN32)
    set(EXTRA_LIBS ws2_32 wsock32)
endif()

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
                                        argparse::argparse
                                PUBLIC  Kernel.Core.Static
										Network.Public
										Utility.Public)

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin/)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin/)
