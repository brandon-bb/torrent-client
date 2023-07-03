#include <errors.hpp>

std::string torrent::common_errors::message (int ev) const
{
  switch (static_cast<torrent::common_errors> (ev))
  {
    case torrent::common_errors::generic:
      return "Unknown error";

    case torrent::common_errors::file_not_exist:
      return "File does not exist";

    case torrent::common_errors::not_torrent_file:
      return "Not a torrent file";
  }
}

std::string torrent::bencode_errors::message (int ev) const
{
  switch (static_cast<torrent::bencode_errors> (ev))
  {
    case torrent::bencode_errors::generic:
      return "Unknown error";

    case torrent::bencode_errors::invalid_bencoding:
      return "File has invalid bencode format";

    case torrent::bencode_errors::invalid_utf8:
      return "File not encoded in UTF-8";

    case torrent::bencode_errors::leading_zero:
      return "Integer has leading zero";

    case torrent::bencode_errors::no_integer_value:
      return "No integer value encoded";

    case torrent::bencode_errors::non_numeric:
      return "Non-numeric character present in integer encoding";

    case torrent::bencode_errors::invalid_string_length:
      return "Given string does not match the given length";
  }
}

torrent::error::error (std::error_code code_arg)
{
  code = code_arg;
}

torrent::error::error (std::string message_arg)
{
  //code = torrent::common_errors::generic;
  message = std::move (message_arg);
}

torrent::error::error (std::exception const & exception_arg)
{
  //code = torrent::common_errors::generic;
  message = exception_arg.what ();
}

torrent::error & torrent::error::operator= (torrent::error const & err_arg)
{
  code = err_arg.code;
  message = err_arg.message;

  return *this;
}

torrent::error & torrent::error::operator= (torrent::error && err_arg)
{
  code = err_arg.code;
  message = std::move(err_arg.message);

  return *this;
}

/* assign error code */
torrent::error & torrent::error::operator= (std::error_code code_arg)
{
  code = code_arg;
  message.clear ();

  return *this;
}

/* set error to common error and the message to the given arg*/
torrent::error & torrent::error::operator= (std::string message_arg)
{
  //code = torrent::common::generic;
  message = std::move (message);

  return *this;
}

/** set error to common::generic and adopt exception message **/
torrent::error & torrent::error::operator= (std::exception const & exception_arg)
{
  //code = torrent::common::generic;
  message = exception_arg.what ();

  return *this;
}

/** return true if this error code == parameter **/
bool torrent::error::operator== (std::error_code code_arg) const
{
  return code == code_arg;
}

/** call function if current error is zero **/
torrent::error & torrent::error::then (std::function<torrent::error &()> next)
{
  code ? *this : next ();
}

torrent::error::operator std::error_code () const
{
  return code;
}

torrent::error::operator bool () const
{
  return code.value () != 0;
}

int torrent::error::errcode_as_int () const 
{
  return code.value ();
}

torrent::error::operator std::string () const
{
  return get_message ();
}

std::string torrent::error::get_message () const
{
  std::string res = message;

  if (code && res.empty ())
  {
    res = code.message ();
  }

  return res;
}

torrent::error & torrent::error::on_error (std::string message_arg)
{
  if (code)
  {
    message = std::move (message_arg);
  }

  return *this;
}

torrent::error & torrent::error::on_error (std::error_code code_arg, std::string message_arg)
{
  if (code == code_arg)
  {
    message = std::move (message_arg);
  }

  return *this;
}

torrent::error & torrent::error::set (std::string message_arg, std::error_code code_arg = nano::common::generic)
{
  code = code_arg;
  message = std::move (message_arg);

  return *this;
}

torrent::error & torrent::error::set_message (std::string message_arg)
{
  if (!code)
  {
    //code = nano::common::generic;  
  }

  message = std::move (message_arg);

  return *this;
}

torrent::error & torrent::error::clear ()
{
  code.clear ();
  message.clear ();

  return *this;
}

