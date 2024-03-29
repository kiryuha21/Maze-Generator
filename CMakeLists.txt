cmake_minimum_required(VERSION 3.26)
project(3d_viewer_template)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(app
        src/main.cc
        src/view/MainWindow.cc
        src/view/MainWindow.h
        src/controller/GenerationController.cc
        src/controller/GenerationController.h
        src/model/GenerationModel.cc
        src/model/GenerationModel.h
        src/model/RouteModel.h
        src/model/RouteModel.cc)

add_executable(test
        src/tests/tests_main.cc
        src/tests/generation_tests.cc
        src/model/GenerationModel.h
        src/model/GenerationModel.cc
        src/model/RouteModel.h
        src/model/RouteModel.cc
        src/tests/route_search_tests.cc)

target_link_libraries(app ${GTKMM_LIBRARIES})
target_link_libraries(test gtest)
