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
