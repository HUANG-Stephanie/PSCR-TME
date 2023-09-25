#include "mystring.hh"
#include <iostream>
#include <cstring>
using namespace pr;

int main(){
    const char* str = "Hello World";
    char* copy = newcopy(str);
    std::cout << length(str) << std::endl;

    delete [] copy;

    return 0;
}