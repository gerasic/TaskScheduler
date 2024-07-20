add_executable(${PROJECT_NAME} main.cpp)

include_directories(../lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})