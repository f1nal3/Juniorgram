set(TARGET Crypto.Static)
set(OUTPUT_NAME Base.Crypto.Static)

set(SOURCE_FILES
    Cryptography.hpp
    Cryptography.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} CONAN_PKG::cryptopp)

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
