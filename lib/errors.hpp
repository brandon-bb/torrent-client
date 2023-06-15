#include <iostream>
#include <string>
#include <system_error>

namespace torrent
{
enum class bencode_errors
{
    generic = 1,
    invalid_bencoding,
    negative_zero,
    leading_zero,
    no_integer
};
};