#include <iostream>
#include <string>
#include <functional>
#include <system_error>
#include <utility>
#include <type_traits>

// this instance of th
namespace Error
{

enum CommonError
{
  common_generic = 1,
  file_not_exist,
  not_torrent_file,
};


enum DecodeError
{
  decode_generic = 1,
  invalid_encoding,
  stack_underflow
};

enum TrackerError
{
  tracker_generic = 1
};

}

namespace TorrentLib
{
template <typename T>
class TorrentError 
{
  public:
    TorrentError (T code, const std::string& message) 
      : error_code (code), error_message (message) {}

    T GetCode () const { return error_code; }
    const std::string& GetMessage () const { return error_message; }

  private:
    T error_code;
    std::string error_message;
};
}

