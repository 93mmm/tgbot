cmake_minimum_required(VERSION 3.10.2)
project(todo_bot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
find_package(SQLite3)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} 
                                       ${Boost_INCLUDE_DIR}
                                       ${SQLite3_INCLUDE_DIRS}
)

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_executable(todo_bot source/main.cxx
                        source/lib/tools/tools.h
                        source/lib/tools/tools.cxx
                        source/lib/bot/bot.cxx
                        source/lib/bot/bot.h
                        source/lib/messages/messages.cxx
                        source/lib/messages/messages.h
)

target_link_libraries(todo_bot /usr/local/lib/libTgBot.a 
                               ${CMAKE_THREAD_LIBS_INIT} 
                               ${OPENSSL_LIBRARIES} 
                               ${Boost_LIBRARIES} 
                               ${CURL_LIBRARIES} 
                               ${SQLite3_LIBRARIES}
                               ${nlohmann_json_LIBRARIES}
)
