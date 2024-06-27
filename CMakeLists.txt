project(record_events)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DCURRENT_BINARY_PATH=${CMAKE_CURRENT_BINARY_PATH})

find_package(MetavisionSDK COMPONENTS core driver REQUIRED)
list(APPEND EXTRA_LIBS MetavisionSDK::core MetavisionSDK::driver)

set(projectName record_events)

# RecordSnapShot
add_executable(${projectName} ${projectName}.cpp)
target_link_libraries(${projectName} PUBLIC ${EXTRA_LIBS})

# decode metavision raw file
add_executable(metavision_evt3_raw_file_decoder metavision_evt3_raw_file_decoder.cpp)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
install(TARGETS ${projectName} DESTINATION bin)
install(TARGETS metavision_evt3_raw_file_decoder DESTINATION bin)
