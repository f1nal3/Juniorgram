set(TARGET Utility.Public)
set(OUTPUT_NAME ${PROJECT}.Utility.Public)

set(SOURCE_FILES
        Include/Utility/JGExceptions.hpp
        Include/Utility/SQLUtility.hpp
        Include/Utility/WarningSuppression.hpp
        Include/Utility/UserDataValidation.hpp
        Include/Utility/Utility.hpp
        Include/Utility/SafeQueue.hpp
        Include/Utility/UtilityTime.hpp)

add_library(${TARGET} INTERFACE ${SOURCE_FILES})

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES 
                            FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(${TARGET} INTERFACE Base.Logger.Static)
