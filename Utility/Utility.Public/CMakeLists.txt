set(TARGET Utility.Public)
set(OUTPUT_NAME Juniorgram.Utility.Public)

set(SOURCE_FILES
    Include/Utility/Exception.hpp
    Include/Utility/SQLUtility.hpp
    Include/Utility/WarningSuppression.hpp
    Include/Utility/UserDataValidation.hpp
    Include/Utility/Utility.hpp)

add_library(${TARGET} INTERFACE)
target_sources(${TARGET} PUBLIC ${SOURCE_FILES})
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
