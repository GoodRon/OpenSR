# Try to find liblzma
# Once done this will define 
# 
# LZMA_FOUND - system has liblzma
# LZMA_INCLUDE_DIR - the liblzma include directory 
# LZMA_LIBRARIES - link these to use lzma
# 

IF(LZMA_LIBRARIES AND LZMA_INCLUDE_DIR)
  SET(LZMA_FIND_QUIETLY TRUE)
ENDIF(LZMA_LIBRARIES AND LZMA_INCLUDE_DIR)

FIND_PATH(LZMA_INCLUDE_DIR lzma.h
  PATHS
  /usr/include
  /usr/local/include
)
FIND_PATH(LZMA_INCLUDE_DIR NAMES lzma.h)

FIND_LIBRARY(LZMA_LIBRARY NAMES lzma
  PATHS
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib64
  /usr/lib
  /usr/local/lib64
  /usr/local/lib
)
FIND_LIBRARY(LZMA_LIBRARIES NAMES lzma)

IF(LZMA_INCLUDE_DIR AND LZMA_LIBRARIES)
   SET(LZMA_FOUND TRUE)
ENDIF(LZMA_INCLUDE_DIR AND LZMA_LIBRARIES)

IF(LZMA_FOUND)
  IF(NOT LZMA_FIND_QUIETLY)
    MESSAGE(STATUS "Found liblzma: ${LZMA_LIBRARIES}")
  ENDIF(NOT LZMA_FIND_QUIETLY)
ELSE(LZMA_FOUND)
  IF(LZMA_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find JsonCpp")
  ENDIF(LZMA_FIND_REQUIRED)
ENDIF(LZMA_FOUND)

MARK_AS_ADVANCED(LZMA_INCLUDE_DIR LZMA_LIBRARY)
