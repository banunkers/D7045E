#--------------------------------------------------------------------------
# lab4
#--------------------------------------------------------------------------

FILE(GLOB lab4_headers src/include/*.h)
FILE(GLOB lab4_sources src/*.cpp)
INCLUDE_DIRECTORIES(src/include)

SET(files_lab3 ${lab3_headers} ${lab3_sources})
SOURCE_GROUP("lab4" FILES ${files_lab3})

ADD_EXECUTABLE(lab4 ${files_lab3})
TARGET_LINK_LIBRARIES(lab4 core render)
ADD_DEPENDENCIES(lab4 core render)