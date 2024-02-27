add_library(OJJson OJJson.h OJJsonParse.h OJJsonArray.h OJJsonArray.cpp)

target_include_directories(OJJson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
