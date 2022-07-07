
file(GLOB_RECURSE HEADER_FILES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

file(GLOB_RECURSE SRC_FILES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        )
add_executable(raytracing_one_weekend
        ${HEADER_FILES}
        ${SRC_FILES}
        )