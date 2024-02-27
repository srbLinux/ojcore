add_library(OJSql OJSql.h 
    OJSqlResult.h OJSqlResult.cpp      
    OJSqlDatabase.h OJSqlDatabase.cpp  
    OJSqlConnector.h OJSqlConnector.cpp
)

# 查找MySql的include目录和库文件
find_path(MYSQL_INCLUDE_DIR mysql/mysql.h)

if (MYSQL_INCLUDE_DIR)
    include_directories(${MYSQL_INCLUDE_DIR})
    target_link_libraries(OJSql sqlite3)
    target_link_libraries(OJSql mysqlclient)
    message(STATUS "MYSQL include directory found: ${MYSQL_INCLUDE_DIR}")
else ()
    message(FATAL_ERROR "MySQL not found. Please make sure MYSQL development")
endif ()

target_include_directories(OJSql PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
