#include <thread>
#include <mutex>
#include <inotify-cxx/Inotify.h>

namespace TorrentLib {


// https://en.cppreference.com/w/cpp/iterator/iterator_traits
template <typename T>
struct is_iterator<T, std::void_t<
    typename std::iterator_traits<T>::value_type,
    typename std::iterator_traits<T>::pointer,
    typename std::iterator_traits<T>::reference,
    typename std::iterator_traits<T>::iterator_category,
    typename std::iterator_traits<T>::difference_type
>> : std::true_type {};



template <typename T>
class Monitor {
  public:
    Monitor (T& resource)

  private:
    T& resource_;
    ResourceType type_;
    std::function<void()> callback_;
    std::thread thread_;
    std::mutex mutex_;
};


}