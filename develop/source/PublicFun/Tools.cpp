#include "Tools.h"



vector<string> Tools::SplitString(const string &src, const string &pattern)
{
    std::string::size_type pos1,pos2;
    std::vector<std::string> result;
    pos2 = src.find(pattern);
    pos1 = 0;
    while(string::npos != pos2)
    {
        //去除所有空格
        if (pos2 != pos1)
        {
            result.push_back(src.substr(pos1,pos2-pos1));
        }
        pos1 = pos2 + pattern.size();
        pos2 = src.find(pattern,pos1);
    }
    if (pos1 != src.size())
    {
        result.push_back(src.substr(pos1));
    }
    return result;
}

bool Tools::is_number(const string &str)
{
    if (str.empty())
    {
        return false;
    }
    for (decltype(str.size()) i = 0; i < str.size(); ++i)
    {
        if (!::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}
