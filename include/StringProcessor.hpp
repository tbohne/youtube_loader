#include <iostream>
#include <string>
#include <codecvt>

namespace youtube_loader
{

class StringProcessor
{

public:
    StringProcessor();
    ~StringProcessor();

    std::string to_utf8(uint32_t cp);
    std::string unescape_unicode(std::string str);
    std::string extract_format(std::string url);
    std::string extract_title(std::string response);
    std::string remove_itag_params(std::string url);
    std::string extract_url(std::string response);
};

} // namespace youtube_loader
