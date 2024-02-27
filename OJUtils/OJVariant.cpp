#include "OJVariant.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <typeinfo>

OJ_NAMESPACE_BEGIN

class OJVariantPrivate
{
private:
    friend class OJVariant;
    union {
        int i;
        bool b;
        char c;
        long l;
        float f;
        short s;
        double d;
        wchar_t wc;
        long long ll;
        long double ld;
        void *ptr;
    } data;
    bool isNull;
    size_t ptrLen;
    OJVariant::Type type;

    OJVariantPrivate() : isNull(true), ptrLen(0), type(OJVariant::NONE) {  }

    OJVariantPrivate(const OJVariantPrivate& other) {
        if (!other.isNull) {
            type = other.type;
            switch (other.type) 
            {
            case OJVariant::INT: data.i = other.data.i; break;
            case OJVariant::BOOL: data.b = other.data.b; break;
            case OJVariant::CHAR: data.c = other.data.c; break;
            case OJVariant::LONG: data.l = other.data.l; break;
            case OJVariant::SHORT: data.s = other.data.s; break;
            case OJVariant::FLOAT: data.f = other.data.f; break;
            case OJVariant::DOUBLE: data.d = other.data.d; break;
            case OJVariant::WCHAR: data.wc = other.data.wc; break;
            case OJVariant::LONGLONG: data.ll = other.data.ll; break;
            case OJVariant::LONGDOUBLE: data.ld = other.data.ld; break;
            case OJVariant::STRING: std::memcpy(data.ptr, other.data.ptr, other.ptrLen); break;
            case OJVariant::WSTRING: std::memcpy(data.ptr, other.data.ptr, other.ptrLen); break;
            default: break;
            }
        } else {
            isNull = true;
            type = OJVariant::NONE;
        }
    }

    void operator=(const OJVariantPrivate& other) {
        if (!other.isNull) {
            type = other.type;
            switch (other.type) 
            {
            case OJVariant::INT: data.i = other.data.i; break;
            case OJVariant::BOOL: data.b = other.data.b; break;
            case OJVariant::CHAR: data.c = other.data.c; break;
            case OJVariant::LONG: data.l = other.data.l; break;
            case OJVariant::SHORT: data.s = other.data.s; break;
            case OJVariant::FLOAT: data.f = other.data.f; break;
            case OJVariant::DOUBLE: data.d = other.data.d; break;
            case OJVariant::WCHAR: data.wc = other.data.wc; break;
            case OJVariant::LONGLONG: data.ll = other.data.ll; break;
            case OJVariant::LONGDOUBLE: data.ld = other.data.ld; break;
            case OJVariant::STRING: std::memcpy(data.ptr, other.data.ptr, other.ptrLen); break;
            case OJVariant::WSTRING: std::memcpy(data.ptr, other.data.ptr, other.ptrLen); break;
            default: break;
            }
        } else {
            isNull = true;
            type = OJVariant::NONE;
        }
    }
public:
    friend std::ostream& operator<<(std::ostream& os, const OJVariantPrivate& var) 
    {
        switch (var.type)
        {
        case OJVariant::INT: os << var.data.i; break;
        case OJVariant::BOOL: os << var.data.b; break;
        case OJVariant::CHAR: os << var.data.c; break;
        case OJVariant::LONG: os << var.data.l; break;
        case OJVariant::SHORT: os << var.data.s; break;
        case OJVariant::FLOAT: os << var.data.f; break;
        case OJVariant::WCHAR: os << var.data.wc; break;
        case OJVariant::DOUBLE: os << var.data.d; break;
        case OJVariant::LONGLONG: os << var.data.ll; break;
        case OJVariant::LONGDOUBLE: os << var.data.ld; break;
        case OJVariant::STRING: os << std::string(static_cast<char *>(var.data.ptr)); break;
        case OJVariant::WSTRING: std::wcout << std::wstring(static_cast<wchar_t *>(var.data.ptr)); break;
        default: break;
        }
        return os;
    }
};

OJVariant::OJVariant() : vptr(new OJVariantPrivate) {  }

OJVariant::OJVariant(int i) : vptr(new OJVariantPrivate) { vptr->data.i = i; vptr->type=INT; vptr->isNull=false; }

OJVariant::OJVariant(char c) : vptr(new OJVariantPrivate) { vptr->data.c = c; vptr->type=CHAR; vptr->isNull=false; }

OJVariant::OJVariant(long l) : vptr(new OJVariantPrivate) { vptr->data.l = l; vptr->type=LONG; vptr->isNull=false; }

OJVariant::OJVariant(float f) : vptr(new OJVariantPrivate) { vptr->data.f = f; vptr->type=FLOAT; vptr->isNull=false; }

OJVariant::OJVariant(short s) : vptr(new OJVariantPrivate) { vptr->data.s = s; vptr->type=SHORT; vptr->isNull=false; }

OJVariant::OJVariant(double d) : vptr(new OJVariantPrivate) { vptr->data.d = d; vptr->type=DOUBLE; vptr->isNull=false; }

OJVariant::OJVariant(wchar_t wc) : vptr(new OJVariantPrivate) { vptr->data.wc = wc; vptr->type=WCHAR; vptr->isNull=false; }

OJVariant::OJVariant(long long ll) : vptr(new OJVariantPrivate) { vptr->data.ll = ll; vptr->type = LONGLONG; vptr->isNull=false; }

OJVariant::OJVariant(long double ld) : vptr(new OJVariantPrivate) { vptr->data.ld = ld; vptr->type = LONGDOUBLE; vptr->isNull=false; }

OJVariant::OJVariant(const char *str) : vptr(new OJVariantPrivate) {
    vptr->ptrLen = std::strlen(str) * sizeof(char);
    vptr->data.ptr = new char[std::strlen(str)+10];
    std::memcpy(vptr->data.ptr, str, std::strlen(str)); 
    vptr->type = STRING; vptr->isNull = false;
}

OJVariant::OJVariant(const wchar_t *str) : vptr(new OJVariantPrivate) {
    vptr->ptrLen = std::wcslen(str) * sizeof(wchar_t);
    vptr->data.ptr = new wchar_t[std::wcslen(str)+10];
    std::memcpy(vptr->data.ptr, str, std::wcslen(str));
    vptr->type = WSTRING; vptr->isNull = false;
}

OJVariant::OJVariant(const std::string& str) : vptr(new OJVariantPrivate) {
    vptr->data.ptr = new char[str.size()+10];
    vptr->ptrLen = str.size() * sizeof(char);
    std::memcpy(vptr->data.ptr, str.c_str(), str.size());
    vptr->type = STRING; vptr->isNull = false;
}

OJVariant::OJVariant(const std::wstring& wstr) : vptr(new OJVariantPrivate) {
    vptr->data.ptr = new wchar_t[wstr.size()+10];
    vptr->ptrLen = wstr.size() * sizeof(wchar_t);
    std::memcpy(vptr->data.ptr, wstr.c_str(), wstr.size());
    vptr->type = WSTRING; vptr->isNull = false;
}

OJVariant::OJVariant(const OJVariant& other)
{
    vptr = other.vptr;    
}

int OJVariant::toInt() { return numberConvert<int>(vptr->data.i); }

char OJVariant::toChar() { return numberConvert<char>(vptr->data.c); }

bool OJVariant::toBool() { return numberConvert<bool>(vptr->data.b); }

long OJVariant::toLong() { return numberConvert<long>(vptr->data.l); }

float OJVariant::toFloat() { return numberConvert<float>(vptr->data.f); }

double OJVariant::toDouble() { return numberConvert<double>(vptr->data.d); }

wchar_t OJVariant::toWChar() { return numberConvert<wchar_t>(vptr->data.wc); }

long long OJVariant::toLongLong() { return numberConvert<long long>(vptr->data.ll); }

long double OJVariant::toLongDouble() { return numberConvert<long double>(vptr->data.ld); }

std::string OJVariant::toString() 
{
    if (STRING==vptr->type) {
        return static_cast<char *>(vptr->data.ptr);
    }
    std::stringstream stream;
    switch (vptr->type)
    {
    case INT: return std::to_string(vptr->data.i); break;
    case CHAR: stream << vptr->data.c; break;
    case BOOL: return vptr->data.b == true ? "true" : "false"; break;
    case LONG: return std::to_string(vptr->data.l); break;
    case SHORT: return std::to_string(vptr->data.s); break;
    case WCHAR: break;
    case FLOAT: return std::to_string(vptr->data.f); break;
    case DOUBLE: return std::to_string(vptr->data.d); break;
    case LONGLONG: return std::to_string(vptr->data.ll); break;
    case LONGDOUBLE: return std::to_string(vptr->data.ld); break;
    default: break;
    }
    return stream.str();
}

std::wstring OJVariant::toWString()
{
    if (WSTRING == vptr->type) {
        return static_cast<wchar_t *>(vptr->data.ptr);
    }
    std::wstringstream wstream;
    switch (vptr->type)
    {
    case INT: return std::to_wstring(vptr->data.i); break;
    case WCHAR: wstream << vptr->data.c; break;
    case BOOL: return vptr->data.b == true ? L"true" : L"false"; break;
    case LONG: return std::to_wstring(vptr->data.l); break;
    case SHORT: return std::to_wstring(vptr->data.s); break;
    case CHAR: break;
    case FLOAT: return std::to_wstring(vptr->data.f); break;
    case DOUBLE: return std::to_wstring(vptr->data.d); break;
    case LONGLONG: return std::to_wstring(vptr->data.ll); break;
    case LONGDOUBLE: return std::to_wstring(vptr->data.ld); break;
    default: break;
    }
    return std::wstring();
}

std::ostream& operator<<(std::ostream& os, const OJVariant& var)
{
    os << (*var.vptr);
    return os;
}

template<typename T>
T OJVariant::numberConvert(const T& val)
{
    if (vptr->type == typeID<T>())
        return val;
    switch (vptr->type) 
    {
    case INT: return T(vptr->data.i); break;
    case BOOL: return T(vptr->data.b); break;
    case CHAR: return T(vptr->data.c); break;
    case LONG: return T(vptr->data.l); break;
    case SHORT: return T(vptr->data.s); break;
    case FLOAT: return T(vptr->data.f); break;
    case DOUBLE: return T(vptr->data.d); break;
    case WCHAR: return T(vptr->data.wc); break;
    case LONGLONG: return T(vptr->data.ll); break;
    case LONGDOUBLE: return T(vptr->data.ld); break;
    case STRING: return strConvertNum<T>(static_cast<char *>(vptr->data.ptr)); break;
    case WSTRING: return strConvertNum<T>(static_cast<wchar_t *>(vptr->data.ptr)); break;
    default: break;
    }
    return T();
}

template<typename T>
T OJVariant::strConvertNum(const std::wstring& wstr)
{
    wchar_t **end;
    switch (typeID<T>())
    {
    case INT:
    case BOOL:
    case CHAR:
    case LONG:
    case WCHAR:
    case SHORT:
        return std::wcstol(wstr.c_str(), end, 10); break;
    case FLOAT:
        return std::wcstof(wstr.c_str(), end); break;
    case DOUBLE:
        return std::wcstod(wstr.c_str(), end); break;
    case LONGLONG:
        return std::wcstoll(wstr.c_str(), end, 10); break;
    case LONGDOUBLE:
        return std::wcstold(wstr.c_str(), end); break;
    default: break;
    }
    return T();
}

template<typename T>
T OJVariant::strConvertNum(const std::string& str) 
{
    switch (typeID<T>())
    {
    case INT:
    case BOOL:
    case CHAR:
    case WCHAR:
    case SHORT:
        return std::stoi(str); break;
    case LONG:
        return std::stol(str); break;
    case FLOAT:
        return std::stof(str); break;
    case DOUBLE:
        return std::stod(str); break;
    case LONGLONG:
        return std::stoll(str); break;
    case LONGDOUBLE:
        return std::stold(str); break;
    default: break;
    }
    return T();
}

OJ_NAMESPACE_END