#include <thread>
#include <mutex>
#include <inotify-cxx/Inotify.h>

namespace TorrentLib {


// https://en.cppreference.com/w/cpp/iterator/iterator_traits
// checks if the given type is actually an iterator
template <typename T, typename = void>
struct is_iterator : std::false_type {};

template <typename T>
struct is_iterator<T, std::void_t<
    typename std::iterator_traits<T>::value_type,
    typename std::iterator_traits<T>::pointer,
    typename std::iterator_traits<T>::reference,
    typename std::iterator_traits<T>::iterator_category,
    typename std::iterator_traits<T>::difference_type
>> : std::true_type {};

template <typename T>
class DecodeManager final 
{
private:
  //wrapper class for decoder.hpp
  template <typename IterType, typename = std::enable_if_t<is_iterator<IterType>::value>>
  class DecodeWrapper final
  {
    public:
      DecodeWrapper (T& encoded_value, IterType& decode_iterator) : 
      resource(encoded_value), iterator (decode_iterator) {}

      typename T::value_type CurrentValue() const {
        return *iterator;
      }

      void AdvanceIterator() {
        iterator++;
      }

    private:
      T& resource;
      IterType& iterator;
  };


public:
    DecodeManager (T it) : iterator_(it) {}
    ~DecodeManager() = default;

    typename T::value_type CurrentValue() const;



  private:
    T& resource;
    IterType iterator;
}



template <typename T>
class Monitor {
  public:
    Monitor (T& resource)

  private:
    T& resource_;
    std::function<void()> callback_;
    std::thread thread_;
    std::mutex mutex_;
};






}