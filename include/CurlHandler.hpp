namespace youtube_loader
{

/**
 * @brief Handles the CURL operations.
 */
class CurlHandler
{

public:
    CurlHandler();
    ~CurlHandler();

    /**
     * @brief Writes the downloaded video to the file.
     * @param CURL* curl - the curl instance
     * @param const char* url - the url to work with
     * @param string filename - the name of the file to write to
     * @param int (*download_progress)(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
     *        - function pointer to download_progress
     */
    void write_file(
        CURL* curl,
        const char* url,
        FILE* file,
        std::string filename,
        int (*download_progress)(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
    );

    /**
     * @brief Receives the content of the response to the url.
     * @param CURL* curl - the curl instance
     * @param const char* url - the url to work with
     * @param string* response - pointer to the response string
     */
    void receive_url_response(CURL* curl, const char* url, std::string* response);
};

/**
 * @brief Writes the received data.
 *        This callback function gets called by libcurl as soon as there is data
 *        received that needs to be saved.
 *        The callback function will be passed as much data as possible in all
 *        invokes, but you must not make any assumptions. It may be one byte,
 *        it may be thousands. Your callback should return the number of bytes
 *        actually taken care of. If that amount differs from the amount passed
 *        to your callback function, it'll signal an error condition to the library.
 * @param char* buffer - the delivered data
 * @param size_t size - the size of one element
 * @param size_t nmemb - the number of elements
 * @param string* response - pointer to the response string
 * @return size_t - the number of bytes taken care of
 */
static size_t write_callback(char* buffer, size_t size, size_t nmemb, std::string* response);

} // namespace youtube_loader
