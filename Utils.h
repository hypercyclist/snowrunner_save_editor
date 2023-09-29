#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

class Utils
{
public:
    static bool contains(std::string& _source, std::string _string);
    static std::string replace(std::string& _source, std::string _from, std::string _to);
    static std::string cutLongCountryName(std::string _countryName);
    static std::string cutSlash(std::string _string);
    static std::string stolower(std::string _string);
    static std::vector<std::string> split(std::string _source, char _splitter);
    static std::string simplified(std::string _string);
private:
    Utils();
};

#endif // UTILS_H
