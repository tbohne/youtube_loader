/**
 * Writes the received data.
 * This callback function gets called by libcurl as soon as there is data
 * received that needs to be saved. buffer points to the delivered data,
 * and the size of that data is size multiplied with nmemb.
 * The callback function will be passed as much data as possible in all
 * invokes, but you must not make any assumptions. It may be one byte,
 * it may be thousands. Your callback should return the number of bytes
 * actually taken care of. If that amount differs from the amount passed
 * to your callback function, it'll signal an error condition to the library.
 */
size_t write_callback(char* buffer, size_t size, size_t nmemb, std::string* response)
{
    *response += buffer;
    return size * nmemb;
}

/**
 * Writes to the file.
 * int (*download_progress)(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
 * return_type (*func_name)(parameter_types) --> function pointer to download_progress().
 */
void write_file(CURL* curl, const char* url, FILE* file, std::string filename,
    int (*download_progress)(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t))
{
    /* Resets all options of a libcurl session handle. */
    curl_easy_reset(curl);

    /* https://curl.haxx.se/libcurl/c/progressfunc.html */
    if (curl)
    {
        /************** Sets options for a curl easy handle *******************/
        /* Sets the url to work on. */
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* Sets the data to be written to the file. */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        /* Using the download_progress function:
           There are a couple of more options to set if necessary (works for now). */
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

/**
 * Receives the content of the response to the url currently working on.
 * http://www.cplusplus.com/forum/unices/45878/
 */
void receive_url_response(CURL* curl, const char* url, std::string* response)
{
    /* Resets all options of a libcurl session handle. */
    curl_easy_reset(curl);

    if (curl)
    {
        /************** Sets options for a curl easy handle *******************/
        /* Sets URL to work on */
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* Callback for writing data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
        /**********************************************************************/

        /* Performs a blocking file transfer.
           Returns a curl error code. */
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
