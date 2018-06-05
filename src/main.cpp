#include <iostream>
#include <sstream>
#include <iomanip>

#include "download.hpp"
#include "string_processing.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Enter url(s) to the video(s) you wish to download, please." << std::endl;
        std::exit(1);
    }

    std::cout << "#######################################################" << std::endl;
    std::cout << "################# YOUTUBE LOADER v0.3 #################" << std::endl;
    std::cout << "#######################################################" << std::endl;
    std::cout << "--> " << argc - 1 << " url(s) entered." << std::endl;

    // Almost all interface functions return a CURLcode (curl error code).
    // CURL_GLOBAL_ALL --> Initializes everything needed.
    CURLcode curl_code = curl_global_init(CURL_GLOBAL_ALL);

    if (curl_code != CURLE_OK)
    {
        std::cout << curl_easy_strerror(curl_code) << std::endl;
        std::exit(1);
    }
    // Starts a libcurl easy session.
    CURL* curl = curl_easy_init();

    // Iterates through the entered urls.
    for (int i = 1; i < argc; i++)
    {
        std::string curr_url = argv[i];
        // TODO: RegEx to check the url's validity.
        std::string response = "";
        // Pointer to the response-string.
        std::string* response_ptr = &response;
        // Receives the current url's http response and stores it in the response-string.
        receive_url_response(curl, curr_url.c_str(), response_ptr);
        std::string title = extract_title(response);
        std::string url = extract_url(response);

        // curl_easy_escape --> URL encodes the given string.
        std::string escaped_title = (std::string)curl_easy_escape(
            curl, title.c_str(), title.length()
        );
        // curl_easy_unescape --> URL decodes the given string.
        std::string unescaped_url = (std::string)curl_easy_unescape(
            curl, url.c_str(), url.size(), NULL
        );
        std::string url_to_download_from = unescaped_url + "title=" + escaped_title;

        // If the video requires a signature, we aren't supposed to download it.
        if ((signed)url_to_download_from.find("&s=") != -1)
        {
            std::cout << "To download this video a signature is required --> skipping url" << std::endl;
            continue;
        }
        std::string extension = extract_format(url_to_download_from);
        title = unescape_unicode(title);
        download_video(url_to_download_from.c_str(), title, extension, curl);
    }
    curl_easy_cleanup(curl);
    std::cout << "Would you like to convert the downloaded file to mp3? (y/n)" << std::endl;
    char buffer[1024];
    std::cin.getline(buffer, 1024);
    // The string class' constructor takes a NULL-terminated C-string.
    std::string mp3(buffer);

    if (mp3 == "y" || mp3 == "Y")
    {
        std::cout << "Converting to mp3..." << std::endl;
        // Calls the bash script that performs the conversion (using ffmpeg).
        system("../src/mp4_to_mp3.sh");
    }
    std::cout << "Done!" << std::endl;
    return 0;
}
