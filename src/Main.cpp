#include <curl/curl.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "CurlHandler.cpp"
#include "Loader.cpp"
#include "StringProcessor.cpp"

using namespace youtube_loader;

void print_header(int urls)
{
    std::cout << "#################################################################" << std::endl;
    std::cout << "###################### YOUTUBE LOADER v0.3 ######################" << std::endl;
    std::cout << "#################################################################" << std::endl;
    std::cout << "--> " << urls << " url(s) entered." << std::endl;
}

void convert_to_mp3()
{
    std::cout << "Would you like to convert the downloaded file to mp3? (y/n)" << std::endl;
    std::string mp3;
    std::getline(std::cin, mp3);

    if (mp3 == "y" || mp3 == "Y")
    {
        std::cout << "Converting to mp3..." << std::endl;
        // Calls the bash script that performs the conversion (using ffmpeg).
        system("../src/mp4_to_mp3.sh");
    }
}

void init_curl()
{
    // Almost all interface functions return a CURLcode (curl error code).
    // CURL_GLOBAL_ALL --> Initializes everything needed.
    CURLcode curl_code = curl_global_init(CURL_GLOBAL_ALL);
    if (curl_code != CURLE_OK)
    {
        std::cout << curl_easy_strerror(curl_code) << std::endl;
        std::exit(1);
    }
}

std::string get_response(CURL* curl, std::string curr_url, CurlHandler* handler)
{
    std::string response = "";
    std::string* response_ptr = &response;
    // Receives the current url's http response and stores it in the response-string.
    handler->receive_url_response(curl, curr_url.c_str(), response_ptr);

    return response;
}

std::string construct_url_to_download_from(
    CURL* curl,
    StringProcessor* processor,
    std::string response,
    std::string title
)
{
    std::string url = processor->extract_url(response);
    // curl_easy_escape --> URL encodes the given string.
    std::string escaped_title = (std::string)curl_easy_escape(
        curl, title.c_str(), title.length()
    );
    // curl_easy_unescape --> URL decodes the given string.
    std::string unescaped_url = (std::string)curl_easy_unescape(
        curl, url.c_str(), url.size(), NULL
    );
    std::string url_to_download_from = unescaped_url + "title=" + escaped_title;

    return url_to_download_from;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Enter url to the video you wish to download, please." << std::endl;
        std::exit(1);
    }
    print_header(argc - 1);
    init_curl();

    // Starts a libcurl easy session.
    CURL* curl = curl_easy_init();
    StringProcessor* processor = new StringProcessor();
    CurlHandler* handler = new CurlHandler();
    Loader* loader = new Loader(handler);

    std::string response = get_response(curl, argv[1], handler);
    std::string title = processor->extract_title(response);
    std::string url_to_download_from = construct_url_to_download_from(
        curl, processor, response, title
    );

    // If the video requires a signature, we aren't supposed to download it.
    if ((signed)url_to_download_from.find("&s=") != -1)
    {
        std::cout << "To download this video a signature is required - aborted." << std::endl;
    }
    else
    {
        std::string extension = processor->extract_format(url_to_download_from);
        title = processor->unescape_unicode(title);
        loader->download_video(url_to_download_from.c_str(), title, extension, curl);
        convert_to_mp3();
    }
    curl_easy_cleanup(curl);

    std::cout << "Done!" << std::endl;
    return 0;
}
