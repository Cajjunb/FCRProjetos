cmake_minimum_required(VERSION 2.8.3)
project(fcr2017)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  sensor_msgs
  p2os_msgs
  tf
)

catkin_package(
 INCLUDE_DIRS include
 CATKIN_DEPENDS roscpp std_msgs geometry_msgs sensor_msgs tf p2os_msgs
)

include_directories(
  include
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(gazebo_sonar src/gazebo_sonar.cpp)
target_link_libraries(gazebo_sonar ${catkin_LIBRARIES})

# add_executable(forward_kinematics_pioneer src/forward_kinematics_pioneer.cpp)
# target_link_libraries(forward_kinematics_pioneer ${catkin_LIBRARIES})

add_executable(obstacle_avoidance_node src/obstacle_avoidance_node.cpp src/obstacle_avoidance.cpp src/vetor.cpp)
target_link_libraries(obstacle_avoidance_node ${catkin_LIBRARIES})


add_executable(odometria_campo_node src/odometria_campo_node.cpp src/odometria_campo.cpp src/graph.cpp src/vetor.cpp)
target_link_libraries(odometria_campo_node ${catkin_LIBRARIES})

add_executable(mapeamento_node src/mapeamento_node.cpp src/mapeamento.cpp src/graphMap.cpp src/vetor.cpp src/mapa.cpp)
target_link_libraries(mapeamento_node ${catkin_LIBRARIES})

# add_executable(pwm_quad src/pwm_quad.cpp)
# target_link_libraries(pwm_quad ${catkin_LIBRARIES})
#add_executable(forward_kinematics_pioneer src/forward_kinematics_pioneer.cpp)
#target_link_libraries(forward_kinematics_pioneer ${catkin_LIBRARIES})

#add_executable(pwm_quad src/pwm_quad.cpp)
#target_link_libraries(pwm_quad ${catkin_LIBRARIES})
