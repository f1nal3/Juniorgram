set(TARGET DataAccess.Static)
set(OUTPUT_NAME DataAccess.Static)

set(SOURCE_FILES
    PostgreAdapter.hpp
    PostgreAdapter.cpp
    PostgreRepository.hpp
    PostgreRepository.cpp
    PostgreAbstractionLayout.hpp
    PostgreAbstractionLayout.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC
    CONAN_PKG::libpq
    CONAN_PKG::libpqxx)
    
target_link_libraries(${TARGET} PUBLIC
    Utility.Public
    DataAccess.Public
    Network.Public)
