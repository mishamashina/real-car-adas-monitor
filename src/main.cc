#include "obd_parser.hpp"

#include <iostream>

int main()
{
    std::cout << "Hello world" << std::endl;

    OBDParser parser = OBDParser::create();

    return 0;
}