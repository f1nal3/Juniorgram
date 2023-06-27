set(TARGET DataAccess.Repository.Kernel)
set(OUTPUT_NAME DataAccess.Repository.Kernel)

set(SOURCE_FILES
    Repositories.hpp
    KernelRepositoryContainer.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET}
    PUBLIC DataAccess.Repository.Public DataAccess.Postgre)
