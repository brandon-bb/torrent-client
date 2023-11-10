#include "errors.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <variant>
#include <expected>
#include <optional>
#include <filesystem>
#include <fstream>

namespace TorrentLib {

struct Metadata;
using list = std::vector<Metadata>;
using dict = std::map<std::string, Metadata>;

struct Metadata {
  std::variant <int64_t, std::string, list, dict> value;

  Metadata ();
  Metadata (const int64_t& data) : value (data) {}
  Metadata (const std::string& data) : value (data) {}
  Metadata (const std::vector<Metadata>& store) : value (store) {}
  Metadata (const std::map<std::string, Metadata>& store) : value (store) {}
};

/*
* T
*/
class Decoder final
{
public:
  enum Token {
    end = 'e',
    dict = 'd',
    integer = 'i',
    list = 'l',
    string = '0' //can handle [0-9]
  };

using metadata_object = std::expected<Metadata, TorrentError<Error::DecodeError>>;

public:
  Decoder (const std::string& value);
  Decoder (const std::filesystem::path& filepath);

private:
  template <typename T, typename E, typename Container, typename KeyType>
  const void InsertValue (const std::expected<T, E>& result, Container& container, 
    const std::optional<KeyType>& key = std::nullopt);

  Token GetNextToken (Token& token);
  void AdvanceIterator();
  bool ValidKey (std::expected<Metadata, TorrentError<Error::DecodeError>>& key);
  std::string ExtractKey (std::expected<Metadata, TorrentError<Error::DecodeError>> &key);
  
  auto StringLength () -> std::expected<const int64_t, TorrentError<Error::DecodeError>>;
  auto DecodeString () -> std::expected<Metadata, TorrentError<Error::DecodeError>>;
  auto DecodeInteger () -> std::expected<Metadata, TorrentError<Error::DecodeError>>;
  auto DecodeList () -> std::expected<Metadata, TorrentError<Error::DecodeError>>;
  auto DecodeDict () -> std::expected<Metadata, TorrentError<Error::DecodeError>>;
  
  std::string to_string () const;


private:
  const std::string encoded_value;
  std::string::iterator decode_iterator;
  std::map<std::string, Metadata> torrent_metadata;
};

}