#pragma once

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
    std::vector<int> length (std::string input); //helper function to return length of string and colon index in string
    int colonIndex (int index); //identifies where the colon is located - most likely will be removed soon once solution is fleshed out
    std::vector<std::any> decodeList (std::string input);
    std::map<std::any, std::any> decodeDictionary (std::string input);
    
    std::string to_string () const;

public:
    std::string bencode;
};
}
