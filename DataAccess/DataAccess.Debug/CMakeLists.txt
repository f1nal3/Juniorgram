set(TARGET DataAccess.Debug)
set(OUTPUT_NAME DataAccess.Debug)

set(SOURCE_FILES
	FileDB.hpp
	FileDB.cpp
	FileRepository.hpp
	FileRepository.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${TARGET} PUBLIC
	Utility.Public
	DataAccess.Static
	Network.Public
	CONAN_PKG::nlohmann_json)
