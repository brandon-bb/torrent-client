#include <bdecode.hpp>
#include <errors.hpp>

std::string torrent::bdecode::decode_string (std::string input)
{
  std::string result = "";
  const int expected_length = decode_integer (input, true);
  int expected_char = (log10(expected_length) + 2) ; //number of digits in length - indicates where the string starts
  
  if (input[expected_char] != ':')
  {
    //invalid bencoding
    return ec = static_cast<bencode_errors> (2);
    //append explanation of this error
  }

  for (int i = string + 1; i < expected_length; i++)
  {
    result += input[i];
  }

  if (result.length() != expected_length)
  {
    //error code - invalid string_length, length of string does not match expected length
    return ec = static_cast<bencode_errors> (7);
  }

  return result;

}

/*
This can be improved but it is a start.

decoding integers & string lengths operate similar but the
difference is that a string cannot have a negative length.

The bool flag in the function is used as an indicator to detect if a negative value
has been attached to a string - if so, we return an error.

As it is possible that a .torrent file could have a string like this -> "-:fooled you"
(correct format is 10:fooled you)
we check for this edge case before converting to integer.
*/
std::int64_t torrent::bdecode::decode_integer (std::string input, bool is_string = false)
{
  std::string integer;
  int i = 0;

  while (input[i] != ':')
  {
    if (i == 0 && input[i] == '-')
    {
      if (is_string)
      { 
        return ec = static_cast<bencode_errors> (7); 
      }
    }

    //handles -0 or leading zero
    else if ((i == 1 && negative_char == true)  || (i == 0 && input[i] == '0'))
    {
      return ec = static_cast<bencode_errors> (4);
    }


    else if (isdigit (input[i]) == false)
    {
      return ec = static_cast<bencode_errors> (6);
    }

    integer += input[i];
    i++;
  }

  if (integer == "-") { return ec = static_cast<bencode_errors> (6); }

  return std::stoi (integer);
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