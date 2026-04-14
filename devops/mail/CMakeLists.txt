cmake_minimum_required(VERSION 3.22)

project(total_mail)

find_package(mailio REQUIRED)

file(GLOB_RECURSE smtp_source_files ${CMAKE_HOME_DIRECTORY}/src/smtp/*.cpp)
add_executable(smtp_server ${smtp_source_files})
target_link_libraries(smtp_server PRIVATE mailio::mailio)

# file(GLOB_RECURSE imap_source_files ${CMAKE_HOME_DIRECTORY}/src/imap/*.cpp)
# add_executable(imap_server ${imap_source_files})

# file(GLOB_RECURSE imap_source_files ${CMAKE_HOME_DIRECTORY}/src/db/*.cpp)
