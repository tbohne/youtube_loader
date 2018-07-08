#define MEBIBYTE 1048576;
// Unicode chars representing a filled block.
#define BLOCK "\u2588\u2588"
#define POINTS "...................."

namespace youtube_loader
{

class Loader
{

public:
    /**
     * @brief Creates an instance of the loader.
     * @param CurlHandler* handler - pointer to the curl handler
     */
    Loader(CurlHandler* handler);
    ~Loader();

    /**
     * @brief Checks whether the given filename refers to an existing file.
     * @param string filename - the filename to be checked
     * @return bool - true if it exists
     */
    bool is_existing(std::string filename);

    /**
     * @brief Checks whether the output dir exists and creates it if not.
     */
    void check_output_dir();

    /**
     * @brief Downloads and saves the video. It checks for duplicate downloads and
     *        renames them if necessary. (The originals aren't overwritten - incremented index).
     * @param const char* url - the url to download from
     * @param string filename - the name of the file to save the video in
     * @param string extension - the file's extension (usually mp4)
     * @param CURL* curl - the curl instance
     */
    void download_video(const char* url, std::string filename, std::string extension, CURL* curl);

private:
    // Member variable that stores a pointer to the curl handler.
    CurlHandler* handler;
};

/**
 * @brief Callback function that visualizes the current download progress.
 * @param void* clientp - progress callback pointer
 * @param curl_off_t dltotal - the total size of the download
 * @param curl_off_t dlnow - the size downloaded so far
 * @param curl_off_t ultotal - the total size of the upload
 * @param curl_off_t ulnow - the size uploaded so far
 * @return int error code - 0 means everything worked as expected
 */
static int download_progress(
    void* clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow
);

} // namespace youtube_loader
