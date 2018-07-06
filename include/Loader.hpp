#define MEGABYTE 1048576;
/* Unicode char representing a filled "block". */
#define BLOCK "\u2588\u2588"
#define POINTS "...................."

namespace youtube_loader
{

class Loader
{

public:
    Loader(CurlHandler* handler);
    ~Loader();

    bool is_existing(std::string filename);
    void check_output_dir();
    void download_video(const char* url, std::string filename, std::string extension, CURL* curl);

private:
    CurlHandler* handler;
};

static int download_progress(
    void* clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow
);

} // namespace youtube_loader
