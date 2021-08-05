include(CMakeParseArguments)

macro(conan_find_apple_frameworks FRAMEWORKS_FOUND FRAMEWORKS SUFFIX BUILD_TYPE)
    if(APPLE)
        if(CMAKE_BUILD_TYPE)
            set(_BTYPE ${CMAKE_BUILD_TYPE})
        elseif(NOT BUILD_TYPE STREQUAL "")
            set(_BTYPE ${BUILD_TYPE})
        endif()
        if(_BTYPE)
            if(${_BTYPE} MATCHES "Debug|_DEBUG")
                set(CONAN_FRAMEWORKS${SUFFIX} ${CONAN_FRAMEWORKS${SUFFIX}_DEBUG} ${CONAN_FRAMEWORKS${SUFFIX}})
                set(CONAN_FRAMEWORK_DIRS${SUFFIX} ${CONAN_FRAMEWORK_DIRS${SUFFIX}_DEBUG} ${CONAN_FRAMEWORK_DIRS${SUFFIX}})
            elseif(${_BTYPE} MATCHES "Release|_RELEASE")
                set(CONAN_FRAMEWORKS${SUFFIX} ${CONAN_FRAMEWORKS${SUFFIX}_RELEASE} ${CONAN_FRAMEWORKS${SUFFIX}})
                set(CONAN_FRAMEWORK_DIRS${SUFFIX} ${CONAN_FRAMEWORK_DIRS${SUFFIX}_RELEASE} ${CONAN_FRAMEWORK_DIRS${SUFFIX}})
            elseif(${_BTYPE} MATCHES "RelWithDebInfo|_RELWITHDEBINFO")
                set(CONAN_FRAMEWORKS${SUFFIX} ${CONAN_FRAMEWORKS${SUFFIX}_RELWITHDEBINFO} ${CONAN_FRAMEWORKS${SUFFIX}})
                set(CONAN_FRAMEWORK_DIRS${SUFFIX} ${CONAN_FRAMEWORK_DIRS${SUFFIX}_RELWITHDEBINFO} ${CONAN_FRAMEWORK_DIRS${SUFFIX}})
            elseif(${_BTYPE} MATCHES "MinSizeRel|_MINSIZEREL")
                set(CONAN_FRAMEWORKS${SUFFIX} ${CONAN_FRAMEWORKS${SUFFIX}_MINSIZEREL} ${CONAN_FRAMEWORKS${SUFFIX}})
                set(CONAN_FRAMEWORK_DIRS${SUFFIX} ${CONAN_FRAMEWORK_DIRS${SUFFIX}_MINSIZEREL} ${CONAN_FRAMEWORK_DIRS${SUFFIX}})
            endif()
        endif()
        foreach(_FRAMEWORK ${FRAMEWORKS})
            # https://cmake.org/pipermail/cmake-developers/2017-August/030199.html
            find_library(CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND NAME ${_FRAMEWORK} PATHS ${CONAN_FRAMEWORK_DIRS${SUFFIX}} CMAKE_FIND_ROOT_PATH_BOTH)
            if(CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND)
                list(APPEND ${FRAMEWORKS_FOUND} ${CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND})
            else()
                message(FATAL_ERROR "Framework library ${_FRAMEWORK} not found in paths: ${CONAN_FRAMEWORK_DIRS${SUFFIX}}")
            endif()
        endforeach()
    endif()
endmacro()


#################
###  ASIO
#################
set(CONAN_ASIO_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/asio/1.19.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")
set(CONAN_INCLUDE_DIRS_ASIO "C:/Users/Daniil_Akhramiuk/.conan/data/asio/1.19.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include")
set(CONAN_LIB_DIRS_ASIO )
set(CONAN_BIN_DIRS_ASIO )
set(CONAN_RES_DIRS_ASIO )
set(CONAN_SRC_DIRS_ASIO )
set(CONAN_BUILD_DIRS_ASIO "C:/Users/Daniil_Akhramiuk/.conan/data/asio/1.19.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/")
set(CONAN_FRAMEWORK_DIRS_ASIO )
set(CONAN_LIBS_ASIO )
set(CONAN_PKG_LIBS_ASIO )
set(CONAN_SYSTEM_LIBS_ASIO )
set(CONAN_FRAMEWORKS_ASIO )
set(CONAN_FRAMEWORKS_FOUND_ASIO "")  # Will be filled later
set(CONAN_DEFINES_ASIO "-DASIO_STANDALONE")
set(CONAN_BUILD_MODULES_PATHS_ASIO )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_ASIO "ASIO_STANDALONE")

set(CONAN_C_FLAGS_ASIO "")
set(CONAN_CXX_FLAGS_ASIO "")
set(CONAN_SHARED_LINKER_FLAGS_ASIO "")
set(CONAN_EXE_LINKER_FLAGS_ASIO "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_ASIO_LIST "")
set(CONAN_CXX_FLAGS_ASIO_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_ASIO_LIST "")
set(CONAN_EXE_LINKER_FLAGS_ASIO_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_ASIO "${CONAN_FRAMEWORKS_ASIO}" "_ASIO" "")
# Append to aggregated values variable
set(CONAN_LIBS_ASIO ${CONAN_PKG_LIBS_ASIO} ${CONAN_SYSTEM_LIBS_ASIO} ${CONAN_FRAMEWORKS_FOUND_ASIO})


#################
###  CRYPTOPP
#################
set(CONAN_CRYPTOPP_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab")
set(CONAN_INCLUDE_DIRS_CRYPTOPP "C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include")
set(CONAN_LIB_DIRS_CRYPTOPP "C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib")
set(CONAN_BIN_DIRS_CRYPTOPP )
set(CONAN_RES_DIRS_CRYPTOPP )
set(CONAN_SRC_DIRS_CRYPTOPP )
set(CONAN_BUILD_DIRS_CRYPTOPP "C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib/cmake")
set(CONAN_FRAMEWORK_DIRS_CRYPTOPP )
set(CONAN_LIBS_CRYPTOPP cryptopp-static)
set(CONAN_PKG_LIBS_CRYPTOPP cryptopp-static)
set(CONAN_SYSTEM_LIBS_CRYPTOPP ws2_32)
set(CONAN_FRAMEWORKS_CRYPTOPP )
set(CONAN_FRAMEWORKS_FOUND_CRYPTOPP "")  # Will be filled later
set(CONAN_DEFINES_CRYPTOPP )
set(CONAN_BUILD_MODULES_PATHS_CRYPTOPP )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_CRYPTOPP )

set(CONAN_C_FLAGS_CRYPTOPP "")
set(CONAN_CXX_FLAGS_CRYPTOPP "")
set(CONAN_SHARED_LINKER_FLAGS_CRYPTOPP "")
set(CONAN_EXE_LINKER_FLAGS_CRYPTOPP "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_CRYPTOPP_LIST "")
set(CONAN_CXX_FLAGS_CRYPTOPP_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_LIST "")
set(CONAN_EXE_LINKER_FLAGS_CRYPTOPP_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_CRYPTOPP "${CONAN_FRAMEWORKS_CRYPTOPP}" "_CRYPTOPP" "")
# Append to aggregated values variable
set(CONAN_LIBS_CRYPTOPP ${CONAN_PKG_LIBS_CRYPTOPP} ${CONAN_SYSTEM_LIBS_CRYPTOPP} ${CONAN_FRAMEWORKS_FOUND_CRYPTOPP})


#################
###  EXPAT
#################
set(CONAN_EXPAT_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a")
set(CONAN_INCLUDE_DIRS_EXPAT "C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a/include")
set(CONAN_LIB_DIRS_EXPAT "C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a/lib")
set(CONAN_BIN_DIRS_EXPAT )
set(CONAN_RES_DIRS_EXPAT )
set(CONAN_SRC_DIRS_EXPAT )
set(CONAN_BUILD_DIRS_EXPAT "C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a/")
set(CONAN_FRAMEWORK_DIRS_EXPAT )
set(CONAN_LIBS_EXPAT libexpatMD)
set(CONAN_PKG_LIBS_EXPAT libexpatMD)
set(CONAN_SYSTEM_LIBS_EXPAT )
set(CONAN_FRAMEWORKS_EXPAT )
set(CONAN_FRAMEWORKS_FOUND_EXPAT "")  # Will be filled later
set(CONAN_DEFINES_EXPAT "-DXML_STATIC")
set(CONAN_BUILD_MODULES_PATHS_EXPAT )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_EXPAT "XML_STATIC")

set(CONAN_C_FLAGS_EXPAT "")
set(CONAN_CXX_FLAGS_EXPAT "")
set(CONAN_SHARED_LINKER_FLAGS_EXPAT "")
set(CONAN_EXE_LINKER_FLAGS_EXPAT "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_EXPAT_LIST "")
set(CONAN_CXX_FLAGS_EXPAT_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_EXPAT_LIST "")
set(CONAN_EXE_LINKER_FLAGS_EXPAT_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_EXPAT "${CONAN_FRAMEWORKS_EXPAT}" "_EXPAT" "")
# Append to aggregated values variable
set(CONAN_LIBS_EXPAT ${CONAN_PKG_LIBS_EXPAT} ${CONAN_SYSTEM_LIBS_EXPAT} ${CONAN_FRAMEWORKS_FOUND_EXPAT})


#################
###  CATCH2
#################
set(CONAN_CATCH2_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")
set(CONAN_INCLUDE_DIRS_CATCH2 "C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include")
set(CONAN_LIB_DIRS_CATCH2 "C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib")
set(CONAN_BIN_DIRS_CATCH2 )
set(CONAN_RES_DIRS_CATCH2 )
set(CONAN_SRC_DIRS_CATCH2 )
set(CONAN_BUILD_DIRS_CATCH2 "C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib/cmake/Catch2")
set(CONAN_FRAMEWORK_DIRS_CATCH2 )
set(CONAN_LIBS_CATCH2 )
set(CONAN_PKG_LIBS_CATCH2 )
set(CONAN_SYSTEM_LIBS_CATCH2 )
set(CONAN_FRAMEWORKS_CATCH2 )
set(CONAN_FRAMEWORKS_FOUND_CATCH2 "")  # Will be filled later
set(CONAN_DEFINES_CATCH2 )
set(CONAN_BUILD_MODULES_PATHS_CATCH2 )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_CATCH2 )

set(CONAN_C_FLAGS_CATCH2 "")
set(CONAN_CXX_FLAGS_CATCH2 "")
set(CONAN_SHARED_LINKER_FLAGS_CATCH2 "")
set(CONAN_EXE_LINKER_FLAGS_CATCH2 "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_CATCH2_LIST "")
set(CONAN_CXX_FLAGS_CATCH2_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_CATCH2_LIST "")
set(CONAN_EXE_LINKER_FLAGS_CATCH2_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_CATCH2 "${CONAN_FRAMEWORKS_CATCH2}" "_CATCH2" "")
# Append to aggregated values variable
set(CONAN_LIBS_CATCH2 ${CONAN_PKG_LIBS_CATCH2} ${CONAN_SYSTEM_LIBS_CATCH2} ${CONAN_FRAMEWORKS_FOUND_CATCH2})


#################
###  LIBTOOL
#################
set(CONAN_LIBTOOL_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4")
set(CONAN_INCLUDE_DIRS_LIBTOOL "C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/include")
set(CONAN_LIB_DIRS_LIBTOOL "C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/lib")
set(CONAN_BIN_DIRS_LIBTOOL "C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/bin")
set(CONAN_RES_DIRS_LIBTOOL )
set(CONAN_SRC_DIRS_LIBTOOL )
set(CONAN_BUILD_DIRS_LIBTOOL "C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/")
set(CONAN_FRAMEWORK_DIRS_LIBTOOL )
set(CONAN_LIBS_LIBTOOL ltdl)
set(CONAN_PKG_LIBS_LIBTOOL ltdl)
set(CONAN_SYSTEM_LIBS_LIBTOOL )
set(CONAN_FRAMEWORKS_LIBTOOL )
set(CONAN_FRAMEWORKS_FOUND_LIBTOOL "")  # Will be filled later
set(CONAN_DEFINES_LIBTOOL )
set(CONAN_BUILD_MODULES_PATHS_LIBTOOL )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBTOOL )

set(CONAN_C_FLAGS_LIBTOOL "")
set(CONAN_CXX_FLAGS_LIBTOOL "")
set(CONAN_SHARED_LINKER_FLAGS_LIBTOOL "")
set(CONAN_EXE_LINKER_FLAGS_LIBTOOL "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBTOOL_LIST "")
set(CONAN_CXX_FLAGS_LIBTOOL_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBTOOL_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBTOOL_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBTOOL "${CONAN_FRAMEWORKS_LIBTOOL}" "_LIBTOOL" "")
# Append to aggregated values variable
set(CONAN_LIBS_LIBTOOL ${CONAN_PKG_LIBS_LIBTOOL} ${CONAN_SYSTEM_LIBS_LIBTOOL} ${CONAN_FRAMEWORKS_FOUND_LIBTOOL})


#################
###  LIBPQXX
#################
set(CONAN_LIBPQXX_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f")
set(CONAN_INCLUDE_DIRS_LIBPQXX "C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f/include")
set(CONAN_LIB_DIRS_LIBPQXX "C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f/lib")
set(CONAN_BIN_DIRS_LIBPQXX )
set(CONAN_RES_DIRS_LIBPQXX )
set(CONAN_SRC_DIRS_LIBPQXX )
set(CONAN_BUILD_DIRS_LIBPQXX "C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f/")
set(CONAN_FRAMEWORK_DIRS_LIBPQXX )
set(CONAN_LIBS_LIBPQXX pqxx)
set(CONAN_PKG_LIBS_LIBPQXX pqxx)
set(CONAN_SYSTEM_LIBS_LIBPQXX wsock32 ws2_32)
set(CONAN_FRAMEWORKS_LIBPQXX )
set(CONAN_FRAMEWORKS_FOUND_LIBPQXX "")  # Will be filled later
set(CONAN_DEFINES_LIBPQXX )
set(CONAN_BUILD_MODULES_PATHS_LIBPQXX )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBPQXX )

set(CONAN_C_FLAGS_LIBPQXX "")
set(CONAN_CXX_FLAGS_LIBPQXX "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQXX "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQXX "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBPQXX_LIST "")
set(CONAN_CXX_FLAGS_LIBPQXX_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQXX_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQXX_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBPQXX "${CONAN_FRAMEWORKS_LIBPQXX}" "_LIBPQXX" "")
# Append to aggregated values variable
set(CONAN_LIBS_LIBPQXX ${CONAN_PKG_LIBS_LIBPQXX} ${CONAN_SYSTEM_LIBS_LIBPQXX} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX})


#################
###  NLOHMANN_JSON
#################
set(CONAN_NLOHMANN_JSON_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/nlohmann_json/3.9.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")
set(CONAN_INCLUDE_DIRS_NLOHMANN_JSON "C:/Users/Daniil_Akhramiuk/.conan/data/nlohmann_json/3.9.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include")
set(CONAN_LIB_DIRS_NLOHMANN_JSON )
set(CONAN_BIN_DIRS_NLOHMANN_JSON )
set(CONAN_RES_DIRS_NLOHMANN_JSON )
set(CONAN_SRC_DIRS_NLOHMANN_JSON )
set(CONAN_BUILD_DIRS_NLOHMANN_JSON "C:/Users/Daniil_Akhramiuk/.conan/data/nlohmann_json/3.9.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/")
set(CONAN_FRAMEWORK_DIRS_NLOHMANN_JSON )
set(CONAN_LIBS_NLOHMANN_JSON )
set(CONAN_PKG_LIBS_NLOHMANN_JSON )
set(CONAN_SYSTEM_LIBS_NLOHMANN_JSON )
set(CONAN_FRAMEWORKS_NLOHMANN_JSON )
set(CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON "")  # Will be filled later
set(CONAN_DEFINES_NLOHMANN_JSON )
set(CONAN_BUILD_MODULES_PATHS_NLOHMANN_JSON )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_NLOHMANN_JSON )

set(CONAN_C_FLAGS_NLOHMANN_JSON "")
set(CONAN_CXX_FLAGS_NLOHMANN_JSON "")
set(CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON "")
set(CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_NLOHMANN_JSON_LIST "")
set(CONAN_CXX_FLAGS_NLOHMANN_JSON_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_LIST "")
set(CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON "${CONAN_FRAMEWORKS_NLOHMANN_JSON}" "_NLOHMANN_JSON" "")
# Append to aggregated values variable
set(CONAN_LIBS_NLOHMANN_JSON ${CONAN_PKG_LIBS_NLOHMANN_JSON} ${CONAN_SYSTEM_LIBS_NLOHMANN_JSON} ${CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON})


#################
###  YAS
#################
set(CONAN_YAS_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/yas/7.0.5/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")
set(CONAN_INCLUDE_DIRS_YAS "C:/Users/Daniil_Akhramiuk/.conan/data/yas/7.0.5/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include")
set(CONAN_LIB_DIRS_YAS )
set(CONAN_BIN_DIRS_YAS )
set(CONAN_RES_DIRS_YAS )
set(CONAN_SRC_DIRS_YAS )
set(CONAN_BUILD_DIRS_YAS "C:/Users/Daniil_Akhramiuk/.conan/data/yas/7.0.5/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/")
set(CONAN_FRAMEWORK_DIRS_YAS )
set(CONAN_LIBS_YAS )
set(CONAN_PKG_LIBS_YAS )
set(CONAN_SYSTEM_LIBS_YAS )
set(CONAN_FRAMEWORKS_YAS )
set(CONAN_FRAMEWORKS_FOUND_YAS "")  # Will be filled later
set(CONAN_DEFINES_YAS )
set(CONAN_BUILD_MODULES_PATHS_YAS )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_YAS )

set(CONAN_C_FLAGS_YAS "")
set(CONAN_CXX_FLAGS_YAS "")
set(CONAN_SHARED_LINKER_FLAGS_YAS "")
set(CONAN_EXE_LINKER_FLAGS_YAS "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_YAS_LIST "")
set(CONAN_CXX_FLAGS_YAS_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_YAS_LIST "")
set(CONAN_EXE_LINKER_FLAGS_YAS_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_YAS "${CONAN_FRAMEWORKS_YAS}" "_YAS" "")
# Append to aggregated values variable
set(CONAN_LIBS_YAS ${CONAN_PKG_LIBS_YAS} ${CONAN_SYSTEM_LIBS_YAS} ${CONAN_FRAMEWORKS_FOUND_YAS})


#################
###  AUTOMAKE
#################
set(CONAN_AUTOMAKE_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/automake/1.16.3/_/_/package/3e48e69237f7f2196164383ef9dedf0f93cbf249")
set(CONAN_INCLUDE_DIRS_AUTOMAKE )
set(CONAN_LIB_DIRS_AUTOMAKE )
set(CONAN_BIN_DIRS_AUTOMAKE "C:/Users/Daniil_Akhramiuk/.conan/data/automake/1.16.3/_/_/package/3e48e69237f7f2196164383ef9dedf0f93cbf249/bin")
set(CONAN_RES_DIRS_AUTOMAKE )
set(CONAN_SRC_DIRS_AUTOMAKE )
set(CONAN_BUILD_DIRS_AUTOMAKE "C:/Users/Daniil_Akhramiuk/.conan/data/automake/1.16.3/_/_/package/3e48e69237f7f2196164383ef9dedf0f93cbf249/")
set(CONAN_FRAMEWORK_DIRS_AUTOMAKE )
set(CONAN_LIBS_AUTOMAKE )
set(CONAN_PKG_LIBS_AUTOMAKE )
set(CONAN_SYSTEM_LIBS_AUTOMAKE )
set(CONAN_FRAMEWORKS_AUTOMAKE )
set(CONAN_FRAMEWORKS_FOUND_AUTOMAKE "")  # Will be filled later
set(CONAN_DEFINES_AUTOMAKE )
set(CONAN_BUILD_MODULES_PATHS_AUTOMAKE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_AUTOMAKE )

set(CONAN_C_FLAGS_AUTOMAKE "")
set(CONAN_CXX_FLAGS_AUTOMAKE "")
set(CONAN_SHARED_LINKER_FLAGS_AUTOMAKE "")
set(CONAN_EXE_LINKER_FLAGS_AUTOMAKE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_AUTOMAKE_LIST "")
set(CONAN_CXX_FLAGS_AUTOMAKE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_AUTOMAKE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_AUTOMAKE "${CONAN_FRAMEWORKS_AUTOMAKE}" "_AUTOMAKE" "")
# Append to aggregated values variable
set(CONAN_LIBS_AUTOMAKE ${CONAN_PKG_LIBS_AUTOMAKE} ${CONAN_SYSTEM_LIBS_AUTOMAKE} ${CONAN_FRAMEWORKS_FOUND_AUTOMAKE})


#################
###  LIBPQ
#################
set(CONAN_LIBPQ_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f")
set(CONAN_INCLUDE_DIRS_LIBPQ "C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f/include")
set(CONAN_LIB_DIRS_LIBPQ "C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f/lib")
set(CONAN_BIN_DIRS_LIBPQ )
set(CONAN_RES_DIRS_LIBPQ )
set(CONAN_SRC_DIRS_LIBPQ )
set(CONAN_BUILD_DIRS_LIBPQ "C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f/")
set(CONAN_FRAMEWORK_DIRS_LIBPQ )
set(CONAN_LIBS_LIBPQ libpq libpgport)
set(CONAN_PKG_LIBS_LIBPQ libpq libpgport)
set(CONAN_SYSTEM_LIBS_LIBPQ ws2_32 secur32 advapi32 shell32 crypt32 wldap32)
set(CONAN_FRAMEWORKS_LIBPQ )
set(CONAN_FRAMEWORKS_FOUND_LIBPQ "")  # Will be filled later
set(CONAN_DEFINES_LIBPQ )
set(CONAN_BUILD_MODULES_PATHS_LIBPQ )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBPQ )

set(CONAN_C_FLAGS_LIBPQ "")
set(CONAN_CXX_FLAGS_LIBPQ "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQ "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQ "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBPQ_LIST "")
set(CONAN_CXX_FLAGS_LIBPQ_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQ_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQ_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBPQ "${CONAN_FRAMEWORKS_LIBPQ}" "_LIBPQ" "")
# Append to aggregated values variable
set(CONAN_LIBS_LIBPQ ${CONAN_PKG_LIBS_LIBPQ} ${CONAN_SYSTEM_LIBS_LIBPQ} ${CONAN_FRAMEWORKS_FOUND_LIBPQ})


#################
###  AUTOCONF
#################
set(CONAN_AUTOCONF_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/autoconf/2.69/_/_/package/3475bd55b91ae904ac96fde0f106a136ab951a5e")
set(CONAN_INCLUDE_DIRS_AUTOCONF )
set(CONAN_LIB_DIRS_AUTOCONF )
set(CONAN_BIN_DIRS_AUTOCONF "C:/Users/Daniil_Akhramiuk/.conan/data/autoconf/2.69/_/_/package/3475bd55b91ae904ac96fde0f106a136ab951a5e/bin")
set(CONAN_RES_DIRS_AUTOCONF )
set(CONAN_SRC_DIRS_AUTOCONF )
set(CONAN_BUILD_DIRS_AUTOCONF "C:/Users/Daniil_Akhramiuk/.conan/data/autoconf/2.69/_/_/package/3475bd55b91ae904ac96fde0f106a136ab951a5e/")
set(CONAN_FRAMEWORK_DIRS_AUTOCONF )
set(CONAN_LIBS_AUTOCONF )
set(CONAN_PKG_LIBS_AUTOCONF )
set(CONAN_SYSTEM_LIBS_AUTOCONF )
set(CONAN_FRAMEWORKS_AUTOCONF )
set(CONAN_FRAMEWORKS_FOUND_AUTOCONF "")  # Will be filled later
set(CONAN_DEFINES_AUTOCONF )
set(CONAN_BUILD_MODULES_PATHS_AUTOCONF )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_AUTOCONF )

set(CONAN_C_FLAGS_AUTOCONF "")
set(CONAN_CXX_FLAGS_AUTOCONF "")
set(CONAN_SHARED_LINKER_FLAGS_AUTOCONF "")
set(CONAN_EXE_LINKER_FLAGS_AUTOCONF "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_AUTOCONF_LIST "")
set(CONAN_CXX_FLAGS_AUTOCONF_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_AUTOCONF_LIST "")
set(CONAN_EXE_LINKER_FLAGS_AUTOCONF_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_AUTOCONF "${CONAN_FRAMEWORKS_AUTOCONF}" "_AUTOCONF" "")
# Append to aggregated values variable
set(CONAN_LIBS_AUTOCONF ${CONAN_PKG_LIBS_AUTOCONF} ${CONAN_SYSTEM_LIBS_AUTOCONF} ${CONAN_FRAMEWORKS_FOUND_AUTOCONF})


#################
###  ZLIB
#################
set(CONAN_ZLIB_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab")
set(CONAN_INCLUDE_DIRS_ZLIB "C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include")
set(CONAN_LIB_DIRS_ZLIB "C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib")
set(CONAN_BIN_DIRS_ZLIB )
set(CONAN_RES_DIRS_ZLIB )
set(CONAN_SRC_DIRS_ZLIB )
set(CONAN_BUILD_DIRS_ZLIB "C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/")
set(CONAN_FRAMEWORK_DIRS_ZLIB )
set(CONAN_LIBS_ZLIB zlib)
set(CONAN_PKG_LIBS_ZLIB zlib)
set(CONAN_SYSTEM_LIBS_ZLIB )
set(CONAN_FRAMEWORKS_ZLIB )
set(CONAN_FRAMEWORKS_FOUND_ZLIB "")  # Will be filled later
set(CONAN_DEFINES_ZLIB )
set(CONAN_BUILD_MODULES_PATHS_ZLIB )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_ZLIB )

set(CONAN_C_FLAGS_ZLIB "")
set(CONAN_CXX_FLAGS_ZLIB "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_ZLIB_LIST "")
set(CONAN_CXX_FLAGS_ZLIB_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB_LIST "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_ZLIB "${CONAN_FRAMEWORKS_ZLIB}" "_ZLIB" "")
# Append to aggregated values variable
set(CONAN_LIBS_ZLIB ${CONAN_PKG_LIBS_ZLIB} ${CONAN_SYSTEM_LIBS_ZLIB} ${CONAN_FRAMEWORKS_FOUND_ZLIB})


#################
###  M4
#################
set(CONAN_M4_ROOT "C:/Users/Daniil_Akhramiuk/.conan/data/m4/1.4.18/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab")
set(CONAN_INCLUDE_DIRS_M4 )
set(CONAN_LIB_DIRS_M4 )
set(CONAN_BIN_DIRS_M4 "C:/Users/Daniil_Akhramiuk/.conan/data/m4/1.4.18/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin")
set(CONAN_RES_DIRS_M4 )
set(CONAN_SRC_DIRS_M4 )
set(CONAN_BUILD_DIRS_M4 "C:/Users/Daniil_Akhramiuk/.conan/data/m4/1.4.18/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/")
set(CONAN_FRAMEWORK_DIRS_M4 )
set(CONAN_LIBS_M4 )
set(CONAN_PKG_LIBS_M4 )
set(CONAN_SYSTEM_LIBS_M4 )
set(CONAN_FRAMEWORKS_M4 )
set(CONAN_FRAMEWORKS_FOUND_M4 "")  # Will be filled later
set(CONAN_DEFINES_M4 )
set(CONAN_BUILD_MODULES_PATHS_M4 )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_M4 )

set(CONAN_C_FLAGS_M4 "")
set(CONAN_CXX_FLAGS_M4 "")
set(CONAN_SHARED_LINKER_FLAGS_M4 "")
set(CONAN_EXE_LINKER_FLAGS_M4 "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_M4_LIST "")
set(CONAN_CXX_FLAGS_M4_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_M4_LIST "")
set(CONAN_EXE_LINKER_FLAGS_M4_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_M4 "${CONAN_FRAMEWORKS_M4}" "_M4" "")
# Append to aggregated values variable
set(CONAN_LIBS_M4 ${CONAN_PKG_LIBS_M4} ${CONAN_SYSTEM_LIBS_M4} ${CONAN_FRAMEWORKS_FOUND_M4})


### Definition of global aggregated variables ###

set(CONAN_PACKAGE_NAME None)
set(CONAN_PACKAGE_VERSION None)

set(CONAN_SETTINGS_ARCH "x86_64")
set(CONAN_SETTINGS_ARCH_BUILD "x86_64")
set(CONAN_SETTINGS_BUILD_TYPE "Release")
set(CONAN_SETTINGS_COMPILER "Visual Studio")
set(CONAN_SETTINGS_COMPILER_RUNTIME "MD")
set(CONAN_SETTINGS_COMPILER_VERSION "16")
set(CONAN_SETTINGS_OS "Windows")
set(CONAN_SETTINGS_OS_BUILD "Windows")

set(CONAN_DEPENDENCIES asio cryptopp expat catch2 libtool libpqxx nlohmann_json yas automake libpq autoconf zlib m4)
# Storing original command line args (CMake helper) flags
set(CONAN_CMD_CXX_FLAGS ${CONAN_CXX_FLAGS})

set(CONAN_CMD_SHARED_LINKER_FLAGS ${CONAN_SHARED_LINKER_FLAGS})
set(CONAN_CMD_C_FLAGS ${CONAN_C_FLAGS})
# Defining accumulated conan variables for all deps

set(CONAN_INCLUDE_DIRS "C:/Users/Daniil_Akhramiuk/.conan/data/asio/1.19.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/nlohmann_json/3.9.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/yas/7.0.5/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f/include"
			"C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include" ${CONAN_INCLUDE_DIRS})
set(CONAN_LIB_DIRS "C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"
			"C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a/lib"
			"C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/lib"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f/lib"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f/lib"
			"C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib" ${CONAN_LIB_DIRS})
set(CONAN_BIN_DIRS "C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/bin"
			"C:/Users/Daniil_Akhramiuk/.conan/data/automake/1.16.3/_/_/package/3e48e69237f7f2196164383ef9dedf0f93cbf249/bin"
			"C:/Users/Daniil_Akhramiuk/.conan/data/autoconf/2.69/_/_/package/3475bd55b91ae904ac96fde0f106a136ab951a5e/bin"
			"C:/Users/Daniil_Akhramiuk/.conan/data/m4/1.4.18/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin" ${CONAN_BIN_DIRS})
set(CONAN_RES_DIRS  ${CONAN_RES_DIRS})
set(CONAN_FRAMEWORK_DIRS  ${CONAN_FRAMEWORK_DIRS})
set(CONAN_LIBS cryptopp-static libexpatMD ltdl pqxx libpq libpgport zlib ${CONAN_LIBS})
set(CONAN_PKG_LIBS cryptopp-static libexpatMD ltdl pqxx libpq libpgport zlib ${CONAN_PKG_LIBS})
set(CONAN_SYSTEM_LIBS wsock32 ws2_32 secur32 advapi32 shell32 crypt32 wldap32 ${CONAN_SYSTEM_LIBS})
set(CONAN_FRAMEWORKS  ${CONAN_FRAMEWORKS})
set(CONAN_FRAMEWORKS_FOUND "")  # Will be filled later
set(CONAN_DEFINES "-DXML_STATIC"
			"-DASIO_STANDALONE" ${CONAN_DEFINES})
set(CONAN_BUILD_MODULES_PATHS  ${CONAN_BUILD_MODULES_PATHS})
set(CONAN_CMAKE_MODULE_PATH "C:/Users/Daniil_Akhramiuk/.conan/data/asio/1.19.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/cryptopp/8.5.0/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib/cmake"
			"C:/Users/Daniil_Akhramiuk/.conan/data/expat/2.2.10/_/_/package/ce5788ba7e3bb7dc834e36b06df66c481f42c99a/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/catch2/2.13.4/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib/cmake/Catch2"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libtool/2.4.6/_/_/package/82ad5af1557e85f11dfc0f68ec2820c275dfe6d4/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libpqxx/7.2.1/_/_/package/9608a5a69bd5ee4ebda5240c72b3c2aa9fe2a85f/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/nlohmann_json/3.9.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/yas/7.0.5/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/automake/1.16.3/_/_/package/3e48e69237f7f2196164383ef9dedf0f93cbf249/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/libpq/11.5/_/_/package/d8ac7a0f959aa6a09fd6a37c84b5d5aa5d138d1f/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/autoconf/2.69/_/_/package/3475bd55b91ae904ac96fde0f106a136ab951a5e/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/zlib/1.2.11/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/"
			"C:/Users/Daniil_Akhramiuk/.conan/data/m4/1.4.18/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/" ${CONAN_CMAKE_MODULE_PATH})

set(CONAN_CXX_FLAGS " ${CONAN_CXX_FLAGS}")
set(CONAN_SHARED_LINKER_FLAGS " ${CONAN_SHARED_LINKER_FLAGS}")
set(CONAN_EXE_LINKER_FLAGS " ${CONAN_EXE_LINKER_FLAGS}")
set(CONAN_C_FLAGS " ${CONAN_C_FLAGS}")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND "${CONAN_FRAMEWORKS}" "" "")
# Append to aggregated values variable: Use CONAN_LIBS instead of CONAN_PKG_LIBS to include user appended vars
set(CONAN_LIBS ${CONAN_LIBS} ${CONAN_SYSTEM_LIBS} ${CONAN_FRAMEWORKS_FOUND})


###  Definition of macros and functions ###

macro(conan_define_targets)
    if(${CMAKE_VERSION} VERSION_LESS "3.1.2")
        message(FATAL_ERROR "TARGETS not supported by your CMake version!")
    endif()  # CMAKE > 3.x
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CONAN_CMD_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CONAN_CMD_C_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CONAN_CMD_SHARED_LINKER_FLAGS}")


    set(_CONAN_PKG_LIBS_ASIO_DEPENDENCIES "${CONAN_SYSTEM_LIBS_ASIO} ${CONAN_FRAMEWORKS_FOUND_ASIO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ASIO_DEPENDENCIES "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ASIO}" "${CONAN_LIB_DIRS_ASIO}"
                                  CONAN_PACKAGE_TARGETS_ASIO "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES}"
                                  "" asio)
    set(_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_ASIO_DEBUG} ${CONAN_FRAMEWORKS_FOUND_ASIO_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ASIO_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ASIO_DEBUG}" "${CONAN_LIB_DIRS_ASIO_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_ASIO_DEBUG "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_DEBUG}"
                                  "debug" asio)
    set(_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_ASIO_RELEASE} ${CONAN_FRAMEWORKS_FOUND_ASIO_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ASIO_RELEASE}" "${CONAN_LIB_DIRS_ASIO_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_ASIO_RELEASE "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELEASE}"
                                  "release" asio)
    set(_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_ASIO_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_ASIO_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ASIO_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_ASIO_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_ASIO_RELWITHDEBINFO "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" asio)
    set(_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_ASIO_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_ASIO_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ASIO_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ASIO_MINSIZEREL}" "${CONAN_LIB_DIRS_ASIO_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_ASIO_MINSIZEREL "${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" asio)

    add_library(CONAN_PKG::asio INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::asio PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_ASIO} ${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ASIO_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_ASIO_RELEASE} ${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ASIO_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_ASIO_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ASIO_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_ASIO_MINSIZEREL} ${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ASIO_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_ASIO_DEBUG} ${_CONAN_PKG_LIBS_ASIO_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ASIO_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ASIO_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::asio PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_ASIO}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_ASIO_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_ASIO_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_ASIO_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_ASIO_DEBUG}>)
    set_property(TARGET CONAN_PKG::asio PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_ASIO}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_ASIO_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_ASIO_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_ASIO_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_ASIO_DEBUG}>)
    set_property(TARGET CONAN_PKG::asio PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_ASIO_LIST} ${CONAN_CXX_FLAGS_ASIO_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_ASIO_RELEASE_LIST} ${CONAN_CXX_FLAGS_ASIO_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_ASIO_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_ASIO_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_ASIO_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_ASIO_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_ASIO_DEBUG_LIST}  ${CONAN_CXX_FLAGS_ASIO_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES "${CONAN_SYSTEM_LIBS_CRYPTOPP} ${CONAN_FRAMEWORKS_FOUND_CRYPTOPP} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CRYPTOPP}" "${CONAN_LIB_DIRS_CRYPTOPP}"
                                  CONAN_PACKAGE_TARGETS_CRYPTOPP "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES}"
                                  "" cryptopp)
    set(_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_CRYPTOPP_DEBUG} ${CONAN_FRAMEWORKS_FOUND_CRYPTOPP_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CRYPTOPP_DEBUG}" "${CONAN_LIB_DIRS_CRYPTOPP_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_CRYPTOPP_DEBUG "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_DEBUG}"
                                  "debug" cryptopp)
    set(_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_CRYPTOPP_RELEASE} ${CONAN_FRAMEWORKS_FOUND_CRYPTOPP_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CRYPTOPP_RELEASE}" "${CONAN_LIB_DIRS_CRYPTOPP_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_CRYPTOPP_RELEASE "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELEASE}"
                                  "release" cryptopp)
    set(_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_CRYPTOPP_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_CRYPTOPP_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CRYPTOPP_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_CRYPTOPP_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_CRYPTOPP_RELWITHDEBINFO "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" cryptopp)
    set(_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_CRYPTOPP_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_CRYPTOPP_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CRYPTOPP_MINSIZEREL}" "${CONAN_LIB_DIRS_CRYPTOPP_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_CRYPTOPP_MINSIZEREL "${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" cryptopp)

    add_library(CONAN_PKG::cryptopp INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::cryptopp PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_CRYPTOPP} ${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CRYPTOPP_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_CRYPTOPP_RELEASE} ${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CRYPTOPP_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_CRYPTOPP_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CRYPTOPP_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_CRYPTOPP_MINSIZEREL} ${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CRYPTOPP_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_CRYPTOPP_DEBUG} ${_CONAN_PKG_LIBS_CRYPTOPP_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CRYPTOPP_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CRYPTOPP_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::cryptopp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_CRYPTOPP}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_CRYPTOPP_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_CRYPTOPP_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_CRYPTOPP_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_CRYPTOPP_DEBUG}>)
    set_property(TARGET CONAN_PKG::cryptopp PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_CRYPTOPP}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_CRYPTOPP_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_CRYPTOPP_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_CRYPTOPP_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_CRYPTOPP_DEBUG}>)
    set_property(TARGET CONAN_PKG::cryptopp PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_CRYPTOPP_LIST} ${CONAN_CXX_FLAGS_CRYPTOPP_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_CRYPTOPP_RELEASE_LIST} ${CONAN_CXX_FLAGS_CRYPTOPP_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_CRYPTOPP_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_CRYPTOPP_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_CRYPTOPP_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_CRYPTOPP_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_CRYPTOPP_DEBUG_LIST}  ${CONAN_CXX_FLAGS_CRYPTOPP_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES "${CONAN_SYSTEM_LIBS_EXPAT} ${CONAN_FRAMEWORKS_FOUND_EXPAT} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_EXPAT_DEPENDENCIES "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_EXPAT}" "${CONAN_LIB_DIRS_EXPAT}"
                                  CONAN_PACKAGE_TARGETS_EXPAT "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES}"
                                  "" expat)
    set(_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_EXPAT_DEBUG} ${CONAN_FRAMEWORKS_FOUND_EXPAT_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_EXPAT_DEBUG}" "${CONAN_LIB_DIRS_EXPAT_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_EXPAT_DEBUG "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_DEBUG}"
                                  "debug" expat)
    set(_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_EXPAT_RELEASE} ${CONAN_FRAMEWORKS_FOUND_EXPAT_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_EXPAT_RELEASE}" "${CONAN_LIB_DIRS_EXPAT_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_EXPAT_RELEASE "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELEASE}"
                                  "release" expat)
    set(_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_EXPAT_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_EXPAT_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_EXPAT_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_EXPAT_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_EXPAT_RELWITHDEBINFO "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" expat)
    set(_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_EXPAT_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_EXPAT_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_EXPAT_MINSIZEREL}" "${CONAN_LIB_DIRS_EXPAT_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_EXPAT_MINSIZEREL "${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" expat)

    add_library(CONAN_PKG::expat INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::expat PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_EXPAT} ${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_EXPAT_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_EXPAT_RELEASE} ${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_EXPAT_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_EXPAT_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_EXPAT_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_EXPAT_MINSIZEREL} ${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_EXPAT_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_EXPAT_DEBUG} ${_CONAN_PKG_LIBS_EXPAT_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_EXPAT_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_EXPAT_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::expat PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_EXPAT}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_EXPAT_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_EXPAT_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_EXPAT_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_EXPAT_DEBUG}>)
    set_property(TARGET CONAN_PKG::expat PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_EXPAT}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_EXPAT_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_EXPAT_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_EXPAT_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_EXPAT_DEBUG}>)
    set_property(TARGET CONAN_PKG::expat PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_EXPAT_LIST} ${CONAN_CXX_FLAGS_EXPAT_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_EXPAT_RELEASE_LIST} ${CONAN_CXX_FLAGS_EXPAT_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_EXPAT_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_EXPAT_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_EXPAT_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_EXPAT_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_EXPAT_DEBUG_LIST}  ${CONAN_CXX_FLAGS_EXPAT_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES "${CONAN_SYSTEM_LIBS_CATCH2} ${CONAN_FRAMEWORKS_FOUND_CATCH2} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CATCH2_DEPENDENCIES "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CATCH2}" "${CONAN_LIB_DIRS_CATCH2}"
                                  CONAN_PACKAGE_TARGETS_CATCH2 "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES}"
                                  "" catch2)
    set(_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_CATCH2_DEBUG} ${CONAN_FRAMEWORKS_FOUND_CATCH2_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CATCH2_DEBUG}" "${CONAN_LIB_DIRS_CATCH2_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_CATCH2_DEBUG "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_DEBUG}"
                                  "debug" catch2)
    set(_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_CATCH2_RELEASE} ${CONAN_FRAMEWORKS_FOUND_CATCH2_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CATCH2_RELEASE}" "${CONAN_LIB_DIRS_CATCH2_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_CATCH2_RELEASE "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELEASE}"
                                  "release" catch2)
    set(_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_CATCH2_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_CATCH2_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CATCH2_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_CATCH2_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_CATCH2_RELWITHDEBINFO "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" catch2)
    set(_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_CATCH2_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_CATCH2_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_CATCH2_MINSIZEREL}" "${CONAN_LIB_DIRS_CATCH2_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_CATCH2_MINSIZEREL "${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" catch2)

    add_library(CONAN_PKG::catch2 INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::catch2 PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_CATCH2} ${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CATCH2_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_CATCH2_RELEASE} ${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CATCH2_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_CATCH2_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CATCH2_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_CATCH2_MINSIZEREL} ${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CATCH2_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_CATCH2_DEBUG} ${_CONAN_PKG_LIBS_CATCH2_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_CATCH2_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_CATCH2_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::catch2 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_CATCH2}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_CATCH2_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_CATCH2_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_CATCH2_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_CATCH2_DEBUG}>)
    set_property(TARGET CONAN_PKG::catch2 PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_CATCH2}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_CATCH2_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_CATCH2_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_CATCH2_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_CATCH2_DEBUG}>)
    set_property(TARGET CONAN_PKG::catch2 PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_CATCH2_LIST} ${CONAN_CXX_FLAGS_CATCH2_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_CATCH2_RELEASE_LIST} ${CONAN_CXX_FLAGS_CATCH2_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_CATCH2_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_CATCH2_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_CATCH2_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_CATCH2_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_CATCH2_DEBUG_LIST}  ${CONAN_CXX_FLAGS_CATCH2_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES "${CONAN_SYSTEM_LIBS_LIBTOOL} ${CONAN_FRAMEWORKS_FOUND_LIBTOOL} CONAN_PKG::automake")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBTOOL}" "${CONAN_LIB_DIRS_LIBTOOL}"
                                  CONAN_PACKAGE_TARGETS_LIBTOOL "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES}"
                                  "" libtool)
    set(_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_LIBTOOL_DEBUG} ${CONAN_FRAMEWORKS_FOUND_LIBTOOL_DEBUG} CONAN_PKG::automake")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBTOOL_DEBUG}" "${CONAN_LIB_DIRS_LIBTOOL_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_LIBTOOL_DEBUG "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_DEBUG}"
                                  "debug" libtool)
    set(_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_LIBTOOL_RELEASE} ${CONAN_FRAMEWORKS_FOUND_LIBTOOL_RELEASE} CONAN_PKG::automake")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBTOOL_RELEASE}" "${CONAN_LIB_DIRS_LIBTOOL_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_LIBTOOL_RELEASE "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELEASE}"
                                  "release" libtool)
    set(_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_LIBTOOL_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_LIBTOOL_RELWITHDEBINFO} CONAN_PKG::automake")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBTOOL_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_LIBTOOL_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_LIBTOOL_RELWITHDEBINFO "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" libtool)
    set(_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_LIBTOOL_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_LIBTOOL_MINSIZEREL} CONAN_PKG::automake")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBTOOL_MINSIZEREL}" "${CONAN_LIB_DIRS_LIBTOOL_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_LIBTOOL_MINSIZEREL "${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" libtool)

    add_library(CONAN_PKG::libtool INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::libtool PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_LIBTOOL} ${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBTOOL_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_LIBTOOL_RELEASE} ${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBTOOL_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_LIBTOOL_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBTOOL_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_LIBTOOL_MINSIZEREL} ${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBTOOL_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_LIBTOOL_DEBUG} ${_CONAN_PKG_LIBS_LIBTOOL_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBTOOL_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBTOOL_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::libtool PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_LIBTOOL}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_LIBTOOL_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_LIBTOOL_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_LIBTOOL_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_LIBTOOL_DEBUG}>)
    set_property(TARGET CONAN_PKG::libtool PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_LIBTOOL}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_LIBTOOL_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_LIBTOOL_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_LIBTOOL_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_LIBTOOL_DEBUG}>)
    set_property(TARGET CONAN_PKG::libtool PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_LIBTOOL_LIST} ${CONAN_CXX_FLAGS_LIBTOOL_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_LIBTOOL_RELEASE_LIST} ${CONAN_CXX_FLAGS_LIBTOOL_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_LIBTOOL_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_LIBTOOL_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_LIBTOOL_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_LIBTOOL_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_LIBTOOL_DEBUG_LIST}  ${CONAN_CXX_FLAGS_LIBTOOL_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES "${CONAN_SYSTEM_LIBS_LIBPQXX} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX} CONAN_PKG::libpq")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQXX}" "${CONAN_LIB_DIRS_LIBPQXX}"
                                  CONAN_PACKAGE_TARGETS_LIBPQXX "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES}"
                                  "" libpqxx)
    set(_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_LIBPQXX_DEBUG} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX_DEBUG} CONAN_PKG::libpq")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQXX_DEBUG}" "${CONAN_LIB_DIRS_LIBPQXX_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_LIBPQXX_DEBUG "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_DEBUG}"
                                  "debug" libpqxx)
    set(_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_LIBPQXX_RELEASE} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX_RELEASE} CONAN_PKG::libpq")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQXX_RELEASE}" "${CONAN_LIB_DIRS_LIBPQXX_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_LIBPQXX_RELEASE "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELEASE}"
                                  "release" libpqxx)
    set(_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_LIBPQXX_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX_RELWITHDEBINFO} CONAN_PKG::libpq")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQXX_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_LIBPQXX_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_LIBPQXX_RELWITHDEBINFO "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" libpqxx)
    set(_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_LIBPQXX_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX_MINSIZEREL} CONAN_PKG::libpq")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQXX_MINSIZEREL}" "${CONAN_LIB_DIRS_LIBPQXX_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_LIBPQXX_MINSIZEREL "${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" libpqxx)

    add_library(CONAN_PKG::libpqxx INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::libpqxx PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_LIBPQXX} ${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQXX_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_LIBPQXX_RELEASE} ${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQXX_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_LIBPQXX_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQXX_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_LIBPQXX_MINSIZEREL} ${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQXX_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_LIBPQXX_DEBUG} ${_CONAN_PKG_LIBS_LIBPQXX_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQXX_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQXX_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::libpqxx PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_LIBPQXX}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_LIBPQXX_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_LIBPQXX_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_LIBPQXX_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_LIBPQXX_DEBUG}>)
    set_property(TARGET CONAN_PKG::libpqxx PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_LIBPQXX}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_LIBPQXX_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_LIBPQXX_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_LIBPQXX_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_LIBPQXX_DEBUG}>)
    set_property(TARGET CONAN_PKG::libpqxx PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_LIBPQXX_LIST} ${CONAN_CXX_FLAGS_LIBPQXX_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_LIBPQXX_RELEASE_LIST} ${CONAN_CXX_FLAGS_LIBPQXX_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_LIBPQXX_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_LIBPQXX_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_LIBPQXX_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_LIBPQXX_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_LIBPQXX_DEBUG_LIST}  ${CONAN_CXX_FLAGS_LIBPQXX_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES "${CONAN_SYSTEM_LIBS_NLOHMANN_JSON} ${CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_NLOHMANN_JSON}" "${CONAN_LIB_DIRS_NLOHMANN_JSON}"
                                  CONAN_PACKAGE_TARGETS_NLOHMANN_JSON "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES}"
                                  "" nlohmann_json)
    set(_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_NLOHMANN_JSON_DEBUG} ${CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_NLOHMANN_JSON_DEBUG}" "${CONAN_LIB_DIRS_NLOHMANN_JSON_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_DEBUG "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_DEBUG}"
                                  "debug" nlohmann_json)
    set(_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_NLOHMANN_JSON_RELEASE} ${CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_NLOHMANN_JSON_RELEASE}" "${CONAN_LIB_DIRS_NLOHMANN_JSON_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_RELEASE "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELEASE}"
                                  "release" nlohmann_json)
    set(_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_NLOHMANN_JSON_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_NLOHMANN_JSON_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_NLOHMANN_JSON_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_RELWITHDEBINFO "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" nlohmann_json)
    set(_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_NLOHMANN_JSON_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_NLOHMANN_JSON_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_NLOHMANN_JSON_MINSIZEREL}" "${CONAN_LIB_DIRS_NLOHMANN_JSON_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_MINSIZEREL "${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" nlohmann_json)

    add_library(CONAN_PKG::nlohmann_json INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::nlohmann_json PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_NLOHMANN_JSON} ${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_RELEASE} ${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_MINSIZEREL} ${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_NLOHMANN_JSON_DEBUG} ${_CONAN_PKG_LIBS_NLOHMANN_JSON_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_NLOHMANN_JSON_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_NLOHMANN_JSON_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::nlohmann_json PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_NLOHMANN_JSON_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_NLOHMANN_JSON_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_NLOHMANN_JSON_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_NLOHMANN_JSON_DEBUG}>)
    set_property(TARGET CONAN_PKG::nlohmann_json PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_NLOHMANN_JSON}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_NLOHMANN_JSON_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_NLOHMANN_JSON_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_NLOHMANN_JSON_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_NLOHMANN_JSON_DEBUG}>)
    set_property(TARGET CONAN_PKG::nlohmann_json PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_NLOHMANN_JSON_LIST} ${CONAN_CXX_FLAGS_NLOHMANN_JSON_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_NLOHMANN_JSON_RELEASE_LIST} ${CONAN_CXX_FLAGS_NLOHMANN_JSON_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_NLOHMANN_JSON_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_NLOHMANN_JSON_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_NLOHMANN_JSON_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_NLOHMANN_JSON_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_NLOHMANN_JSON_DEBUG_LIST}  ${CONAN_CXX_FLAGS_NLOHMANN_JSON_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_YAS_DEPENDENCIES "${CONAN_SYSTEM_LIBS_YAS} ${CONAN_FRAMEWORKS_FOUND_YAS} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_YAS_DEPENDENCIES "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_YAS}" "${CONAN_LIB_DIRS_YAS}"
                                  CONAN_PACKAGE_TARGETS_YAS "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES}"
                                  "" yas)
    set(_CONAN_PKG_LIBS_YAS_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_YAS_DEBUG} ${CONAN_FRAMEWORKS_FOUND_YAS_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_YAS_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_YAS_DEBUG}" "${CONAN_LIB_DIRS_YAS_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_YAS_DEBUG "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_DEBUG}"
                                  "debug" yas)
    set(_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_YAS_RELEASE} ${CONAN_FRAMEWORKS_FOUND_YAS_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_YAS_RELEASE}" "${CONAN_LIB_DIRS_YAS_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_YAS_RELEASE "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELEASE}"
                                  "release" yas)
    set(_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_YAS_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_YAS_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_YAS_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_YAS_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_YAS_RELWITHDEBINFO "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" yas)
    set(_CONAN_PKG_LIBS_YAS_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_YAS_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_YAS_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_YAS_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_YAS_MINSIZEREL}" "${CONAN_LIB_DIRS_YAS_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_YAS_MINSIZEREL "${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" yas)

    add_library(CONAN_PKG::yas INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::yas PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_YAS} ${_CONAN_PKG_LIBS_YAS_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_YAS_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_YAS_RELEASE} ${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_YAS_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_YAS_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_YAS_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_YAS_MINSIZEREL} ${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_YAS_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_YAS_DEBUG} ${_CONAN_PKG_LIBS_YAS_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_YAS_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_YAS_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::yas PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_YAS}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_YAS_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_YAS_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_YAS_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_YAS_DEBUG}>)
    set_property(TARGET CONAN_PKG::yas PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_YAS}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_YAS_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_YAS_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_YAS_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_YAS_DEBUG}>)
    set_property(TARGET CONAN_PKG::yas PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_YAS_LIST} ${CONAN_CXX_FLAGS_YAS_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_YAS_RELEASE_LIST} ${CONAN_CXX_FLAGS_YAS_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_YAS_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_YAS_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_YAS_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_YAS_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_YAS_DEBUG_LIST}  ${CONAN_CXX_FLAGS_YAS_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES "${CONAN_SYSTEM_LIBS_AUTOMAKE} ${CONAN_FRAMEWORKS_FOUND_AUTOMAKE} CONAN_PKG::autoconf")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOMAKE}" "${CONAN_LIB_DIRS_AUTOMAKE}"
                                  CONAN_PACKAGE_TARGETS_AUTOMAKE "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES}"
                                  "" automake)
    set(_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_AUTOMAKE_DEBUG} ${CONAN_FRAMEWORKS_FOUND_AUTOMAKE_DEBUG} CONAN_PKG::autoconf")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOMAKE_DEBUG}" "${CONAN_LIB_DIRS_AUTOMAKE_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_AUTOMAKE_DEBUG "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_DEBUG}"
                                  "debug" automake)
    set(_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_AUTOMAKE_RELEASE} ${CONAN_FRAMEWORKS_FOUND_AUTOMAKE_RELEASE} CONAN_PKG::autoconf")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOMAKE_RELEASE}" "${CONAN_LIB_DIRS_AUTOMAKE_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_AUTOMAKE_RELEASE "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELEASE}"
                                  "release" automake)
    set(_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_AUTOMAKE_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_AUTOMAKE_RELWITHDEBINFO} CONAN_PKG::autoconf")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOMAKE_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_AUTOMAKE_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_AUTOMAKE_RELWITHDEBINFO "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" automake)
    set(_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_AUTOMAKE_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_AUTOMAKE_MINSIZEREL} CONAN_PKG::autoconf")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOMAKE_MINSIZEREL}" "${CONAN_LIB_DIRS_AUTOMAKE_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_AUTOMAKE_MINSIZEREL "${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" automake)

    add_library(CONAN_PKG::automake INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::automake PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_AUTOMAKE} ${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOMAKE_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_AUTOMAKE_RELEASE} ${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOMAKE_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_AUTOMAKE_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOMAKE_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_AUTOMAKE_MINSIZEREL} ${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOMAKE_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_AUTOMAKE_DEBUG} ${_CONAN_PKG_LIBS_AUTOMAKE_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOMAKE_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOMAKE_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::automake PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_AUTOMAKE}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_AUTOMAKE_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_AUTOMAKE_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_AUTOMAKE_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_AUTOMAKE_DEBUG}>)
    set_property(TARGET CONAN_PKG::automake PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_AUTOMAKE}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_AUTOMAKE_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_AUTOMAKE_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_AUTOMAKE_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_AUTOMAKE_DEBUG}>)
    set_property(TARGET CONAN_PKG::automake PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_AUTOMAKE_LIST} ${CONAN_CXX_FLAGS_AUTOMAKE_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_AUTOMAKE_RELEASE_LIST} ${CONAN_CXX_FLAGS_AUTOMAKE_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_AUTOMAKE_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_AUTOMAKE_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_AUTOMAKE_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_AUTOMAKE_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_AUTOMAKE_DEBUG_LIST}  ${CONAN_CXX_FLAGS_AUTOMAKE_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES "${CONAN_SYSTEM_LIBS_LIBPQ} ${CONAN_FRAMEWORKS_FOUND_LIBPQ} CONAN_PKG::zlib")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQ}" "${CONAN_LIB_DIRS_LIBPQ}"
                                  CONAN_PACKAGE_TARGETS_LIBPQ "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES}"
                                  "" libpq)
    set(_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_LIBPQ_DEBUG} ${CONAN_FRAMEWORKS_FOUND_LIBPQ_DEBUG} CONAN_PKG::zlib")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQ_DEBUG}" "${CONAN_LIB_DIRS_LIBPQ_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_LIBPQ_DEBUG "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_DEBUG}"
                                  "debug" libpq)
    set(_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_LIBPQ_RELEASE} ${CONAN_FRAMEWORKS_FOUND_LIBPQ_RELEASE} CONAN_PKG::zlib")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQ_RELEASE}" "${CONAN_LIB_DIRS_LIBPQ_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_LIBPQ_RELEASE "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELEASE}"
                                  "release" libpq)
    set(_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_LIBPQ_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_LIBPQ_RELWITHDEBINFO} CONAN_PKG::zlib")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQ_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_LIBPQ_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_LIBPQ_RELWITHDEBINFO "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" libpq)
    set(_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_LIBPQ_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_LIBPQ_MINSIZEREL} CONAN_PKG::zlib")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_LIBPQ_MINSIZEREL}" "${CONAN_LIB_DIRS_LIBPQ_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_LIBPQ_MINSIZEREL "${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" libpq)

    add_library(CONAN_PKG::libpq INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::libpq PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_LIBPQ} ${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQ_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_LIBPQ_RELEASE} ${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQ_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_LIBPQ_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQ_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_LIBPQ_MINSIZEREL} ${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQ_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_LIBPQ_DEBUG} ${_CONAN_PKG_LIBS_LIBPQ_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_LIBPQ_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_LIBPQ_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::libpq PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_LIBPQ}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_LIBPQ_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_LIBPQ_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_LIBPQ_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_LIBPQ_DEBUG}>)
    set_property(TARGET CONAN_PKG::libpq PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_LIBPQ}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_LIBPQ_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_LIBPQ_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_LIBPQ_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_LIBPQ_DEBUG}>)
    set_property(TARGET CONAN_PKG::libpq PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_LIBPQ_LIST} ${CONAN_CXX_FLAGS_LIBPQ_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_LIBPQ_RELEASE_LIST} ${CONAN_CXX_FLAGS_LIBPQ_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_LIBPQ_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_LIBPQ_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_LIBPQ_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_LIBPQ_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_LIBPQ_DEBUG_LIST}  ${CONAN_CXX_FLAGS_LIBPQ_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES "${CONAN_SYSTEM_LIBS_AUTOCONF} ${CONAN_FRAMEWORKS_FOUND_AUTOCONF} CONAN_PKG::m4")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOCONF}" "${CONAN_LIB_DIRS_AUTOCONF}"
                                  CONAN_PACKAGE_TARGETS_AUTOCONF "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES}"
                                  "" autoconf)
    set(_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_AUTOCONF_DEBUG} ${CONAN_FRAMEWORKS_FOUND_AUTOCONF_DEBUG} CONAN_PKG::m4")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOCONF_DEBUG}" "${CONAN_LIB_DIRS_AUTOCONF_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_AUTOCONF_DEBUG "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_DEBUG}"
                                  "debug" autoconf)
    set(_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_AUTOCONF_RELEASE} ${CONAN_FRAMEWORKS_FOUND_AUTOCONF_RELEASE} CONAN_PKG::m4")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOCONF_RELEASE}" "${CONAN_LIB_DIRS_AUTOCONF_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_AUTOCONF_RELEASE "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELEASE}"
                                  "release" autoconf)
    set(_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_AUTOCONF_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_AUTOCONF_RELWITHDEBINFO} CONAN_PKG::m4")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOCONF_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_AUTOCONF_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_AUTOCONF_RELWITHDEBINFO "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" autoconf)
    set(_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_AUTOCONF_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_AUTOCONF_MINSIZEREL} CONAN_PKG::m4")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_AUTOCONF_MINSIZEREL}" "${CONAN_LIB_DIRS_AUTOCONF_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_AUTOCONF_MINSIZEREL "${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" autoconf)

    add_library(CONAN_PKG::autoconf INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::autoconf PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_AUTOCONF} ${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOCONF_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_AUTOCONF_RELEASE} ${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOCONF_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_AUTOCONF_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOCONF_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_AUTOCONF_MINSIZEREL} ${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOCONF_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_AUTOCONF_DEBUG} ${_CONAN_PKG_LIBS_AUTOCONF_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_AUTOCONF_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_AUTOCONF_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::autoconf PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_AUTOCONF}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_AUTOCONF_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_AUTOCONF_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_AUTOCONF_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_AUTOCONF_DEBUG}>)
    set_property(TARGET CONAN_PKG::autoconf PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_AUTOCONF}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_AUTOCONF_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_AUTOCONF_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_AUTOCONF_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_AUTOCONF_DEBUG}>)
    set_property(TARGET CONAN_PKG::autoconf PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_AUTOCONF_LIST} ${CONAN_CXX_FLAGS_AUTOCONF_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_AUTOCONF_RELEASE_LIST} ${CONAN_CXX_FLAGS_AUTOCONF_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_AUTOCONF_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_AUTOCONF_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_AUTOCONF_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_AUTOCONF_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_AUTOCONF_DEBUG_LIST}  ${CONAN_CXX_FLAGS_AUTOCONF_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES "${CONAN_SYSTEM_LIBS_ZLIB} ${CONAN_FRAMEWORKS_FOUND_ZLIB} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ZLIB_DEPENDENCIES "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ZLIB}" "${CONAN_LIB_DIRS_ZLIB}"
                                  CONAN_PACKAGE_TARGETS_ZLIB "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES}"
                                  "" zlib)
    set(_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_ZLIB_DEBUG} ${CONAN_FRAMEWORKS_FOUND_ZLIB_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ZLIB_DEBUG}" "${CONAN_LIB_DIRS_ZLIB_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_ZLIB_DEBUG "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_DEBUG}"
                                  "debug" zlib)
    set(_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_ZLIB_RELEASE} ${CONAN_FRAMEWORKS_FOUND_ZLIB_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ZLIB_RELEASE}" "${CONAN_LIB_DIRS_ZLIB_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_ZLIB_RELEASE "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELEASE}"
                                  "release" zlib)
    set(_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_ZLIB_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_ZLIB_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ZLIB_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_ZLIB_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_ZLIB_RELWITHDEBINFO "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" zlib)
    set(_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_ZLIB_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_ZLIB_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_ZLIB_MINSIZEREL}" "${CONAN_LIB_DIRS_ZLIB_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_ZLIB_MINSIZEREL "${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" zlib)

    add_library(CONAN_PKG::zlib INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_ZLIB} ${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ZLIB_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_ZLIB_RELEASE} ${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ZLIB_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_ZLIB_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ZLIB_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_ZLIB_MINSIZEREL} ${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ZLIB_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_ZLIB_DEBUG} ${_CONAN_PKG_LIBS_ZLIB_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_ZLIB_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_ZLIB_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_ZLIB}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_ZLIB_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_ZLIB_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_ZLIB_DEBUG}>)
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_ZLIB}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_ZLIB_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_ZLIB_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_ZLIB_DEBUG}>)
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_ZLIB_LIST} ${CONAN_CXX_FLAGS_ZLIB_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_CXX_FLAGS_ZLIB_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_ZLIB_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_ZLIB_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_ZLIB_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_ZLIB_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_ZLIB_DEBUG_LIST}  ${CONAN_CXX_FLAGS_ZLIB_DEBUG_LIST}>)


    set(_CONAN_PKG_LIBS_M4_DEPENDENCIES "${CONAN_SYSTEM_LIBS_M4} ${CONAN_FRAMEWORKS_FOUND_M4} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_M4_DEPENDENCIES "${_CONAN_PKG_LIBS_M4_DEPENDENCIES}")
    conan_package_library_targets("${CONAN_PKG_LIBS_M4}" "${CONAN_LIB_DIRS_M4}"
                                  CONAN_PACKAGE_TARGETS_M4 "${_CONAN_PKG_LIBS_M4_DEPENDENCIES}"
                                  "" m4)
    set(_CONAN_PKG_LIBS_M4_DEPENDENCIES_DEBUG "${CONAN_SYSTEM_LIBS_M4_DEBUG} ${CONAN_FRAMEWORKS_FOUND_M4_DEBUG} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_M4_DEPENDENCIES_DEBUG "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_DEBUG}")
    conan_package_library_targets("${CONAN_PKG_LIBS_M4_DEBUG}" "${CONAN_LIB_DIRS_M4_DEBUG}"
                                  CONAN_PACKAGE_TARGETS_M4_DEBUG "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_DEBUG}"
                                  "debug" m4)
    set(_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELEASE "${CONAN_SYSTEM_LIBS_M4_RELEASE} ${CONAN_FRAMEWORKS_FOUND_M4_RELEASE} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_M4_DEPENDENCIES_RELEASE "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELEASE}")
    conan_package_library_targets("${CONAN_PKG_LIBS_M4_RELEASE}" "${CONAN_LIB_DIRS_M4_RELEASE}"
                                  CONAN_PACKAGE_TARGETS_M4_RELEASE "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELEASE}"
                                  "release" m4)
    set(_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELWITHDEBINFO "${CONAN_SYSTEM_LIBS_M4_RELWITHDEBINFO} ${CONAN_FRAMEWORKS_FOUND_M4_RELWITHDEBINFO} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_M4_DEPENDENCIES_RELWITHDEBINFO "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELWITHDEBINFO}")
    conan_package_library_targets("${CONAN_PKG_LIBS_M4_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_M4_RELWITHDEBINFO}"
                                  CONAN_PACKAGE_TARGETS_M4_RELWITHDEBINFO "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELWITHDEBINFO}"
                                  "relwithdebinfo" m4)
    set(_CONAN_PKG_LIBS_M4_DEPENDENCIES_MINSIZEREL "${CONAN_SYSTEM_LIBS_M4_MINSIZEREL} ${CONAN_FRAMEWORKS_FOUND_M4_MINSIZEREL} ")
    string(REPLACE " " ";" _CONAN_PKG_LIBS_M4_DEPENDENCIES_MINSIZEREL "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_MINSIZEREL}")
    conan_package_library_targets("${CONAN_PKG_LIBS_M4_MINSIZEREL}" "${CONAN_LIB_DIRS_M4_MINSIZEREL}"
                                  CONAN_PACKAGE_TARGETS_M4_MINSIZEREL "${_CONAN_PKG_LIBS_M4_DEPENDENCIES_MINSIZEREL}"
                                  "minsizerel" m4)

    add_library(CONAN_PKG::m4 INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::m4 PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_PACKAGE_TARGETS_M4} ${_CONAN_PKG_LIBS_M4_DEPENDENCIES}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_M4_LIST}>

                                                                 $<$<CONFIG:Release>:${CONAN_PACKAGE_TARGETS_M4_RELEASE} ${_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELEASE}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_RELEASE_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_M4_RELEASE_LIST}>>

                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_PACKAGE_TARGETS_M4_RELWITHDEBINFO} ${_CONAN_PKG_LIBS_M4_DEPENDENCIES_RELWITHDEBINFO}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_RELWITHDEBINFO_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_M4_RELWITHDEBINFO_LIST}>>

                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_PACKAGE_TARGETS_M4_MINSIZEREL} ${_CONAN_PKG_LIBS_M4_DEPENDENCIES_MINSIZEREL}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_MINSIZEREL_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_M4_MINSIZEREL_LIST}>>

                                                                 $<$<CONFIG:Debug>:${CONAN_PACKAGE_TARGETS_M4_DEBUG} ${_CONAN_PKG_LIBS_M4_DEPENDENCIES_DEBUG}
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${CONAN_SHARED_LINKER_FLAGS_M4_DEBUG_LIST}>
                                                                 $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CONAN_EXE_LINKER_FLAGS_M4_DEBUG_LIST}>>)
    set_property(TARGET CONAN_PKG::m4 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_M4}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_M4_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_M4_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_M4_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_M4_DEBUG}>)
    set_property(TARGET CONAN_PKG::m4 PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_M4}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_M4_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_M4_RELWITHDEBINFO}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_M4_MINSIZEREL}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_M4_DEBUG}>)
    set_property(TARGET CONAN_PKG::m4 PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_M4_LIST} ${CONAN_CXX_FLAGS_M4_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_M4_RELEASE_LIST} ${CONAN_CXX_FLAGS_M4_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_M4_RELWITHDEBINFO_LIST} ${CONAN_CXX_FLAGS_M4_RELWITHDEBINFO_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_M4_MINSIZEREL_LIST} ${CONAN_CXX_FLAGS_M4_MINSIZEREL_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_M4_DEBUG_LIST}  ${CONAN_CXX_FLAGS_M4_DEBUG_LIST}>)

    set(CONAN_TARGETS CONAN_PKG::asio CONAN_PKG::cryptopp CONAN_PKG::expat CONAN_PKG::catch2 CONAN_PKG::libtool CONAN_PKG::libpqxx CONAN_PKG::nlohmann_json CONAN_PKG::yas CONAN_PKG::automake CONAN_PKG::libpq CONAN_PKG::autoconf CONAN_PKG::zlib CONAN_PKG::m4)

endmacro()


macro(conan_basic_setup)
    set(options TARGETS NO_OUTPUT_DIRS SKIP_RPATH KEEP_RPATHS SKIP_STD SKIP_FPIC)
    cmake_parse_arguments(ARGUMENTS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if(CONAN_EXPORTED)
        conan_message(STATUS "Conan: called by CMake conan helper")
    endif()

    if(CONAN_IN_LOCAL_CACHE)
        conan_message(STATUS "Conan: called inside local cache")
    endif()

    if(NOT ARGUMENTS_NO_OUTPUT_DIRS)
        conan_message(STATUS "Conan: Adjusting output directories")
        conan_output_dirs_setup()
    endif()

    if(NOT ARGUMENTS_TARGETS)
        conan_message(STATUS "Conan: Using cmake global configuration")
        conan_global_flags()
    else()
        conan_message(STATUS "Conan: Using cmake targets configuration")
        conan_define_targets()
    endif()

    if(ARGUMENTS_SKIP_RPATH)
        # Change by "DEPRECATION" or "SEND_ERROR" when we are ready
        conan_message(WARNING "Conan: SKIP_RPATH is deprecated, it has been renamed to KEEP_RPATHS")
    endif()

    if(NOT ARGUMENTS_SKIP_RPATH AND NOT ARGUMENTS_KEEP_RPATHS)
        # Parameter has renamed, but we keep the compatibility with old SKIP_RPATH
        conan_set_rpath()
    endif()

    if(NOT ARGUMENTS_SKIP_STD)
        conan_set_std()
    endif()

    if(NOT ARGUMENTS_SKIP_FPIC)
        conan_set_fpic()
    endif()

    conan_check_compiler()
    conan_set_libcxx()
    conan_set_vs_runtime()
    conan_set_find_paths()
    conan_include_build_modules()
    conan_set_find_library_paths()
endmacro()


macro(conan_set_find_paths)
    # CMAKE_MODULE_PATH does not have Debug/Release config, but there are variables
    # CONAN_CMAKE_MODULE_PATH_DEBUG to be used by the consumer
    # CMake can find findXXX.cmake files in the root of packages
    set(CMAKE_MODULE_PATH ${CONAN_CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})

    # Make find_package() to work
    set(CMAKE_PREFIX_PATH ${CONAN_CMAKE_MODULE_PATH} ${CMAKE_PREFIX_PATH})

    # Set the find root path (cross build)
    set(CMAKE_FIND_ROOT_PATH ${CONAN_CMAKE_FIND_ROOT_PATH} ${CMAKE_FIND_ROOT_PATH})
    if(CONAN_CMAKE_FIND_ROOT_PATH_MODE_PROGRAM)
        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ${CONAN_CMAKE_FIND_ROOT_PATH_MODE_PROGRAM})
    endif()
    if(CONAN_CMAKE_FIND_ROOT_PATH_MODE_LIBRARY)
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ${CONAN_CMAKE_FIND_ROOT_PATH_MODE_LIBRARY})
    endif()
    if(CONAN_CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ${CONAN_CMAKE_FIND_ROOT_PATH_MODE_INCLUDE})
    endif()
endmacro()


macro(conan_set_find_library_paths)
    # CMAKE_INCLUDE_PATH, CMAKE_LIBRARY_PATH does not have Debug/Release config, but there are variables
    # CONAN_INCLUDE_DIRS_DEBUG/RELEASE CONAN_LIB_DIRS_DEBUG/RELEASE to be used by the consumer
    # For find_library
    set(CMAKE_INCLUDE_PATH ${CONAN_INCLUDE_DIRS} ${CMAKE_INCLUDE_PATH})
    set(CMAKE_LIBRARY_PATH ${CONAN_LIB_DIRS} ${CMAKE_LIBRARY_PATH})
endmacro()


macro(conan_set_vs_runtime)
    if(CONAN_LINK_RUNTIME)
        conan_get_policy(CMP0091 policy_0091)
        if(policy_0091 STREQUAL "NEW")
            if(CONAN_LINK_RUNTIME MATCHES "MTd")
                set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
            elseif(CONAN_LINK_RUNTIME MATCHES "MDd")
                set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
            elseif(CONAN_LINK_RUNTIME MATCHES "MT")
                set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
            elseif(CONAN_LINK_RUNTIME MATCHES "MD")
                set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
            endif()
        else()
            foreach(flag CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE
                         CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO
                         CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL)
                if(DEFINED ${flag})
                    string(REPLACE "/MD" ${CONAN_LINK_RUNTIME} ${flag} "${${flag}}")
                endif()
            endforeach()
            foreach(flag CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG)
                if(DEFINED ${flag})
                    string(REPLACE "/MDd" ${CONAN_LINK_RUNTIME} ${flag} "${${flag}}")
                endif()
            endforeach()
        endif()
    endif()
endmacro()


macro(conan_flags_setup)
    # Macro maintained for backwards compatibility
    conan_set_find_library_paths()
    conan_global_flags()
    conan_set_rpath()
    conan_set_vs_runtime()
    conan_set_libcxx()
endmacro()


function(conan_message MESSAGE_OUTPUT)
    if(NOT CONAN_CMAKE_SILENT_OUTPUT)
        message(${ARGV${0}})
    endif()
endfunction()


function(conan_get_policy policy_id policy)
    if(POLICY "${policy_id}")
        cmake_policy(GET "${policy_id}" _policy)
        set(${policy} "${_policy}" PARENT_SCOPE)
    else()
        set(${policy} "" PARENT_SCOPE)
    endif()
endfunction()


function(conan_find_libraries_abs_path libraries package_libdir libraries_abs_path)
    foreach(_LIBRARY_NAME ${libraries})
        find_library(CONAN_FOUND_LIBRARY NAME ${_LIBRARY_NAME} PATHS ${package_libdir}
                     NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
        if(CONAN_FOUND_LIBRARY)
            conan_message(STATUS "Library ${_LIBRARY_NAME} found ${CONAN_FOUND_LIBRARY}")
            set(CONAN_FULLPATH_LIBS ${CONAN_FULLPATH_LIBS} ${CONAN_FOUND_LIBRARY})
        else()
            conan_message(STATUS "Library ${_LIBRARY_NAME} not found in package, might be system one")
            set(CONAN_FULLPATH_LIBS ${CONAN_FULLPATH_LIBS} ${_LIBRARY_NAME})
        endif()
        unset(CONAN_FOUND_LIBRARY CACHE)
    endforeach()
    set(${libraries_abs_path} ${CONAN_FULLPATH_LIBS} PARENT_SCOPE)
endfunction()


function(conan_package_library_targets libraries package_libdir libraries_abs_path deps build_type package_name)
    unset(_CONAN_ACTUAL_TARGETS CACHE)
    unset(_CONAN_FOUND_SYSTEM_LIBS CACHE)
    foreach(_LIBRARY_NAME ${libraries})
        find_library(CONAN_FOUND_LIBRARY NAME ${_LIBRARY_NAME} PATHS ${package_libdir}
                     NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
        if(CONAN_FOUND_LIBRARY)
            conan_message(STATUS "Library ${_LIBRARY_NAME} found ${CONAN_FOUND_LIBRARY}")
            set(_LIB_NAME CONAN_LIB::${package_name}_${_LIBRARY_NAME}${build_type})
            add_library(${_LIB_NAME} UNKNOWN IMPORTED)
            set_target_properties(${_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${CONAN_FOUND_LIBRARY})
            set(CONAN_FULLPATH_LIBS ${CONAN_FULLPATH_LIBS} ${_LIB_NAME})
            set(_CONAN_ACTUAL_TARGETS ${_CONAN_ACTUAL_TARGETS} ${_LIB_NAME})
        else()
            conan_message(STATUS "Library ${_LIBRARY_NAME} not found in package, might be system one")
            set(CONAN_FULLPATH_LIBS ${CONAN_FULLPATH_LIBS} ${_LIBRARY_NAME})
            set(_CONAN_FOUND_SYSTEM_LIBS "${_CONAN_FOUND_SYSTEM_LIBS};${_LIBRARY_NAME}")
        endif()
        unset(CONAN_FOUND_LIBRARY CACHE)
    endforeach()

    # Add all dependencies to all targets
    string(REPLACE " " ";" deps_list "${deps}")
    foreach(_CONAN_ACTUAL_TARGET ${_CONAN_ACTUAL_TARGETS})
        set_property(TARGET ${_CONAN_ACTUAL_TARGET} PROPERTY INTERFACE_LINK_LIBRARIES "${_CONAN_FOUND_SYSTEM_LIBS};${deps_list}")
    endforeach()

    set(${libraries_abs_path} ${CONAN_FULLPATH_LIBS} PARENT_SCOPE)
endfunction()


macro(conan_set_libcxx)
    if(DEFINED CONAN_LIBCXX)
        conan_message(STATUS "Conan: C++ stdlib: ${CONAN_LIBCXX}")
        if(CONAN_COMPILER STREQUAL "clang" OR CONAN_COMPILER STREQUAL "apple-clang")
            if(CONAN_LIBCXX STREQUAL "libstdc++" OR CONAN_LIBCXX STREQUAL "libstdc++11" )
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
            elseif(CONAN_LIBCXX STREQUAL "libc++")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
            endif()
        endif()
        if(CONAN_COMPILER STREQUAL "sun-cc")
            if(CONAN_LIBCXX STREQUAL "libCstd")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=Cstd")
            elseif(CONAN_LIBCXX STREQUAL "libstdcxx")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stdcxx4")
            elseif(CONAN_LIBCXX STREQUAL "libstlport")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stlport4")
            elseif(CONAN_LIBCXX STREQUAL "libstdc++")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stdcpp")
            endif()
        endif()
        if(CONAN_LIBCXX STREQUAL "libstdc++11")
            add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
        elseif(CONAN_LIBCXX STREQUAL "libstdc++")
            add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
        endif()
    endif()
endmacro()


macro(conan_set_std)
    conan_message(STATUS "Conan: Adjusting language standard")
    # Do not warn "Manually-specified variables were not used by the project"
    set(ignorevar "${CONAN_STD_CXX_FLAG}${CONAN_CMAKE_CXX_STANDARD}${CONAN_CMAKE_CXX_EXTENSIONS}")
    if (CMAKE_VERSION VERSION_LESS "3.1" OR
        (CMAKE_VERSION VERSION_LESS "3.12" AND ("${CONAN_CMAKE_CXX_STANDARD}" STREQUAL "20" OR "${CONAN_CMAKE_CXX_STANDARD}" STREQUAL "gnu20")))
        if(CONAN_STD_CXX_FLAG)
            conan_message(STATUS "Conan setting CXX_FLAGS flags: ${CONAN_STD_CXX_FLAG}")
            set(CMAKE_CXX_FLAGS "${CONAN_STD_CXX_FLAG} ${CMAKE_CXX_FLAGS}")
        endif()
    else()
        if(CONAN_CMAKE_CXX_STANDARD)
            conan_message(STATUS "Conan setting CPP STANDARD: ${CONAN_CMAKE_CXX_STANDARD} WITH EXTENSIONS ${CONAN_CMAKE_CXX_EXTENSIONS}")
            set(CMAKE_CXX_STANDARD ${CONAN_CMAKE_CXX_STANDARD})
            set(CMAKE_CXX_EXTENSIONS ${CONAN_CMAKE_CXX_EXTENSIONS})
        endif()
    endif()
endmacro()


macro(conan_set_rpath)
    conan_message(STATUS "Conan: Adjusting default RPATHs Conan policies")
    if(APPLE)
        # https://cmake.org/Wiki/CMake_RPATH_handling
        # CONAN GUIDE: All generated libraries should have the id and dependencies to other
        # dylibs without path, just the name, EX:
        # libMyLib1.dylib:
        #     libMyLib1.dylib (compatibility version 0.0.0, current version 0.0.0)
        #     libMyLib0.dylib (compatibility version 0.0.0, current version 0.0.0)
        #     /usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 120.0.0)
        #     /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1197.1.1)
        # AVOID RPATH FOR *.dylib, ALL LIBS BETWEEN THEM AND THE EXE
        # SHOULD BE ON THE LINKER RESOLVER PATH (./ IS ONE OF THEM)
        set(CMAKE_SKIP_RPATH 1 CACHE BOOL "rpaths" FORCE)
        # Policy CMP0068
        # We want the old behavior, in CMake >= 3.9 CMAKE_SKIP_RPATH won't affect the install_name in OSX
        set(CMAKE_INSTALL_NAME_DIR "")
    endif()
endmacro()


macro(conan_set_fpic)
    if(DEFINED CONAN_CMAKE_POSITION_INDEPENDENT_CODE)
        conan_message(STATUS "Conan: Adjusting fPIC flag (${CONAN_CMAKE_POSITION_INDEPENDENT_CODE})")
        set(CMAKE_POSITION_INDEPENDENT_CODE ${CONAN_CMAKE_POSITION_INDEPENDENT_CODE})
    endif()
endmacro()


macro(conan_output_dirs_setup)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endmacro()


macro(conan_split_version VERSION_STRING MAJOR MINOR)
    #make a list from the version string
    string(REPLACE "." ";" VERSION_LIST "${VERSION_STRING}")

    #write output values
    list(LENGTH VERSION_LIST _version_len)
    list(GET VERSION_LIST 0 ${MAJOR})
    if(${_version_len} GREATER 1)
        list(GET VERSION_LIST 1 ${MINOR})
    endif()
endmacro()


macro(conan_error_compiler_version)
    message(FATAL_ERROR "Detected a mismatch for the compiler version between your conan profile settings and CMake: \n"
                        "Compiler version specified in your conan profile: ${CONAN_COMPILER_VERSION}\n"
                        "Compiler version detected in CMake: ${VERSION_MAJOR}.${VERSION_MINOR}\n"
                        "Please check your conan profile settings (conan profile show [default|your_profile_name])\n"
                        "P.S. You may set CONAN_DISABLE_CHECK_COMPILER CMake variable in order to disable this check."
           )
endmacro()

set(_CONAN_CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})

function(conan_get_compiler CONAN_INFO_COMPILER CONAN_INFO_COMPILER_VERSION)
    conan_message(STATUS "Current conanbuildinfo.cmake directory: " ${_CONAN_CURRENT_DIR})
    if(NOT EXISTS ${_CONAN_CURRENT_DIR}/conaninfo.txt)
        conan_message(STATUS "WARN: conaninfo.txt not found")
        return()
    endif()

    file (READ "${_CONAN_CURRENT_DIR}/conaninfo.txt" CONANINFO)

    # MATCHALL will match all, including the last one, which is the full_settings one
    string(REGEX MATCH "full_settings.*" _FULL_SETTINGS_MATCHED ${CONANINFO})
    string(REGEX MATCH "compiler=([-A-Za-z0-9_ ]+)" _MATCHED ${_FULL_SETTINGS_MATCHED})
    if(DEFINED CMAKE_MATCH_1)
        string(STRIP "${CMAKE_MATCH_1}" _CONAN_INFO_COMPILER)
        set(${CONAN_INFO_COMPILER} ${_CONAN_INFO_COMPILER} PARENT_SCOPE)
    endif()

    string(REGEX MATCH "compiler.version=([-A-Za-z0-9_.]+)" _MATCHED ${_FULL_SETTINGS_MATCHED})
    if(DEFINED CMAKE_MATCH_1)
        string(STRIP "${CMAKE_MATCH_1}" _CONAN_INFO_COMPILER_VERSION)
        set(${CONAN_INFO_COMPILER_VERSION} ${_CONAN_INFO_COMPILER_VERSION} PARENT_SCOPE)
    endif()
endfunction()


function(check_compiler_version)
    conan_split_version(${CMAKE_CXX_COMPILER_VERSION} VERSION_MAJOR VERSION_MINOR)
    if(DEFINED CONAN_SETTINGS_COMPILER_TOOLSET)
       conan_message(STATUS "Conan: Skipping compiler check: Declared 'compiler.toolset'")
       return()
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        # MSVC_VERSION is defined since 2.8.2 at least
        # https://cmake.org/cmake/help/v2.8.2/cmake.html#variable:MSVC_VERSION
        # https://cmake.org/cmake/help/v3.14/variable/MSVC_VERSION.html
        if(
            # 1920-1929 = VS 16.0 (v142 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "16" AND NOT((MSVC_VERSION GREATER 1919) AND (MSVC_VERSION LESS 1930))) OR
            # 1910-1919 = VS 15.0 (v141 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "15" AND NOT((MSVC_VERSION GREATER 1909) AND (MSVC_VERSION LESS 1920))) OR
            # 1900      = VS 14.0 (v140 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "14" AND NOT(MSVC_VERSION EQUAL 1900)) OR
            # 1800      = VS 12.0 (v120 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "12" AND NOT VERSION_MAJOR STREQUAL "18") OR
            # 1700      = VS 11.0 (v110 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "11" AND NOT VERSION_MAJOR STREQUAL "17") OR
            # 1600      = VS 10.0 (v100 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "10" AND NOT VERSION_MAJOR STREQUAL "16") OR
            # 1500      = VS  9.0 (v90 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "9" AND NOT VERSION_MAJOR STREQUAL "15") OR
            # 1400      = VS  8.0 (v80 toolset)
            (CONAN_COMPILER_VERSION STREQUAL "8" AND NOT VERSION_MAJOR STREQUAL "14") OR
            # 1310      = VS  7.1, 1300      = VS  7.0
            (CONAN_COMPILER_VERSION STREQUAL "7" AND NOT VERSION_MAJOR STREQUAL "13") OR
            # 1200      = VS  6.0
            (CONAN_COMPILER_VERSION STREQUAL "6" AND NOT VERSION_MAJOR STREQUAL "12") )
            conan_error_compiler_version()
        endif()
    elseif(CONAN_COMPILER STREQUAL "gcc")
        conan_split_version(${CONAN_COMPILER_VERSION} CONAN_COMPILER_MAJOR CONAN_COMPILER_MINOR)
        set(_CHECK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
        set(_CONAN_VERSION ${CONAN_COMPILER_MAJOR}.${CONAN_COMPILER_MINOR})
        if(NOT ${CONAN_COMPILER_VERSION} VERSION_LESS 5.0)
            conan_message(STATUS "Conan: Compiler GCC>=5, checking major version ${CONAN_COMPILER_VERSION}")
            conan_split_version(${CONAN_COMPILER_VERSION} CONAN_COMPILER_MAJOR CONAN_COMPILER_MINOR)
            if("${CONAN_COMPILER_MINOR}" STREQUAL "")
                set(_CHECK_VERSION ${VERSION_MAJOR})
                set(_CONAN_VERSION ${CONAN_COMPILER_MAJOR})
            endif()
        endif()
        conan_message(STATUS "Conan: Checking correct version: ${_CHECK_VERSION}")
        if(NOT ${_CHECK_VERSION} VERSION_EQUAL ${_CONAN_VERSION})
            conan_error_compiler_version()
        endif()
    elseif(CONAN_COMPILER STREQUAL "clang")
        conan_split_version(${CONAN_COMPILER_VERSION} CONAN_COMPILER_MAJOR CONAN_COMPILER_MINOR)
        set(_CHECK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
        set(_CONAN_VERSION ${CONAN_COMPILER_MAJOR}.${CONAN_COMPILER_MINOR})
        if(NOT ${CONAN_COMPILER_VERSION} VERSION_LESS 8.0)
            conan_message(STATUS "Conan: Compiler Clang>=8, checking major version ${CONAN_COMPILER_VERSION}")
            if("${CONAN_COMPILER_MINOR}" STREQUAL "")
                set(_CHECK_VERSION ${VERSION_MAJOR})
                set(_CONAN_VERSION ${CONAN_COMPILER_MAJOR})
            endif()
        endif()
        conan_message(STATUS "Conan: Checking correct version: ${_CHECK_VERSION}")
        if(NOT ${_CHECK_VERSION} VERSION_EQUAL ${_CONAN_VERSION})
            conan_error_compiler_version()
        endif()
    elseif(CONAN_COMPILER STREQUAL "apple-clang" OR CONAN_COMPILER STREQUAL "sun-cc" OR CONAN_COMPILER STREQUAL "mcst-lcc")
        conan_split_version(${CONAN_COMPILER_VERSION} CONAN_COMPILER_MAJOR CONAN_COMPILER_MINOR)
        if(NOT ${VERSION_MAJOR}.${VERSION_MINOR} VERSION_EQUAL ${CONAN_COMPILER_MAJOR}.${CONAN_COMPILER_MINOR})
           conan_error_compiler_version()
        endif()
    elseif(CONAN_COMPILER STREQUAL "intel")
        conan_split_version(${CONAN_COMPILER_VERSION} CONAN_COMPILER_MAJOR CONAN_COMPILER_MINOR)
        if(NOT ${CONAN_COMPILER_VERSION} VERSION_LESS 19.1)
            if(NOT ${VERSION_MAJOR}.${VERSION_MINOR} VERSION_EQUAL ${CONAN_COMPILER_MAJOR}.${CONAN_COMPILER_MINOR})
               conan_error_compiler_version()
            endif()
        else()
            if(NOT ${VERSION_MAJOR} VERSION_EQUAL ${CONAN_COMPILER_MAJOR})
               conan_error_compiler_version()
            endif()
        endif()
    else()
        conan_message(STATUS "WARN: Unknown compiler '${CONAN_COMPILER}', skipping the version check...")
    endif()
endfunction()


function(conan_check_compiler)
    if(CONAN_DISABLE_CHECK_COMPILER)
        conan_message(STATUS "WARN: Disabled conan compiler checks")
        return()
    endif()
    if(NOT DEFINED CMAKE_CXX_COMPILER_ID)
        if(DEFINED CMAKE_C_COMPILER_ID)
            conan_message(STATUS "This project seems to be plain C, using '${CMAKE_C_COMPILER_ID}' compiler")
            set(CMAKE_CXX_COMPILER_ID ${CMAKE_C_COMPILER_ID})
            set(CMAKE_CXX_COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION})
        else()
            message(FATAL_ERROR "This project seems to be plain C, but no compiler defined")
        endif()
    endif()
    if(NOT CMAKE_CXX_COMPILER_ID AND NOT CMAKE_C_COMPILER_ID)
        # This use case happens when compiler is not identified by CMake, but the compilers are there and work
        conan_message(STATUS "*** WARN: CMake was not able to identify a C or C++ compiler ***")
        conan_message(STATUS "*** WARN: Disabling compiler checks. Please make sure your settings match your environment ***")
        return()
    endif()
    if(NOT DEFINED CONAN_COMPILER)
        conan_get_compiler(CONAN_COMPILER CONAN_COMPILER_VERSION)
        if(NOT DEFINED CONAN_COMPILER)
            conan_message(STATUS "WARN: CONAN_COMPILER variable not set, please make sure yourself that "
                          "your compiler and version matches your declared settings")
            return()
        endif()
    endif()

    if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL ${CMAKE_SYSTEM_NAME})
        set(CROSS_BUILDING 1)
    endif()

    # If using VS, verify toolset
    if (CONAN_COMPILER STREQUAL "Visual Studio")
        if (CONAN_SETTINGS_COMPILER_TOOLSET MATCHES "LLVM" OR
            CONAN_SETTINGS_COMPILER_TOOLSET MATCHES "llvm" OR
            CONAN_SETTINGS_COMPILER_TOOLSET MATCHES "clang" OR
            CONAN_SETTINGS_COMPILER_TOOLSET MATCHES "Clang")
            set(EXPECTED_CMAKE_CXX_COMPILER_ID "Clang")
        elseif (CONAN_SETTINGS_COMPILER_TOOLSET MATCHES "Intel")
            set(EXPECTED_CMAKE_CXX_COMPILER_ID "Intel")
        else()
            set(EXPECTED_CMAKE_CXX_COMPILER_ID "MSVC")
        endif()

        if (NOT CMAKE_CXX_COMPILER_ID MATCHES ${EXPECTED_CMAKE_CXX_COMPILER_ID})
            message(FATAL_ERROR "Incorrect '${CONAN_COMPILER}'. Toolset specifies compiler as '${EXPECTED_CMAKE_CXX_COMPILER_ID}' "
                                "but CMake detected '${CMAKE_CXX_COMPILER_ID}'")
        endif()

    # Avoid checks when cross compiling, apple-clang crashes because its APPLE but not apple-clang
    # Actually CMake is detecting "clang" when you are using apple-clang, only if CMP0025 is set to NEW will detect apple-clang
    elseif((CONAN_COMPILER STREQUAL "gcc" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR
        (CONAN_COMPILER STREQUAL "apple-clang" AND NOT CROSS_BUILDING AND (NOT APPLE OR NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")) OR
        (CONAN_COMPILER STREQUAL "clang" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR
        (CONAN_COMPILER STREQUAL "sun-cc" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "SunPro") )
        message(FATAL_ERROR "Incorrect '${CONAN_COMPILER}', is not the one detected by CMake: '${CMAKE_CXX_COMPILER_ID}'")
    endif()


    if(NOT DEFINED CONAN_COMPILER_VERSION)
        conan_message(STATUS "WARN: CONAN_COMPILER_VERSION variable not set, please make sure yourself "
                             "that your compiler version matches your declared settings")
        return()
    endif()
    check_compiler_version()
endfunction()


macro(conan_set_flags build_type)
    set(CMAKE_CXX_FLAGS${build_type} "${CMAKE_CXX_FLAGS${build_type}} ${CONAN_CXX_FLAGS${build_type}}")
    set(CMAKE_C_FLAGS${build_type} "${CMAKE_C_FLAGS${build_type}} ${CONAN_C_FLAGS${build_type}}")
    set(CMAKE_SHARED_LINKER_FLAGS${build_type} "${CMAKE_SHARED_LINKER_FLAGS${build_type}} ${CONAN_SHARED_LINKER_FLAGS${build_type}}")
    set(CMAKE_EXE_LINKER_FLAGS${build_type} "${CMAKE_EXE_LINKER_FLAGS${build_type}} ${CONAN_EXE_LINKER_FLAGS${build_type}}")
endmacro()


macro(conan_global_flags)
    if(CONAN_SYSTEM_INCLUDES)
        include_directories(SYSTEM ${CONAN_INCLUDE_DIRS}
                                   "$<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                                   "$<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_RELWITHDEBINFO}>"
                                   "$<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_MINSIZEREL}>"
                                   "$<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_DEBUG}>")
    else()
        include_directories(${CONAN_INCLUDE_DIRS}
                            "$<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                            "$<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_RELWITHDEBINFO}>"
                            "$<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_MINSIZEREL}>"
                            "$<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_DEBUG}>")
    endif()

    link_directories(${CONAN_LIB_DIRS})

    conan_find_libraries_abs_path("${CONAN_LIBS_DEBUG}" "${CONAN_LIB_DIRS_DEBUG}"
                                  CONAN_LIBS_DEBUG)
    conan_find_libraries_abs_path("${CONAN_LIBS_RELEASE}" "${CONAN_LIB_DIRS_RELEASE}"
                                  CONAN_LIBS_RELEASE)
    conan_find_libraries_abs_path("${CONAN_LIBS_RELWITHDEBINFO}" "${CONAN_LIB_DIRS_RELWITHDEBINFO}"
                                  CONAN_LIBS_RELWITHDEBINFO)
    conan_find_libraries_abs_path("${CONAN_LIBS_MINSIZEREL}" "${CONAN_LIB_DIRS_MINSIZEREL}"
                                  CONAN_LIBS_MINSIZEREL)

    add_compile_options(${CONAN_DEFINES}
                        "$<$<CONFIG:Debug>:${CONAN_DEFINES_DEBUG}>"
                        "$<$<CONFIG:Release>:${CONAN_DEFINES_RELEASE}>"
                        "$<$<CONFIG:RelWithDebInfo>:${CONAN_DEFINES_RELWITHDEBINFO}>"
                        "$<$<CONFIG:MinSizeRel>:${CONAN_DEFINES_MINSIZEREL}>")

    conan_set_flags("")
    conan_set_flags("_RELEASE")
    conan_set_flags("_DEBUG")

endmacro()


macro(conan_target_link_libraries target)
    if(CONAN_TARGETS)
        target_link_libraries(${target} ${CONAN_TARGETS})
    else()
        target_link_libraries(${target} ${CONAN_LIBS})
        foreach(_LIB ${CONAN_LIBS_RELEASE})
            target_link_libraries(${target} optimized ${_LIB})
        endforeach()
        foreach(_LIB ${CONAN_LIBS_DEBUG})
            target_link_libraries(${target} debug ${_LIB})
        endforeach()
    endif()
endmacro()


macro(conan_include_build_modules)
    if(CMAKE_BUILD_TYPE)
        if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(CONAN_BUILD_MODULES_PATHS ${CONAN_BUILD_MODULES_PATHS_DEBUG} ${CONAN_BUILD_MODULES_PATHS})
        elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
            set(CONAN_BUILD_MODULES_PATHS ${CONAN_BUILD_MODULES_PATHS_RELEASE} ${CONAN_BUILD_MODULES_PATHS})
        elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
            set(CONAN_BUILD_MODULES_PATHS ${CONAN_BUILD_MODULES_PATHS_RELWITHDEBINFO} ${CONAN_BUILD_MODULES_PATHS})
        elseif(${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel")
            set(CONAN_BUILD_MODULES_PATHS ${CONAN_BUILD_MODULES_PATHS_MINSIZEREL} ${CONAN_BUILD_MODULES_PATHS})
        endif()
    endif()

    foreach(_BUILD_MODULE_PATH ${CONAN_BUILD_MODULES_PATHS})
        include(${_BUILD_MODULE_PATH})
    endforeach()
endmacro()


### Definition of user declared vars (user_info) ###

set(CONAN_USER_AUTOMAKE_compile "C:\\Users\\Daniil_Akhramiuk\\.conan\\data\\automake\\1.16.3\\_\\_\\package\\3e48e69237f7f2196164383ef9dedf0f93cbf249\\bin\\share\\automake-1.16\\compile")
set(CONAN_USER_AUTOMAKE_ar_lib "C:\\Users\\Daniil_Akhramiuk\\.conan\\data\\automake\\1.16.3\\_\\_\\package\\3e48e69237f7f2196164383ef9dedf0f93cbf249\\bin\\share\\automake-1.16\\ar-lib")