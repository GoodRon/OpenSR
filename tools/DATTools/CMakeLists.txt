FIND_PACKAGE(JsonCpp REQUIRED)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${Boost_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
)

SET(DAT_CONVERTER_SRCS
    crypt.cpp
    convert.cpp
)

SET(DAT_JSON_SRCS
    json.cpp
)

ADD_EXECUTABLE(opensr-dat-convert ${DAT_CONVERTER_SRCS})
TARGET_LINK_LIBRARIES(opensr-dat-convert ranger)

ADD_EXECUTABLE(opensr-dat-json ${DAT_JSON_SRCS})
TARGET_LINK_LIBRARIES(opensr-dat-json ranger ${JSONCPP_LIBRARIES})

INSTALL(TARGETS opensr-dat-convert RUNTIME DESTINATION bin)
INSTALL(TARGETS opensr-dat-json RUNTIME DESTINATION bin)