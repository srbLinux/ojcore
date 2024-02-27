# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(OJCore)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# 添加 OJSql 子目录
add_subdirectory(OJSql)

# 添加 OJRun 子目录
add_subdirectory(OJRun)

# 添加 OJJson 子目录
add_subdirectory(OJJson)

# 添加 OJUtils 子目录
add_subdirectory(OJUtils)

# 主项目源文件
add_executable(OJCore main.cpp)

# 将 OJSql、OJJson、OJUtils 的库链接到主项目
target_link_libraries(OJCore PRIVATE OJSql OJJson OJUtils OJRun)

