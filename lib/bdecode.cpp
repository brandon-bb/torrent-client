#include <bdecode.hpp>

std::string torrent::bdecode::decode_string (std::string input)
{
  std::string result = "";
  const int expected_length = decode_integer (input, true);
  int string = (log10(expected_length) + 1); //number of digits in length
  
  if (input[string] != ':')
  {
    //error code - invalid bencoding, string not properly encoded
  }

  for (int i = string + 1; i < expected_length; i++)
  {
    result += input[i];
  }

  if (result.length() != expected_length)
  {
    //error code - invalid string_length, length of string does not match expected length
  }

  return result;

}

std::int64_t torrent::bdecode::decode_integer (std::string input, bool is_string = false)
{
  std::string integer;
  int i = 0;

  /*****decoding string length errors*******
   * non-numeric characters //check in loop
   * leading zero
   * negative length
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
  //error code for improper format (end denoting)
}

std::map<std::any, std::any> torrent::bdecode::decode_dictionary (std::string input)
{
  std::map<std::any, std::any> result;
  int i = 0;

  //find a way to detect if a pair has been made then add to dict.

  return result;

  //similar to list

}