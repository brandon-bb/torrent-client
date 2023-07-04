#include <iostream>
#include <string>
#include <functional>
#include <system_error>
#include <utility>
#include <type_traits>

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
  inconsistent_string_length,
};

enum tracker_errors
{
  generic = 1
};

}

/*a template that all the enums have access to, thus reducing the need to reproduce code for all the different error
  message().
*/
namespace torrent
{
  template <typename T>
  class error_template : public std::error_category
  {
    static_assert (std::is_enum<T>::value, "Must be enum");
    static_assert (static_cast<int> (T::generic) == 1, "first error enum must be generic = 1");

    public:
      T err;

    public:
      error_template (T e) : err (e) {}

      char const * name () const noexcept override
      {
        return err;
      }

      std::string message (int ev) const override
      {
        return err.message (ev);
      }
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
	error & set (std::string message_arg, std::error_code code_arg = torrent::common_errors::generic);
	error & set_message (std::string message_arg);
	error & clear ();

private:
	std::error_code code;
	std::string message;
};
}