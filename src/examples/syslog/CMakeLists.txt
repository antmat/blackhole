CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_BUILD_TYPE "Debug")

SET(PROJECT blackhole-example-syslog)

SET(HEADERS
)

SET(SOURCES
    main
)

SET(Boost_USE_MULTITHREADED ON)

SET(BOOST_COMPONENTS
    system
    thread
    filesystem
)

PROJECT(${PROJECT})

INCLUDE_DIRECTORIES(
    ../..
)

FIND_PACKAGE(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

ADD_EXECUTABLE(${PROJECT} ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

ADD_DEFINITIONS(-std=c++0x)
