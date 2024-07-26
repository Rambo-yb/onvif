cmake_minimum_required(VERSION 2.8)
project(onvif)

add_definitions(-Wall -DWITH_NONAMESPACES -Os -DWITH_DOM -DWITH_OPENSSL -DUSE_EASYLOGGER -DWITH_NO_C_LOCALE)

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gsoap)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/cjson SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/cjson)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/third_src/easylogger SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_src/easylogger)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/common SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/common)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/general_src/list SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/general_src/list)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/openssl/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/openssl/include/openssl)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/_install/openssl/lib)

add_library(onvif ${GSOAP_SHARE_SRC} ${SRC})
#add_library(network SHARED ${SRC})

add_executable(onvif_test ${MAIN_SRC})
target_link_libraries(onvif_test onvif pthread ssl crypto dl m rt)
install(TARGETS onvif_test DESTINATION bin)
install(TARGETS onvif DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)