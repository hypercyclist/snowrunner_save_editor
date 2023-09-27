#include "Utils.h"

bool Utils::contains(std::string& _source, std::string _string)
{
    return stolower(_source).find(stolower(_string)) != std::string::npos;
}

// Можно добавить while.
std::string Utils::replace(std::string& _source, std::string _from, std::string _to)
{
//    std::string::size_type position = stolower(_source).find(stolower(_source));

//    std::string prefix = _source.substr(0, position);
//    int postfixStart = position + _from.size();
//    int postfixLength = _source.length()-postfixStart;
//    std::string postfix = _source.substr(postfixStart, postfixLength);

//    return  prefix + _to + postfix;
    return _source;
}

std::string Utils::cutLongCountryName(std::string _countryName)
{
    if (contains(_countryName, "Российская Федерация"))
    {
        _countryName = replace(_countryName, "Российская Федерация", "РФ");
    }
    return _countryName;
}

std::string Utils::cutSlash(std::string _string)
{
    if (contains(_string, "\\"))
    {
        _string = replace(_string, "\\", "");
    }
    return _string;
}

std::string Utils::stolower(std::string _string)
{
    for(unsigned int i = 0; i  < _string.length(); i++)
        _string[i] = std::tolower(_string[i]);
//    std::transform(_string.begin(), _string.end(), _string.begin(),
//        [](unsigned char c){ return std::tolower(c); });
    return _string;
}
