#include <iostream>
#include <regex>
#include <sstream>

int main(int argc, char *argv[]) {
    int size;
    std::string base = argv[1];
    std::regex re("\\d+");
    std::smatch match;
    if (std::regex_search(base, match, re) > 1) base = match.str(1);
    std::stringstream sconverter(base);
    sconverter >> base;
    std::cout << size << std::endl;
    return 0;
}