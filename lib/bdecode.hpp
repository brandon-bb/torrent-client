#pragma once

#include <system_error>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <any>

namespace torrent
{

class bdecode final
{
public:
    std::string readFile (std::string filepath);
    std::string decodeString (std::string input);
    std::int64_t decodeInt (std::string input);
    std::vector<int> length_and_string (std::string input); //helper function to return length of string and colon index in string
    std::vector<std::any> decodeList (std::string input);
    std::map<std::any, std::any> decodeDictionary (std::string input);
    
    std::string to_string () const;

public:
    const std::string bdecode;
};
}
