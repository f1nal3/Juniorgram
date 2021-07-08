set(TARGET Utility.Public)
set(OUTPUT_NAME Juniorgram.Utility.Public)

set(SOURCE_FILES
    Include/Utility/CompressionHandler.hpp
    Include/Utility/EncryptionHandler.hpp
    Include/Utility/Handler.hpp
    Include/Utility/SerializationHandler.hpp
    Include/Utility/Utility.hpp
    Include/Utility/WarningSuppression.hpp
    Include/Utility/Exception.hpp
    Include/Utility/YasSerializer.hpp
    Include/Utility/SQLUtility.hpp
    Include/Utility/UserDataValidation.hpp
	Include/Utility/KeyHolder.hpp
	Include/Utility/KeyDestibutor.hpp
	Include/Utility/TokenBuilder.hpp)

add_library(${TARGET} INTERFACE)
target_sources(${TARGET} INTERFACE ${SOURCE_FILES})
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
