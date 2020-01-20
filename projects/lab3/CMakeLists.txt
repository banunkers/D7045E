#--------------------------------------------------------------------------
# lab3
#--------------------------------------------------------------------------

FILE(GLOB lab3_headers src/*.h)
FILE(GLOB lab3_sources src/*.cc)

SET(files_lab3 ${lab3_headers} ${lab3_sources})
SOURCE_GROUP("lab3" FILES ${files_lab3})

ADD_EXECUTABLE(lab2 ${files_lab3})
TARGET_LINK_LIBRARIES(lab3 core render)
ADD_DEPENDENCIES(lab3 core render)