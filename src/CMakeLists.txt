
find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

#find_package(XMLRPC REQUIRED c++2)
# XMLRPC find mechanism is broken in this version
# work arround for that
set(XMLRPC_LIBRARIES xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_xmlparse xmlrpc_xmltok xmlrpc_util xmlrpc_packetsocket)

add_executable(panel-design-manager main.cpp window.cpp loginwidget.cpp setupwidget.cpp n4dcontext.cpp ../rsrc/resources.qrc)
target_link_libraries(panel-design-manager Qt5::Widgets Qt5::Core ${XMLRPC_LIBRARIES})

install(TARGETS panel-design-manager RUNTIME DESTINATION bin)