#include <iostream>
#include <string>
#include <system_error>

namespace torrent
{

enum class file_errors
{
  generic = 1,
  file_not_exist,
  not_torrent_file,
};


enum class bencode_errors
{
  generic = 1,
  invalid_bencoding,
  invalid_utf8,
  leading_zero,
  no_integer_value,
  non_numeric,
  invalid_string_length,
};

enum tracker_errors
{

};
};