
cmake_minimum_required(VERSION 3.20)

project(SwapConsumable VERSION 1.0.0 LANGUAGES CXX)

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(SOURCES
    src/settings.cpp
    src/settings.h
    src/events.cpp
    src/events.h
	src/main.cpp)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
