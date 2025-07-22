#include "string"
class Helper
{
public:
    static void printStringData(std::string data);
    static std::string gbk_to_utf8(const std::string &gbk);
    static std::string utf8_to_gbk(const std::string &utf8);
};