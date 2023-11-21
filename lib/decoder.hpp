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

public:
  Decoder (const std::string& value);
  Decoder (const std::filesystem::path& filepath);
  ~Decoder();

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
  
  std::string ToString () const;


private:
  const std::string encoded_value;
  std::string::iterator decode_iterator;
  std::unordered_map<std::string, Metadata> torrent_metadata;
  
  using variant_type = std::variant <int64_t, std::string, std::vector<Metadata>, 
    std::unordered_map<std::string, Metadata>>;

  template <typename T>
  void Visit (const variant_type& variant, std::stack<std::string>& context_stack, std::string& result);
};

}