#include <iostream>
#include <string>
#include "riscv_cpp_test.h"


class S {
    std::string s = "Hello CPP !";
public:
    const std::string& get() {return s;}
    
};

extern "C" {

unit hello_cpp() {
    S s;
    std::cout << s.get() << std::endl;
    return UNIT;
}

}