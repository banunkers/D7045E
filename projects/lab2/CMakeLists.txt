#--------------------------------------------------------------------------
# lab2
#--------------------------------------------------------------------------

FILE(GLOB lab2_headers src/*.h)
FILE(GLOB lab2_sources src/*.cc)

SET(files_lab2 ${lab2_headers} ${lab2_sources})
SOURCE_GROUP("lab2" FILES ${files_lab2})

ADD_EXECUTABLE(lab2 ${files_lab2})
TARGET_LINK_LIBRARIES(lab2 core render)
ADD_DEPENDENCIES(lab2 core render)