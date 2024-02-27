add_library(OJRun 
    OJServer.h OJServer.cpp
    OJFileCheck.h OJFileCheck.cpp
    OJProcessPool.h OJProcessPool.cpp)

target_link_libraries(OJRun pthread OJSql)    
target_include_directories(OJRun PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})