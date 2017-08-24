#include <curl/curl.h>
#include <fstream>

#include "curl.hpp"

#define MEGABYTE 1048576;
/* Special unicode char representing a filled "block". */
#define BLOCK "\u2588\u2588"
#define POINTS "....................";

/**
 * Callback function that visualizes the current download progress.
 */
int download_progress(
    void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow
)
{
    /* Prevents division by 0. */
    if (dltotal == 0) { return 0; }

    /* Stores the current download progress in percent. */
    float progress = ((float)dlnow / (float)dltotal) * 100.0;

    /* The string blocks represents the current download progress
       in blocks to visualize it with a download status bar. */
    std::string points = POINTS;
    std::string blocks = "";

    for (int i = 0; i < progress / 10; i++)
    {
        blocks += BLOCK;
        points.erase(0, 2);
    }

    /* dlnow stores the progress in bytes - convert to MB. */
    float dlnow_mb = (float)dlnow / (float)MEGABYTE;

    /* std::fixed        --> Modifies the default formatting for floating-point input / output.
       std::setprecision --> Sets the decimal precision to be used to format floating-point
                             values on output operations (2 digits)
       "\r"              --> Carriage Return - moves the cursor to the beginning of the line
                             without advancing to the next line
       I'm using the carriage return to be able to override the current line again and again
       to have a nice and clean visualization of the download progress without printing
       in a new line each iteration. */
    std::cout << "  " << blocks << points << "  " << std::fixed << std::setprecision(2)
    << progress << "% - " << std::fixed << std::setprecision(2) << dlnow_mb << "MB" << "\r";

    /* For output streams (and for update streams on which the last operation
       was output), writes any unwritten data from the stream's buffer to the
       associated output device.
       --> In this case it stops the cursor from "blinking" through the output line. */
    fflush(stdout);
    return 0;
}

/**
 * Checks whether the given filename refers to an existing file.
 */
bool is_existing(std::string filename)
{
    std::ifstream infile(filename);
    return infile.good();
}

/**
 * Downloads and saves the video.
 * It checks for duplicate downloads and renames them if necessary.
 * (The originals aren't overwritten - incremented index).
 */
void download_video(const char* url, std::string filename, std::string extension, CURL* curl)
{
    filename += "." + extension;
    /* The downloaded videos should be stored in the output folder. */
    filename = "../output/" + filename;

    /* Appends an index with the number of the copy of the same file to the filename. */
    if (is_existing(filename))
    {
        std::cout << "The file " << filename << " already exists." << std::endl;

        int idx = 1;
        std::string tmp_filename = filename;
        while (is_existing(tmp_filename))
        {
            tmp_filename = filename;
            tmp_filename += "(" + std::to_string(idx) + ")";
            idx++;
        }
        filename = tmp_filename;
    }

    FILE* file;
    file = fopen(filename.c_str(), "w");

    if (file == NULL)
    {
        std::cout << "Error while opening " << filename << std::endl;
        std::exit(1);
    }

    /* download_progess is a pointer to the function. */
    write_file(curl, url, file, filename, download_progress);

    std::cout << std::endl << "Done!" << std::endl;
}
