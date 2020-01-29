#--------------------------------------------------------------------------
# lab4
#--------------------------------------------------------------------------

FILE(GLOB lab4_headers src/include/*.h)
FILE(GLOB lab4_sources src/*.cpp)
INCLUDE_DIRECTORIES(src/include)

SET(files_lab4 ${lab4_headers} ${lab4_sources})
SOURCE_GROUP("lab4" FILES ${files_lab4})

ADD_EXECUTABLE(lab4 ${files_lab4})
TARGET_LINK_LIBRARIES(lab4 core render)
ADD_DEPENDENCIES(lab4 core render)