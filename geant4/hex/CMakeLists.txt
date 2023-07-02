cmake_minimum_required(VERSION 3.16)
project(hex)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(hex hex.cc ${sources} ${headers})
target_link_libraries(hex ${Geant4_LIBRARIES})

set(HEX_SCRIPTS
init.mac
run.mac
plotHisto.C
plotCell.C
plotPerCellEdep.C
plots.C
cluster.C
checkev.C
)

foreach(_script ${HEX_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()