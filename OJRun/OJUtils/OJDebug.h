#ifndef _OJ_CORE_UTILS_DEBUG_H
#define _OJ_CORE_UTILS_DEBUG_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

inline void print() {
	std::cout << std::endl;
}

/**
 * 
 * 
 * @version 1.0.0
 * 
 * @brief 递归打印函数，用于在一行打印输出多个不同类型的参数
 * 该函数接收任意数量的参数并在一行输出他们，使用空格分割。
 * 注意，如果想要在自定义类中使用，需要重载operator<<()函数。
 * 如:
 * 	struct test {
 * 		int id;
 * 		test(int i) : id(i) { }
 * 		friend std::ostream& operator<<(std::ostream& os, const test& t) {
 * 			os << t;
 * 			return os;
 * 		}
 * 	}
 * 使用方式如下:
 * 	oj::print(1, 1.2, test(1));
 * 
 * 
*/
template<typename T, typename ...Types>
void print(const T& arg, const Types& ...args) {
	std::cout << arg <<  " ";
	print(args...);
}

/**
 * @brief 重载了std::vector
*/
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	os << "{";
	if (!vec.empty()) {
		os << " " << vec[0] << " ";
		for (int i = 1;i < vec.size();i ++ )
			os << vec[i] <<  " ";
	}
	os <<  "}";
	return os;
}

/**
 * @brief 该类有两种形式的输出，
*/
class OJDebug 
{
public:
	OJDebug();
	OJDebug(const std::string& format);

	void putFormat(const std::string& format);

	template<typename T>
	OJDebug& args(const T& arg) {
		std::stringstream stream;
		stream << arg; 
		std::string str = stream.str();
		placeholders.push_back(str);
		return *this;
	}

	void print();

	template<typename ...Args>
	static void print(const std::string& format, const Args&... args);
private:
	template<typename T, typename ...Args>
	static void print_helper(int index, const T& arg, const Args&... args);

	static void print_helper(int index) { std::cout << std::endl; } 

	static int index;
	static std::vector<std::string> formats;
private:
	std::string format;
	std::vector<std::string> placeholders;
};

template<typename ...Args>
void OJDebug::print(const std::string& format, const Args&... args) {
	OJDebug::formats.push_back(format);
	print_helper(OJDebug::index, args...);
	OJDebug::index++;
}

template<typename T, typename ...Args>
void OJDebug::print_helper(int index, const T& arg, const Args&... args) {
	std::stringstream stream;
	stream << arg;
	
}

class OJLogger 
{
public:
	enum LogLevel {
		ERROR, WARNING, INFO,
	};
	OJLogger() = default;
	OJLogger(const std::string& logger_path);

	void print(const std::string& logger_context, LogLevel level = INFO);

	
private:
	std::string error;
	std::ofstream infile;
};

OJ_NAMESPACE_END

#endif
