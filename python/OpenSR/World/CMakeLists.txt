SET(PYTHON_WORLD_SOURCES
    Artifact.cpp
    CargoHook.cpp
    DefenceGenerator.cpp
    DesertPlanet.cpp
    Droid.cpp
    Engine.cpp
    Equipment.cpp
    Goods.cpp
    HabitablePlanet.cpp
    Hull.cpp
    InteractionContext.cpp
    InteractionWidget.cpp
    Item.cpp
    LandContext.cpp
    Asteroid.cpp
    Micromodulus.cpp
    Planet.cpp
    PlanetManager.cpp
    QuestPlayer.cpp
    Radar.cpp
    Scanner.cpp
    Ship.cpp
    ShipContext.cpp
    PlanetarySystem.cpp
    SpaceBase.cpp
    InfoWidget.cpp
    SpaceObject.cpp
    SystemObject.cpp
    SystemPlanetWidget.cpp
    SystemWidget.cpp
    Tank.cpp
    Types.cpp
    Weapon.cpp
    World.cpp
    WorldGenHook.cpp
    WorldManager.cpp
    WorldObject.cpp
    SkinManager.cpp
    RaceManager.cpp
    SystemManager.cpp
    StyleManager.cpp
    SpaceObjectWidget.cpp
)

SET(PYTHON_WORLD_HEADERS 
    ${CMAKE_SOURCE_DIR}/include/OpenSR/World/python/WorldObjectWrap.h
)

SET(PYTHON_MODULE_WORLD_SOURCES
    __init__.py
)

INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${LIBINTL_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_SOURCE_DIR}/libRanger
)

ADD_LIBRARY(_World SHARED ${PYTHON_WORLD_SOURCES})
TARGET_LINK_LIBRARIES(_World world ${Boost_PYTHON3_LIBRARY} ${PYTHON_LIBRARIES})

IF(WIN32)
    SET_TARGET_PROPERTIES(_World PROPERTIES PREFIX "" SUFFIX ".pyd")
ELSE(WIN32)
    SET_TARGET_PROPERTIES(_World PROPERTIES PREFIX "")
ENDIF(WIN32)

INSTALL(FILES ${PYTHON_MODULE_WORLD_SOURCES} DESTINATION ${PYTHON_PACKAGE_PATH}/OpenSR/World)
INSTALL(TARGETS _World LIBRARY DESTINATION ${PYTHON_PACKAGE_PATH}/OpenSR/World RUNTIME DESTINATION ${PYTHON_PACKAGE_PATH}/OpenSR/World)

