namespace youtube_loader
{

class CurlHandler
{

public:
    CurlHandler();
    ~CurlHandler();

    void write_file(CURL* curl, const char* url, FILE* file, std::string filename,
        int (*download_progress)(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t));
    void receive_url_response(CURL* curl, const char* url, std::string* response);

};

static size_t write_callback(char* buffer, size_t size, size_t nmemb, std::string* response);

} // namespace youtube_loader
