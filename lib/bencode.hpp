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
    std::string encode_string (std::string input);
    std::string encode_integer (int64_t input);
    std::string encode_list (std::vector<std::any> input);
    std::string encode_dictionary (std::map<std::any, std::any> input);
    
    std::string to_string () const;

public:
    const std::string bencode;
};
}