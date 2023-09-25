#pragma once
#include <cstddef>
namespace pr{
    class String{
        const char* str;
    public:
        String(const char* ori = ""); //Constructeur
        size_t length()const;
        int compare(const char* a, const char* b);
        friend bool operator==(const String &a,const String &b);
        bool operator<(const String & b) const;
        int String::compare2(const String &a, const String &b);
    };
    bool operator==(const String &a,const String &b);
}