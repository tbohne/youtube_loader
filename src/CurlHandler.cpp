#include "../include/CurlHandler.hpp"

namespace youtube_loader
{

CurlHandler::CurlHandler() {}
CurlHandler::~CurlHandler() {}

static size_t write_callback(char* buffer, size_t size, size_t nmemb, std::string* response)
{
    *response += buffer;
    return size * nmemb;
}

void CurlHandler::write_file(
    CURL* curl,
    const char* url,
    FILE* file,
    std::string filename,
    int (*download_progress)(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
)
{
    // Resets all options of a libcurl session handle.
    curl_easy_reset(curl);

    // https://curl.haxx.se/libcurl/c/progressfunc.html
    if (curl)
    {
        /************** Sets options for a curl easy handle *******************/
        // Sets the url to work with.
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // Sets the data to be written to the file.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        // Using the download_progress function:
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, download_progress);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        /**********************************************************************/

        std::cout << "Writing file: " << filename << "\n" << std::endl;
        CURLcode curl_code = curl_easy_perform(curl);

        if (curl_code != CURLE_OK)
        {
            std::cout << "libcurl error: " << curl_easy_strerror(curl_code) << std::endl;
            remove(filename.c_str());
            std::exit(1);
        }
        fclose(file);
    }
    else
    {
        std::cout << "libcurl error - couldn't start download." << std::endl;
        remove(filename.c_str());
        std::exit(1);
    }
}

void CurlHandler::receive_url_response(CURL* curl, const char* url, std::string* response)
{
    // Resets all options of a libcurl session handle.
    curl_easy_reset(curl);

    if (curl)
    {
        /************** Sets options for a curl easy handle *******************/
        // Sets the url to work with.
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // Callback for writing the data.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
        /**********************************************************************/

        // Performs a blocking file transfer and returns a curl error code.
        CURLcode curl_code = curl_easy_perform(curl);

        if (curl_code != CURLE_OK)
        {
            std::cout << "libcurl error: " << curl_easy_strerror(curl_code) << std::endl;
            std::exit(1);
        }
    }
    else
    {
        std::cout << "Problem occured while trying to download from: " << url << std::endl;
        std::exit(1);
    }
}

} // namespace youtube_loader
