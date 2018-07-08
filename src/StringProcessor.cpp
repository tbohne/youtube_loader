#include "../include/StringProcessor.hpp"

namespace youtube_loader
{

StringProcessor::StringProcessor() {}
StringProcessor::~StringProcessor() {}

std::string StringProcessor::to_utf8(uint32_t cp)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes((char32_t)cp);
}

std::string StringProcessor::unescape_unicode(std::string str)
{
    int start_idx = 0;

    while ((start_idx = str.find("\\u", start_idx)) != -1)
    {
        int end_idx = str.find_first_not_of("0123456789abcdefABCDEF", start_idx + 2);

        std::string tmp = str.substr(start_idx + 2, end_idx - (start_idx + 2));
        std::istringstream iss(tmp);

        uint32_t cp;
        if (iss >> std::hex >> cp)
        {
            std::string utf8 = to_utf8(cp);
            str.replace(start_idx, 2 + tmp.length(), utf8);
            start_idx += utf8.length();
        }
        else
            start_idx += 2;
    }
    return str;
}

std::string StringProcessor::extract_format(std::string url)
{
    std::string str = "type=video/";
    int start_idx = url.find(str) + str.length();
    int end_idx = url.find(";", start_idx);
    std::string extension = url.substr(start_idx, end_idx - start_idx);

    // If the format isn't found, the default is mp4.
    if (extension.length() == 0)
    {
        std::cout << "No video format found - set to mp4.";
        return "mp4";
    }
    return extension;
}

std::string StringProcessor::extract_title(std::string response)
{
    // The escaped quotes and the colon is used to get the exact length.
    std::string str = "\"title\":\"";
    int start_idx = response.find(str) + str.length();
    int end_idx = response.find("\",", start_idx);
    return response.substr(start_idx, end_idx - start_idx);
}

std::string StringProcessor::remove_itag_params(std::string url)
{
    int itag_idx = url.find("itag=");
    // The part between "itag=" and "&quality=" should be erased.
    return url.erase(itag_idx, url.find("&", itag_idx) - itag_idx);
}

std::string StringProcessor::extract_url(std::string response)
{
    if ((signed)response.find("url_encoded_fmt_stream_map") == -1)
    {
        std::cout << "URL could not be extracted - missing url_encoded_fmt_stream_map!" << std::endl;
        std::exit(1);
    }
    // Using the escaped quotes and the colon to get the exact position.
    std::string str = "\"url_encoded_fmt_stream_map\":";

    // The member "url_encoded_fmt_stream_map" contains the relevantt information,
    // it starts right after str.
    int start_idx = response.find(str) + str.length() + 1;
    int url_start_idx = response.find("http", start_idx);

    // Everything before the starting idx of the url: before "url="
    // needs to be appended to the url (some kind of additional data which seems
    // to occur in random order from time to time).
    std::string append_to_url = "";

    // "url=".length() = 4 --> the 4 chars before the "http" need to be considered.
    int url_meta_length = 4;
    // There is some additional data between the "url_encoded_fmt_stream_map:"
    // and the "http" - this data needs to be appended to the url separated by "&".
    if (start_idx != url_start_idx - url_meta_length)
    {
        append_to_url += "&";
        append_to_url += response.substr(start_idx, url_start_idx - start_idx - url_meta_length);
    }
    // The end of the first url (the one with the best quality available) is indicated by ",".
    int end_idx = response.find(",", url_start_idx);
    std::string url = response.substr(url_start_idx, end_idx - url_start_idx);

    // If there is additional data to be added, it is done here (otherwise it adds an empty string).
    url += append_to_url;
    url = unescape_unicode(url);
    url = remove_itag_params(url);

    return url;
}

} // namespace youtube_loader
