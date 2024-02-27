add_library(OJUtils OJDebug.h OJDebug.cpp OJVariant.h OJVariant.cpp)

target_include_directories(OJUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
