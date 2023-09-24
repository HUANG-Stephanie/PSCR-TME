#include "mystring.hh"
namespace pr {
    char* newcopy(const char* str){
        size_t len = length(str)+1;
        char* copy = new char[len+1];
        for(int i = 0; i<len; ++i){
            copy[i] = str[i];
        }
        return copy;
    }
    size_t length(const char* str)
    {
        const char* chaine = str;
        while(*chaine){
            ++chaine;
        }
        return chaine-str;
    }
}