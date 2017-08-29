youtube_loader
=====================================================
CLI tool to download and convert videos from youtube.

### DEPENDENCIES
- **cmake** --> apt-get install cmake
- **libcurl** --> apt-get install libcurl4-openssl-dev
- **ffmpeg** --> apt-get install ffmpeg (if you want to be able to convert the videos to mp3)

### BUILD PROCESS
```
$ cd youtube_loader
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./main https://www.youtube.com/...
```

The downloaded videos and converted mp3s are stored in the output folder.
