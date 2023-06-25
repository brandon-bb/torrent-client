#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <any>

namespace torrent
{

class bencode final
{
public:
    std::string readFile (std::string filepath);
    void validBencode (std::string text);
    std::string encodeString (std::string input);
    std::string encodeInt (int64_t input);
    std::vector<int> length (std::string input); //helper function to return length of string and colon index in string
    std::string encodeList (std::vector<std::any> input);
    std::string encodeDictionary (std::map<std::any, std::any> input);
    
    std::string to_string () const;

public:
    const std::string bencode;
};
}