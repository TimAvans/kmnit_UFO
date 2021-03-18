# Author: Jeroen de Haas <jah.dehaas@avans.nl>
cmake_minimum_required(VERSION 3.1.3)
project(pigisland)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wpedantic)#  -Werror)
    add_compile_options("$<$<CONFIG:DEBUG>:-g>")
endif()

  
add_library(ufo-common STATIC
  src/kmint/ufo/resources.cpp
  src/kmint/ufo/tank.cpp
  src/kmint/ufo/andre.cpp
  src/kmint/ufo/saucer.cpp
  src/kmint/ufo/human.cpp
  src/kmint/ufo/node_algorithm.cpp
  src/kmint/ufo/play.cpp
  
  src/kmint/ufo/AI/StateMachine/state_user.cpp
  src/kmint/ufo/AI/StateMachine/wander_state.cpp
  src/kmint/ufo/AI/StateMachine/flee_state.cpp
  src/kmint/ufo/AI/StateMachine/target_state.cpp
  src/kmint/ufo/AI/StateMachine/repair_state.cpp
  src/kmint/ufo/AI/StateMachine/global_tank_state.cpp
  src/kmint/ufo/emp.cpp
  src/kmint/ufo/AI/astar.cpp
  src/kmint/ufo/shield.cpp
  src/kmint/ufo/AI/ForceDriven/steering_behaviour.cpp
  src/kmint/ufo/AI/ForceDriven/moving_entity.cpp
  src/kmint/ufo/AI/ForceDriven/wall.cpp
  src/kmint/ufo/AI/ForceDriven/c2d_matrix.cpp
  )

target_compile_features(ufo-common PUBLIC cxx_std_17)
target_link_libraries(ufo-common PRIVATE kmint)
target_include_directories(ufo-common PRIVATE 
  src
  PUBLIC
  include)

add_executable(ufo
  src/main.cpp)
target_link_libraries(ufo PRIVATE ufo-common)

add_custom_target(ufo-resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
