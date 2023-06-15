#include <bdecode.hpp>



std::string torrent::bdecode::readFile (std::string filepath) 
{
    std::string result;
    std::ifstream input;

    if (input)
    {
        while (!input.eof())
        {
            std::getline (input, result);
        }
    }

    return result;
}

/*
helper function to deduce the length of a string
first result is length, second is colon index
*/
std::vector<int> torrent::bdecode::length (std::string input)
{
    bool numeric = true;
    uint8_t i = 0;
    std::vector<int> result;
    std::string len;


    while (numeric)
    {
        if (isdigit (input[i]) ) {
            len += input[i];
        }

        else 
        {
            result.push_back(std::stoi(len));
            result.push_back(colonIndex(i));
            numeric = false;
        }
    }

    return result;
}

std::string torrent::bdecode::decodeString (std::string input)
{
    std::string result;
    std::vector<int> values = length(input);
    int len = values[0];
    int c = values[1] + 1; //we know location of colon so we can exclude it

    while (len) 
    {
        result += input[c];
        c++;
        len--;
    }

    return result;
}

std::int64_t torrent::bdecode::decodeInt (std::string input)
{
    std::string result;
    int i = 1; //we ignore the "i" delimiter

    while (input[i] != 'e')
    {
        result += input[i];
        i++;
    }

    return std::stoi(result);
}

std::vector<std::any> torrent::bdecode::decodeList (std::string input)
{
    std::vector<std::any> result;
    int i = 0;

    while (input[i] != 'e')
    {
        if (isdigit(input[i])) { result.push_back(decodeInt(input)); }
        
        if (input[i] == 'i') { result.push_back(decodeInt(input)); }

        if (input[i] == 'd') { result.push_back(decodeDictionary(input)); }

        if (input[i] == 'l') { result.push_back(decodeList(input)); }

        i++;
   }

    return result;
}

std::map<std::any, std::any> torrent::bdecode::decodeDictionary (std::string input)
{
    std::map<std::any, std::any> result;
    int i = 0;

    //find a way to detect if a pair has been made then add to dict.

    return result;
}