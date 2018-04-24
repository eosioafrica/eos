cmake_minimum_required(VERSION 3.8)
project(ClusterApi)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include( InstallDirectoryPermissions )


file(GLOB HEADERS "include/clusterapi/*.hpp")


set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS thread )

find_package(Boost 1.66 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
set( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )


add_library(clusterapi
            clusterapi.cpp
            ${HEADERS} )


target_link_libraries( clusterapi ${Boost_LIBRARIES} rt)

target_include_directories( clusterapi
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIR})



set_target_properties( clusterapi PROPERTIES PUBLIC_HEADER "${HEADERS}" )

set(CPACK_PACKAGING_INSTALL_PREFIX /)

install( TARGETS
   clusterapi

   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/clusterapi
)

install_directory_permissions( DIRECTORY ${CMAKE_INSTALL_FULL_INCLUDEDIR}/clusterapi )
