#include "String.hh"
#include <iostream>
using namespace pr;

int main(){
    const char* ori = "Hello";
    const char* s1 = "Hello";
    const char* s2 = "Hello World";
    String str(ori);
    std::cout<<str.length()<<std::endl;
    std::cout<<str.compare(s1,s2)<<std::endl;
    std::cout<<pr::operator==(ori,ori)<<std::endl;
    std::cout<<str.operator<(s2)<<std::endl;

    return 0;
}