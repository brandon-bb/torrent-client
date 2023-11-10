#include <filesystem>
#include <string>

namespace TorrentLib {
class TorrentIO final {
  public:
    TorrentIO () : filepath ("");
    TorrentIO (const std::filesystem::path& filepath);
    TorrentIO (const std::string& filepath);

    const std::string ExtractContent () const;
    const std::string CurrentPath () const;

  private:
    std::filesystem::path filepath;
    std::string contents;

    //verifies if file ends with .torrent
    const bool TorrentExtension (const std::filesystem::path& filepath) const;
    std::string RetrieveAbsolute ();
    
};
}

