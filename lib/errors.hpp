#include <iostream>
#include <string>
#include <functional>
#include <system_error>
#include <utility>

namespace torrent
{

enum class common_errors
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
  generic = 1
};

}

/* **CONCEPT - NOT ACTUAL IMPLEMENTATION** at the moment the plan is to implement a template
  that all the enums have access to, thus reducing the need to reproduce code for all the different error
  message().
*/
namespace torrent
{
  template <enum T>
  class error_template : public std::error_category
  {
    static_assert (static_cast<int> (T::generic) > 0, "first error enum must be generic = 1");

    public:
      char const * name () const noexcept override
      {
        return T;
      }

    std::string message (int ev) const override;
  };
}

namespace torrent
{
/** adapter for std::error_code, std::exception and bool flags to facilitate unified error handling */
class error
{
public:
	error () = default;
	error (torrent::error const & error_a) = default;
	error (torrent::error && error_a) = default;

	error (std::error_code code_arg);
	error (std::string message_arg);
	error (std::exception const & exception_arg);
	error & operator= (torrent::error const & err_arg);
	error & operator= (torrent::error && err_arg);
	error & operator= (std::error_code code_arg);
	error & operator= (std::string message_arg);
	error & operator= (std::exception const & exception_arg);
	bool operator== (std::error_code code_arg) const;
	error & then (std::function<torrent::error &()> next);

	explicit operator std::error_code () const;
	explicit operator bool () const;
	explicit operator std::string () const;
	std::string get_message () const;

	int error_code_as_int () const;
	error & on_error (std::string message_arg);
	error & on_error (std::error_code code_arg, std::string message_arg);
	error & set (std::string message_arg, std::error_code code_arg = nano::common_errors::generic);
	error & set_message (std::string message_arg);
	error & clear ();

private:
	std::error_code code;
	std::string message;
};
}