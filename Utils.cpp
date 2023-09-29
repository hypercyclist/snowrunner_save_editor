#include "Utils.h"
#include <sstream>
#include <algorithm>

bool Utils::contains(std::string& _source, std::string _string)
{
    return stolower(_source).find(stolower(_string)) != std::string::npos;
}

// Можно добавить while.
// Не работает замена без учета регистра с кириллицей.
std::string Utils::replace(std::string& _source, std::string _from, std::string _to)
{
//    std::string::size_type position = stolower(_source).find(stolower(_from));
    while (_source.find(_from) != std::string::npos)
    {
        std::string::size_type position = _source.find(_from);

        std::string prefix = _source.substr(0, position);
        int postfixStart = position + _from.size();
        int postfixLength = _source.length() - postfixStart;
        std::string postfix = _source.substr(postfixStart, postfixLength);

        _source = prefix + _to + postfix;
    }
    return  _source;
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
    std::transform(_string.begin(), _string.end(), _string.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return _string;
}

std::vector<std::string> Utils::split(std::string _source, char _splitter)
{
    std::vector<std::string> strings;
    std::istringstream stream(_source);
    std::string buf;
    while (getline(stream, buf, _splitter))
    {
        strings.push_back(buf);
    }
    return strings;
}

std::string Utils::simplified(std::string _string)
{
    replace(_string, "\n", "");
    replace(_string, "\r", "");
    replace(_string, "\t", " ");
    replace(_string, "  ", " ");
    if (_string[0] == ' ')
    {
        _string.erase(0, 1);
    }
    if (_string[_string.size() - 1] == ' ')
    {
        _string.erase(_string.size() - 1, 1);
    }
    return _string;
}
