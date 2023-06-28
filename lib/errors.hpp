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

// Convenience macro to implement the standard boilerplate for using std::error_code with enums
// Use this at the end of any header defining one or more error code enums.
#define REGISTER_ERROR_CODES(namespace_name, enum_type)                                                        \
  namespace namespace_name                                                                                   \
	{                                                                                                          \
		static_assert (static_cast<int> (enum_type::generic) > 0, "The first error enum must be generic = 1"); \
		class enum_type##_messages : public std::error_category                                                \
		{                                                                                                      \
		public:                                                                                                \
			char const * name () const noexcept override                                                       \
			{                                                                                                  \
				return #enum_type;                                                                             \
			}                                                                                                  \
                                                                                                               \
			std::string message (int ev) const override;                                                       \
		};                                                                                                     \
                                                                                                               \
		inline std::error_category const & enum_type##_category ()                                             \
		{                                                                                                      \
			static enum_type##_messages instance;                                                              \
			return instance;                                                                                   \
		}                                                                                                      \
                                                                                                               \
		inline std::error_code make_error_code (::namespace_name::enum_type err)                               \
		{                                                                                                      \
			return { static_cast<int> (err), enum_type##_category () };                                        \
		}                                                                                                      \
	}                                                                                                          \
	namespace std                                                                                              \
	{                                                                                                          \
		template <>                                                                                            \
		struct is_error_code_enum<::namespace_name::enum_type> : std::true_type                                \
		{                                                                                                      \
		};                                                                                                     \
	}

REGISTER_ERROR_CODES (torrent, common_errors);
REGISTER_ERROR_CODES (torrent, bencode_errors);
REGISTER_ERROR_CODES (torrent, tracker_errors);

namespace torrent
{
/** Adapter for std::error_code, std::exception and bool flags to facilitate unified error handling */
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
	template <typename... ErrorCode>
	error & accept (ErrorCode... err)
	{
		// Convert variadic arguments to std::error_code
		auto codes = { std::error_code (err)... };
		if (std::any_of (codes.begin (), codes.end (), [this, &codes] (auto & code_arg) { return code == code_arg; }))
		{
			code.clear ();
		}

		return *this;
	}

	explicit operator std::error_code () const;
	explicit operator bool () const;
	explicit operator std::string () const;
	std::string get_message () const;
	/**
	 * The error code as an integer. Note that some error codes have platform dependent values.
	 * A return value of 0 signifies there is no error.
	 */
	int error_code_as_int () const;
	error & on_error (std::string message_arg);
	error & on_error (std::error_code code_arg, std::string message_arg);
	error & set (std::string message_arg, std::error_code code_arg = nano::error_common::generic);
	error & set_message (std::string message_arg);
	error & clear ();

private:
	std::error_code code;
	std::string message;
};

/**
 * A type that manages a nano::error.
 * The default return type is nano::error&, though shared_ptr<nano::error> is a good option in cases
 * where shared error state is desirable.
 */
template <typename RET_TYPE = torrent::error &>
class error_aware
{
	static_assert (std::is_same<RET_TYPE, nano::error &>::value || std::is_same<RET_TYPE, std::shared_ptr<nano::error>>::value, "Must be torrent::error& or shared_ptr<nano::error>");

public:
	/** Returns the error object managed by this object */
	virtual RET_TYPE get_error () = 0;
};
}