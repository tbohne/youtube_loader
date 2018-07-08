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

    /**
     * @brief Converts the input to utf-8.
     * @param uint32_t cp - the data to be converted
     * @return string - the utf-8 string
     */
    std::string to_utf8(uint32_t cp);

    /**
     * @brief Converts escaped unicode strings into utf8 string.
     * @param string str - escaped unicode string
     * @return string - the utf-8 string
     */
    std::string unescape_unicode(std::string str);

    /**
     * @brief Extracts the format of the video.
     * @param string url - the video's url
     * @return string - the video's format
     */
    std::string extract_format(std::string url);

    /**
     * @brief Extracts the title of the video.
     * @param string response - the http response the title gets extracted from
     * @return string - the video's title
     */
    std::string extract_title(std::string response);

    /**
     * @brief Removes the unused itag parameters from the url.
     *        itag is an undocumented parameter used internally by YouTube to
     *        differentiate between quality profiles. The remaining "itags" need to be removed
     *        since they aren't used (I only use the first one which seems to have the best quality).
     * @param string url - the video's url
     * @return string - the remaining parts of the url
     */
    std::string remove_itag_params(std::string url);

    /**
     * @brief Extracts the video's url from the content of the url response.
     *        The first url seems to be the one with the best quality available.
     * @param string response - the http response
     * @return string the actual url to download from
     */
    std::string extract_url(std::string response);
};

} // namespace youtube_loader
