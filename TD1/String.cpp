#include "String.hh"
#include "mystring.hh"
namespace pr {
    String::String(const char* ori):str(ori){}
    size_t String::length()const{
        return pr::length(str);
    }
    int String::compare(const char *a, const char *b)
    {
        const char* pa = a;
        const char* pb = b;
        
        while(*pa == *pb && *pa){
            pa++;
            pb++;
        }
        if(!*pa && !*pb){
            return 0;
        }
        if(!*pa || *pa<*pb){
            return -1;
        }
        return 1;
    }
    
    bool String::operator<(const String &b) const
    {
        if(this->str<b.str){
            return true;
        }
        return false;
    }

    bool operator==(const String &a, const String &b)
    {
        if(a.length() != b.length()){
            return false;
        }
        for(size_t i = 0; i<a.length(); ++i){
            if(a.str[i] != b.str[i]){
                return false;
            }
        }
        return true;
    }

     int String::compare2(const String &a, const String &b)
    {
        if(operator==(a,b)){
            return 0;
        }
        if(a.operator<(b)){
            return 1;
        }
        return -1;
    }
}