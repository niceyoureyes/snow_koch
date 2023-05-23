cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_INSTALL_PREFIX "../Snow Koch")

project(snow_koch VERSION 0.1.0)

set(SOURCES main.cpp)

add_executable(snow_koch ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(${PROJECT_NAME} OpenGL::GL GLUT::GLUT)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# set install rules (windows only)
if(WIN32)
    if(MSVC)
        install(TARGETS snow_koch DESTINATION bin COMPONENT binaries)
    endif(MSVC)
endif(WIN32)
