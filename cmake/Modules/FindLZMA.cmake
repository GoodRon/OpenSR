# - Try to find LZMA
# Once done this will define
#
#  LZMA_FOUND - system has LZMA
#  LZMA_INCLUDE_DIRS - the LZMA include directories
#  LZMA_LIBRARIES - link these to use LZMA
#

INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(LZMA_INCLUDE_DIR NAMES lzma.h)
FIND_LIBRARY(LZMA_LIBRARY NAMES lzma)

SET(LZMA_LIBRARIES ${LZMA_LIBRARY})
SET(LZMA_INCLUDE_DIRS ${LZMA_INCLUDE_DIR})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LZMA DEFAULT_MSG LZMA_LIBRARY LZMA_INCLUDE_DIR)

MARK_AS_ADVANCED(LZMA_INCLUDE_DIR LZMA_LIBRARY)