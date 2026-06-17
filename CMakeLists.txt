cmake_minimum_required(VERSION 3.5)

project(LearningThreads)
set(CMAKE_CXX_STANDARD 17)

# thread1
add_executable(Thread1 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/thread1.c"
)

# producer/consumer problem
add_executable(ProducerConsumer
    "${CMAKE_CURRENT_SOURCE_DIR}/src/producer_consumer.c"
)

# four_shaders
add_executable(FourShaders 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/four_shaders.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/four_shaders/render_setup.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/four_shaders/render_data.c"
)

add_subdirectory(external/glfw)
add_library(glad external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

target_link_directories(FourShaders PRIVATE
    external/glfw/src/
)
target_include_directories(FourShaders PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include/"
)
target_link_libraries(FourShaders glfw glad opengl32 dwmapi)

target_compile_definitions(FourShaders PRIVATE
        $<$<CONFIG:Debug>:BUILD_PROFILE_DEBUG>
        $<$<CONFIG:Release>:BUILD_PROFILE_RELEASE>
)

include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

#dead code
# add all project source files
#file(GLOB_RECURSE PROJECT_SOURCES_C CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
#file(GLOB_RECURSE PROJECT_SOURCES_CPP CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#target_compile_definitions(Thread1 PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
#target_compile_definitions(Thread1 PUBLIC RESOURCE_PATH="./resources/")

# copy resources if they are different to build directory
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#        "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
#        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
#        COMMENT "Copying resources..."
#)

# target_link_libraries(${PROJECT_NAME} glfw opengl32 dwmapi glad)
