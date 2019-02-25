cmake_minimum_required(VERSION 3.10)

project(panel-design-manager)
add_subdirectory(src)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/n4d/client/PanelDesignManagerClient.py
  DESTINATION share/n4d/python-plugins/
)
install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/n4d/client/PanelDesignManagerClient
  DESTINATION etc/n4d/conf.d/
)
install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/n4d/server/PanelDesignManagerServer.py
  DESTINATION share/n4d/python-plugins/
)
install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/n4d/server/PanelDesignManagerServer
  DESTINATION etc/n4d/conf.d/
)