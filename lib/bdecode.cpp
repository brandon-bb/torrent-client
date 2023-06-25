#include <bdecode.hpp>


std::string torrent::bdecode::read_file (std::string filepath) 
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

  //error for file not exisiting
  //error for file not being a .torrent file
  //error for contents not being utf-8 encoded
}

bool torrent::bdecode::validate_bencode (std::string bencode)
{
  /******VALID BENCODE******
  - utf8-encoded
  - formatting of types -  relevant error code
  */
  
}

std::string torrent::bdecode::decode_string (std::string input)
{
  std::string result = "";
  std::vector<int> variables = length_and_string(input);
  int length = variables[0];
  int c = variables[1];

  while (length) 
  {
    result += input[c];
    c++;
    length--;
  }

  return result;
  //
}

std::int64_t torrent::bdecode::decode_integer (std::string input, bool is_string = false)
{
  std::string integer;
  int i = 0;

  /*****decoding string length errors*******
   * no colon present
   * non-numeric characters
   * leading zero
   * -0
  */
  //error code if no colon present
  if (is_string)
  {
    bool numeric = true;
    
    while (numeric)
    {
      if (isdigit(input[i]))
      {
      integer += input[i];
      i++;
      }

      else
      {
        numeric = false;
      }
    }

    is_string = false;
  }



  /*****bencoded integer errors*****
   * non-numeric characters
   * no value between ie
   * -0
   * leading zeros
  */
  while (input[i] != 'e')
  {
    integer += input[i];
    i++;
  }

  return std::stoi(integer);
}

std::vector<std::any> torrent::bdecode::decode_list(std::string input)
{
  std::vector<std::any> result;
  int i = 0;

  return result;
  //error code for improper format
}

std::map<std::any, std::any> torrent::bdecode::decode_dictionary (std::string input)
{
  std::map<std::any, std::any> result;
  int i = 0;

  //find a way to detect if a pair has been made then add to dict.

  return result;

  //similar to list

}