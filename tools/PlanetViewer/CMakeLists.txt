FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore QtGui QtOpenGL)

INCLUDE(${QT_USE_FILE})

SET(PW_HEADERS 
    MainWindow.h
    PlanetViewer.h
    PlanetModel.h
)

SET(PW_SOURCES
    main.cpp
    MainWindow.cpp
    PlanetViewer.cpp
    PlanetModel.cpp
)

SET(PW_UI
    MainWindow.ui
)

INCLUDE_DIRECTORIES(
  ${JSONCPP_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/libRanger
  ${CMAKE_CURRENT_SOURCE_DIR}
)

IF(WIN32)
    INCLUDE_DIRECTORIES(
        ${GLEW_INCLUDE_DIR}
    )
ENDIF(WIN32)

ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)

QT4_WRAP_CPP(PW_MOC_SOURCES ${PW_HEADERS})
QT4_WRAP_UI(PW_UI_SOURCES ${PW_UI})
QT4_ADD_RESOURCES(PWRES_SOURCES resources.qrc)

ADD_EXECUTABLE(opensr-planetviewer  ${PWRES_SOURCES} ${PW_SOURCES} ${PW_UI_SOURCES} ${PW_MOC_SOURCES} ${PW_HEADERS})

IF(WIN32)
    SET_TARGET_PROPERTIES(opensr-planetviewer ranger PROPERTIES COMPILE_FLAGS "/Zc:wchar_t-")
    TARGET_LINK_LIBRARIES(opensr-planetviewer ranger ${GLEW_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES})
ELSE(WIN32)
    TARGET_LINK_LIBRARIES(opensr-planetviewer ranger ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES})
ENDIF(WIN32)

INSTALL(TARGETS opensr-planetviewer RUNTIME DESTINATION bin)