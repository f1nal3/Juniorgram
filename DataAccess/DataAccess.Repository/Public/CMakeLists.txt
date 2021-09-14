set(TARGET DataAccess.Repository.Public)
set(OUTPUT_NAME Juniorgram.DataAccess.Repository.Public)

set(SOURCE_FILES
    AbstarctRepositoryContainer.hpp
	IMasterRepository.hpp
	IClientRepository.hpp
	IServerRepository.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})
target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
