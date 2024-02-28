add_library(OJRun 
    OJServer.h OJServer.cpp
    OJFileCheck.h OJFileCheck.cpp
    OJProcessPool.h OJProcessPool.cpp
    OJProcessPoolPrivate.h OJProcessPoolPrivate.cpp)

target_link_libraries(OJRun pthread OJSql OJUtils)    
target_include_directories(OJRun PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})