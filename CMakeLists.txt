cmake_minimum_required(VERSION 2.6)
project(scheduler)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package (Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(CURLpp REQUIRED)

set( BOOST_COMPONENTS_NEEDED system)
set(BOOST_MIN_VERSION "1.46.0")
find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${BOOST_COMPONENTS_NEEDED} REQUIRED)

add_executable(scheduler main.cpp Scheduler.cpp)

target_include_directories(scheduler 
                           PRIVATE 
                            ${CURL_INCLUDE_DIRECTORIES} 
                            ${CURLPP_INCLUDE_DIRECTORIES})
                           
target_link_libraries (scheduler ${CMAKE_THREAD_LIBS_INIT} ${CURL_LIBRARIES} ${CURLPP_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS scheduler RUNTIME DESTINATION bin)
