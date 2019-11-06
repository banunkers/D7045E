#--------------------------------------------------------------------------
# lab1
#--------------------------------------------------------------------------

FILE(GLOB lab1_headers code/*.h)
FILE(GLOB lab1_sources code/*.cc)

SET(files_example ${lab1_headers} ${lab1_sources})
SOURCE_GROUP("lab1" FILES ${files_example})

ADD_EXECUTABLE(lab1 ${files_example})
TARGET_LINK_LIBRARIES(lab1 core render)
ADD_DEPENDENCIES(lab1 core render)