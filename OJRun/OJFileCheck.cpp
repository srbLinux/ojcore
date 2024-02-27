#include "./OJFileCheck.h"

#include <regex>
#include <string>
#include <vector>

OJ_NAMESPACE_BEGIN

/**
 * @brief   对于C、C++语言来说只允许出现标准库头文件，如果引入标准库头文件之外的头文件报错
 *          不检查哪些头文件在哪个版本的C、C++废弃，这些由gcc和g++编译器指定的C++和C版本控制
*/
struct OJCHelper 
{
    static std::vector<std::string> StdInclude;
};

std::vector<std::string> OJCHelper::StdInclude = {
    "assert.h", "complex.h", "ctype.h", "errno.h", "fenv.h",
    "float.h", "inttypes.h", "iso646.h", "limits.h", "locale.h",
    "math.h", "setjmp.h", "signal.h", "stdalign.h", "stdarg.h", 
    "stdatomic.h", "stdbit.h", "stdbool.h", "stdckdint.h", "stddef.h",
    "stdint.h", "stdio.h", "stdlib.h", "stdnoreturn.h", "string.h", "tgmath.h",
    "threads.h", "time.h", "uchar.h", "wchar.h", "wctype.h",
};

struct OJCPlusPlusHelper 
{
    static std::vector<std::string> StdInclude;
};

std::vector<std::string> OJCPlusPlusHelper::StdInclude = {
    "concepts", "coroutine", "any", "bitset", "chrono", "compare",
    "csetjmp", "csignal", "cstdarg", "cstddef", "cstdlib", "ctime", 
    "debugging", "expected", "functional", "initializer_list", "optional",
    "source_location", "tuple", "tuple_traits", "typeindex", "typeinfo", "utility",
    "variant", "version", "memory", "memory_resource", "new", "scoped_allocator",
    "cfloat", "cinttypes", "climits", "limits", "cstdint", "limits", "stdfloat", 
    "cassert", "cerrno", "exception", "stacktrace", "stdexcept", "system_error",
    "cctype", "charconv", "cstring", "cuchar", "cwchar", "cwctype", "format", "string",
    "string_view", "array", "deque", "flat_map", "flat_set", "forward_list", "list", "map",
    "mdspan", "queue", "set", "span", "stack", "unordered_map", "unordered_set", "vector", 
    "iterator", "generator", "ranges", "algorithm", "execution", "bit", "cfenv", "cmath",
    "complex", "linalg", "numbers", "numeric", "random", "ratio", "valarray", "clocale",
    "codecvt", "locale", "text_encoding", "cstdio", "fstream", "iomanip", "ios", "iosfwd",
    "iostream", "ostream", "istream", "print", "spanstream", "sstream", "streambuf", "filesystem",
    "syncstream", "regex", "atomic", "barrier", "condition_variable", "future", "hazard_pointrt",
    "latch", "mutex", "rcu", "semaphore", "shared_mutex", "stop_token", "thread", "stdatomic.h",
    "ccomplex", "complex.h", "ctgmath", "tgmath.h", "ciso646", "cstdalign", "cstdbool", "iso646.h",
};

OJFileCheck::ErrorType checkC(const std::vector<std::string>& content, std::string& error);
OJFileCheck::ErrorType checkCPlusPlus(const std::vector<std::string>& content, std::string& error);

OJFileCheck::ErrorType OJFileCheck::check(const std::vector<std::string>& content)
{
    if (language.empty()) {
        return OJFileCheck::NOT_SET_LANG;
    }

    if (language == "C") {
        return checkC(content, error);
    } else if (language == "C++") {
        return checkCPlusPlus(content, error);
    }

    return OJFileCheck::CHECK_SUCCESS;
}

OJFileCheck::ErrorType checkC(const std::vector<std::string>& content, std::string& error)
{
    std::regex importRegex(R"(^\s*#include\s*<([^>]*)>)");
    std::vector<std::string> imports = OJCHelper::StdInclude;

    for (auto line : content) {
        std::smatch match;
        if (std::regex_search(line, match, importRegex)) {
            int flag=0;
            std::string header = match[1];
            for (auto stdHeader : imports) {
                if (stdHeader == header) {
                    flag=1; break;
                }
            }
            if (flag == 0) {
                error = std::string("Fatal error because the C standard library does not have ")+header;
                return OJFileCheck::IMPORT_ERROR;
            }
        }
    }

    return OJFileCheck::CHECK_SUCCESS;

}

OJFileCheck::ErrorType checkCPlusPlus(const std::vector<std::string>& content, std::string& error) 
{
    std::regex importRegex(R"(^\s*#include\s*<([^>]*)>)");
    std::vector<std::string> imports = OJCHelper::StdInclude;

    for (auto include : OJCPlusPlusHelper::StdInclude) 
        imports.push_back(include);

    for (auto line : content) {
        std::smatch match;
        if (std::regex_search(line, match, importRegex)) {
            int flag=0;
            std::string header = match[1];
            for (auto stdHeader : imports) {
                if (stdHeader == header) {
                    flag=1; break;
                }
            }
            if (flag == 0) {
                error = std::string("Fatal error because the C++ standard library does not have ")+header;
                return OJFileCheck::IMPORT_ERROR;
            }
        }
    }

    return OJFileCheck::CHECK_SUCCESS;
}

OJ_NAMESPACE_END