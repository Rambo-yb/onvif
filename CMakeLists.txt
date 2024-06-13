cmake_minimum_required(VERSION 2.8)
project(onvif)

add_definitions(-Wall -DWITH_NONAMESPACES -Os -DWITH_DOM -DWITH_OPENSSL)

set(GSOAP_SHARE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/custom/duration.c
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/custom/struct_timeval.c
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/plugin/wsaapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/plugin/wsseapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/plugin/smdevp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/plugin/mecevp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/plugin/threads.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/plugin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/custom)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/extras)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/share/gsoap/import)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/example MAIN_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/gsoap SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gsoap)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/include)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/gsoap/lib)

include_directories(/home/smb/third_src/openssl-1.1.1h/_install/include/openssl)
link_directories(/home/smb/third_src/openssl-1.1.1h/_install/lib)

add_library(onvif ${GSOAP_SHARE_SRC} ${SRC})
#add_library(network SHARED ${SRC})

add_executable(onvif_test ${MAIN_SRC})
target_link_libraries(onvif_test onvif pthread ssl crypto dl)