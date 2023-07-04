#pragma once

#include <system_error>
#include <cmath>
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
  bool validate_bencode (std::string bencode);
  bool validate_utf8 (); //https://stackoverflow.com/questions/28270310/how-to-easily-detect-utf8-encoding-in-the-string
  std::string decode_string (std::string input);
  std::int64_t decode_integer (std::string input, bool is_string);
  std::vector<std::any> decode_list (std::string input);
  std::map<std::any, std::any> decode_dictionary (std::string input);
    
  std::string to_string () const;

public:
  const std::string bdecode;
  std::error_code ec;

};
}
