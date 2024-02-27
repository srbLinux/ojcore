#ifndef _OJ_CORE_UTILS_VARIANT_H
#define _OJ_CORE_UTILS_VARIANT_H

#include <string>
#include <iostream>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJVariantPrivate;

class OJVariant
{
public:
    OJVariant();
    OJVariant(int i);
    OJVariant(char c);
    OJVariant(bool b);
    OJVariant(long l);
    OJVariant(short s);
    OJVariant(float f);
    OJVariant(double d);
    OJVariant(wchar_t wc);
    OJVariant(long long ll);
    OJVariant(long double ld);
    OJVariant(const char *str);
    OJVariant(const wchar_t *wstr);
    OJVariant(const std::string& str);
    OJVariant(const std::wstring& wstr);

    OJVariant(const OJVariant& other);

    enum Type 
    {
        INT, CHAR, WCHAR, LONG, DOUBLE, LONGLONG, BOOL,
        SHORT, FLOAT, LONGDOUBLE, STRING, WSTRING, NONE, INVIALID
    };

private:

    OJVariantPrivate *vptr;

    template<typename T>
    Type typeID();

    template<typename T>
    T numberConvert(const T& val);

    template<typename T>
    T strConvertNum(const std::string& str);

    template<typename T>
    T strConvertNum(const std::wstring& wstr);

public:
    int toInt();
    bool toBool();
    char toChar();
    long toLong();
    short toShort();
    float toFloat();
    double toDouble();
    wchar_t toWChar();
    long long toLongLong();
    std::string toString();
    std::wstring toWString();
    long double toLongDouble();

    friend std::ostream& operator<<(std::ostream& os, const OJVariant& var);
};

template<typename T>
OJVariant::Type OJVariant::typeID()
{
    if (std::is_same<T, int>::value) return INT;
    else if (std::is_same<T, bool>::value) return BOOL;
    else if (std::is_same<T, char>::value) return CHAR;
    else if (std::is_same<T, long>::value) return  LONG;
    else if (std::is_same<T, short>::value) return SHORT;
    else if (std::is_same<T, float>::value) return FLOAT;
    else if (std::is_same<T, wchar_t>::value) return WCHAR;
    else if (std::is_same<T, double>::value) return DOUBLE;
    else if (std::is_same<T, long long>::value) return LONGLONG;
    else if (std::is_same<T, std::string>::value) return STRING;
    else if (std::is_same<T, std::wstring>::value) return WSTRING;
    else if (std::is_same<T, long double>::value) return LONGDOUBLE;
}

OJ_NAMESPACE_END

#endif