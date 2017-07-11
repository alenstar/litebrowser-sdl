cmake_minimum_required(VERSION 2.8)

include(FindPkgConfig)

project(litebrowser)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -O0 -DDEBUG -g")
set(CMAKE_C_FLAGS_DEBUG "-std=c99 -O0 -DDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3 -DNODEBUG")
set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3 -DNODEBUG")

set(LITEBROWSER_PATH src)
set(LITEHTML_PATH litehtml)
set(NANOVG_PATH nanovg)
#set(CONTAINER_PATH ${LITEHTML_PATH}/src)

add_definitions(-DNANOVG_GLEW)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
	add_definitions(-DDEBUG)
else()
	add_definitions(-DNODEBUG)
endif()

#pkg_check_modules(LB_LIBS REQUIRED fontconfig gdkmm-3.0 gtkmm-3.0 libcurl cairo)
pkg_check_modules(LB_LIBS REQUIRED sdl2 SDL2_image SDL2_ttf libcurl gl glu glew)

set(SOURCE  ${LITEBROWSER_PATH}/main.cpp
            ${LITEBROWSER_PATH}/container_sdl.cpp
            ${LITEBROWSER_PATH}/http_loader.cpp
            ${LITEBROWSER_PATH}/utils.cpp
			${NANOVG_PATH}/src/nanovg.c
    )

include_directories(${LITEHTML_PATH}/include ${LB_LIBS_INCLUDE_DIRS} ${NANOVG_PATH}/src /usr/include/GL)

add_subdirectory(${LITEHTML_PATH})

find_program(XXD_COMMAND xxd)
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/${LITEHTML_PATH}/include/master.css | xxd -i > ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc PROPERTIES GENERATED TRUE)

add_executable(litebrowser ${SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc)

target_link_libraries(litebrowser litehtml ${LB_LIBS_LIBRARIES} PocoFoundation m)

