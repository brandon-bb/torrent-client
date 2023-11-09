#include "decoder.hpp"


namespace TorrentLib 
{

/*
in the actual implementation, the file path would be passed to the constructor,
we would parse the contents of the metainfo (.torrent) file to our encoded_value variable.
As the content of a metainfo file is an benncoded dictionary, a check would be put in place to verify
this before we begin decoding.
*/
// make a class that handles file operations
Decoder::Decoder (const std::filesystem::path& filepath) : encoded_value ("") 
{
  if (!std::filesystem::exists (filepath)) {
    //return error - file does not exist
  }

  else if (!filepath.has_extension() || filepath.extension() != ".torrent") {
    //return error - invalid file format
  }
}


void Decoder::AdvanceIterator () { decode_iterator++; }

Decoder::Token Decoder::GetNextToken (Token& token) 
{
  token = static_cast<Token> (*decode_iterator);
  AdvanceIterator();
  return token;
}

bool Decoder::ValidKey (std::expected<Metadata, TorrentError<Error::DecodeError>>& key) {
  if (std::holds_alternative <std::string> (key->value)) {
    return true;
  }
  return false;
}

std::string Decoder::ExtractKey (std::expected<Metadata, TorrentError<Error::DecodeError>>& key) {
  if (!ValidKey (key)) {
    //return error
  }

  return std::get<std::string> (key->value);
}

//plan to add appropriate error checks here;
template <typename T, typename E, typename Container, typename KeyType>
const void Decoder::InsertValue (const std::expected<T, E>& input, Container& container,
  const std::optional<KeyType>& key)
{
  if (input.has_value()) {
    if constexpr (std::is_same_v<Container, std::vector<T>>) {
      container.push_back (*input);
    }

    else if constexpr (std::is_same_v<Container, std::map<std::string, T>>) {
      if constexpr (std::is_same_v<KeyType, std::string>) {
        if (key.has_value () && ValidKey (key)) { 
          const std::string extracted_key = ExtractKey (key);
          container.insert({ extracted_key, *input });
          }
      }

      else {
      //error for no key provided
      }
    }

    else {
      static_assert (std::is_same_v<Container, std::vector<T>> || std::is_same_v<Container, std::map<std::string, T>>,
        "Unsupported container type");
    }
  }
}

auto Decoder::StringLength () -> std::expected<const int64_t, TorrentError<Error::DecodeError>>
{
  std::string length;
  while (decode_iterator != encoded_value.end() && *decode_iterator != ':') {
    if (!std::isdigit (*decode_iterator)) {
      //error: invalid character in bencoded string length
      std::unexpected (TorrentError { Error::DecodeError::invalid_encoding, "Non-numeric character found in integer encoding"});
    }

    length += *decode_iterator;
    AdvanceIterator();
  }

  if (length.empty()) {
    return std::unexpected (TorrentError {Error::DecodeError::invalid_encoding, "No length value encoded"});
  }

  AdvanceIterator();
  
  return std::stoll(length);
}

//debugged
auto Decoder::DecodeString () -> std::expected<Metadata, TorrentError<Error::DecodeError>>
{
  std::string result;
  int64_t expected_length = 0;

  auto process = [this, &expected_length]() {
    if (const auto num = StringLength(); num.has_value())
      expected_length = *num;
  };

  int expected_colon = std::to_string (expected_length).length();

  std::advance (decode_iterator, expected_colon);
  if (*decode_iterator != ':') {
    //error: invalid bencoding - no colon at expected position
    std::unexpected (TorrentError { Error::DecodeError::invalid_encoding, "Non-numeric character found in integer encoding"});
  }

  AdvanceIterator();
  int i = 0;
  while (decode_iterator != encoded_value.end() && i < expected_length) {
    result += *decode_iterator;
    AdvanceIterator();
    i++;
  }

  AdvanceIterator();
  return Metadata(result);

}

//debugged
auto Decoder::DecodeInteger () -> std::expected<Metadata, TorrentError<Error::DecodeError>>
{
  std::string integer;

  if (*decode_iterator != 'i') {
    std::unexpected (TorrentError { Error::DecodeError::invalid_encoding, "Non-numeric character found in integer encoding"});
  }
  AdvanceIterator();

  if (*decode_iterator == '-') {
    integer += *decode_iterator;
    AdvanceIterator();

    if (*decode_iterator == '0') {
      std::unexpected (TorrentError { Error::DecodeError::invalid_encoding, "Non-numeric character found in integer encoding"});
    }
  }

  if (*decode_iterator == '0' && *(decode_iterator + 1) != 'e') {
    // error: invalid bencoding, leading zero
    std::unexpected (TorrentError { Error::DecodeError::invalid_encoding, "Non-numeric character found in integer encoding"});
  }

  while (*decode_iterator != 'e') {
    if (!std::isdigit (*decode_iterator)) {
      //error: invalid character in bencoded string
      std::unexpected (TorrentError { Error::DecodeError::invalid_encoding, "Non-numeric character found in integer encoding"});
    }

    integer += *decode_iterator;
    AdvanceIterator();
  }

  return Metadata(std::stoll(integer));
}

/*
need to revise the solutions for decode_list/dict to handle the edge case
of no end symbol.
this would be a corrupt file as the decoding process would assume every
entry belongs to the list when in reality there just isn't an end signifier
*/

auto Decoder::DecodeList () -> std::expected<Metadata, TorrentError<Error::DecodeError>>
{
  std::vector<Metadata> result;
  std::stack<char> stack;
  
  if (*decode_iterator != 'l') {
    //return error for incorrect bencoding
    std::unexpected (TorrentError {Error::DecodeError::invalid_encoding, "Invalid bencode"});
  }
  
  stack.push (*decode_iterator);
  AdvanceIterator();
  Token current_token;

  while (!stack.empty() && decode_iterator != encoded_value.end()) {
    current_token = GetNextToken (current_token);
    std::optional<std::string> optional_key;

    switch (current_token) {
      case Token::list:
      {
        stack.push (*decode_iterator);
        break;
      }
      
      case Token::end:
      {
        stack.pop();
        break;
      }

      case Token::dict:
      {
        auto dict_result = DecodeDict();
        InsertValue (dict_result, result, optional_key);
        break;
      }

      case Token::string:
      {
        auto string_result = DecodeString();
        InsertValue (string_result, result, optional_key);
        break;
      }

      case Token::integer:
      {
        auto int_result = DecodeInteger();
        InsertValue (int_result, result, optional_key);
        break;
      }

      default:
        //error message - unknown character
        break;
    }
  }

  //check for extra 'l' characters to handle stack underflow
  if (!stack.empty()) {
    std::unexpected (TorrentError { Error::DecodeError::stack_underflow, "Stack underflow detected: end characters are not equivalent to given list start characters"});
  }

  return Metadata (result);
}

//strategy
// check if 'd' can be put onto stack
// check if character after d is numeric, indicates that this is a key
// decode - we check after if the key flag has been set and we have a string
// when value has been decoded, we then add it to the map
// similar approach to decode_list
auto Decoder::DecodeDict () -> std::expected<Metadata, TorrentError<Error::DecodeError>>
{
  std::map <std::string, Metadata> decoded_dict;
  std::stack <char> stack;


  if (*decode_iterator != 'd') {
    std::unexpected (TorrentError {Error::DecodeError::invalid_encoding, "Invalid encoding"});
  }
  stack.push (*decode_iterator);
  AdvanceIterator();

  bool expect_key = true;
  std::optional<metadata_object> temp_key = Metadata {};
  Token current_token;

  while (!stack.empty() && decode_iterator != encoded_value.end())
  {
    current_token = static_cast<Token> (*decode_iterator);
    
    switch (current_token) {
      case Token::string:
      {
        auto decoded_string = DecodeString();

        if (expect_key) {
          temp_key = DecodeString();
          expect_key = false;
        }

        else {
          InsertValue (decoded_string, decoded_dict, temp_key);
          expect_key = true;
        }
        break;
      }
      
      case Token::dict:
      {
        stack.push (*decode_iterator);
        break;
      }

      case Token::end:
      {
        stack.pop();
        break;
      }

      case Token::integer:
      {
        auto decoded_integer = DecodeInteger ();

        if (expect_key) {
          //return error - incorrect format
        }

        InsertValue (decoded_integer, decoded_dict, temp_key);
        break;
      }

      case Token::list:
      {
        auto decoded_list = DecodeList();

        if (expect_key) {
          // return error - incorrect dict format
        }
        InsertValue (decoded_list, decoded_dict, temp_key);
        break;
      }
    }

    AdvanceIterator();
  }

  if (!stack.empty()) {
    //return error - stack underflow
  }

  return Metadata (decoded_dict);
}

}