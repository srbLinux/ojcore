#ifndef _OJ_CORE_DEF_H
#define _OJ_CORE_DEF_H

#define OJ_NAMESPACE_BEGIN namespace oj {

#define OJ_NAMESPACE_END }

OJ_NAMESPACE_BEGIN

#ifdef __linux__
#define __OJ_EXPORT_API__ __attribute__((visibility("default")))
#define __OJ_EXPORT_ENUM__ __attribute__((visibility("default")))
#define __OJ_EXPORT_STRUCT__ __attribute__((visibility("default")))
#define __OJ_C_EXPORT_API__ extern "C" __attribute__((visibility("default")))
#elif _WIN32_
#define __OJ__EXPORT_API__
#else
#define __OJ__EXPORT_API__
#endif

#if __linux__
#if (_GLIBCXX_STRING && _GLIBCXX_VECTOR)
typedef std::vector<std::string> OJStringList;
#endif
#endif

#define CHECK_CLASS_HAS_MEMBERORFUN(type,member) class_has_check_##member<type>::Value

#define CLASS_HAS_MEMBERORFUN_IMPL(member) \
    template<typename T> \
    struct class_has_check_##member\
    {\
        template<typename U> static auto check(...)->int;\
        template<typename U> static auto check(decltype(&U::##member))->void;\
        constexpr static bool Value = std::is_void_v<decltype(check<T>(0))>; \
    };

OJ_NAMESPACE_END

typedef enum {
    OUT_OF_TIME, OUT_OF_MEMORY, NOT_STD_IMPORT, NOT_STD_API, 
    NO_ERROR, SERVER_INTERNAL_ERROR, COMPLIER_ERROR,
} OJJudgeState;

typedef enum {
    CPLUSPLUS98, C11, JAVA, PYTHON2, PYTHON3,
    CPLUSPLUS11, CPLUSPLUS14, CPLUSPLUS17, C99,
} OJJudgeLanguage;

OJ_NAMESPACE_BEGIN

OJ_NAMESPACE_END

extern "C" 
{
    typedef void (*judge_server_result)(const char *result, int code);
}

#endif