#!/bin/bash

DIR=../output/*

echo "********************************"
echo "***** MP4 to MP3 Converter *****"
echo "********************************"

# >> /dev/null redirects standard output (stdout) to /dev/null, which discards it.
if which ffmpeg >> /dev/null;
  then
    # Get only the first line from the output of ffmpeg -version
    # "head & tail solution":
    echo $(ffmpeg -version | head -n1)
  else
    echo "You have to install ffmpeg on your system to run this script."
    exit
  fi

for FILE in $DIR
do
  # https://superuser.com/questions/439771/command-line-tool-to-identify-audio-file-specs/439812#439812
  FORMAT=$(ffprobe "$FILE" -show_format 2> /dev/null | awk -F "=" '$1 == "format_name" {print $2}')

  # The format is copied from a downloaded youtube clip by using
  # "ffprobe -show_format sqrt2.mp4" - I'm not sure if this works for all of
  # the downloaded videos - will check it later.
  if [ "$FORMAT" == "mov,mp4,m4a,3gp,3g2,mj2" ];
    then
      echo "$FILE ---- supported file format --- starting conversion"

      # Remove last n (here 4) characters from a string in bash
      # --> removes the file extension (.mp4)
      RENAME=${FILE::-4}

      # https://askubuntu.com/questions/84584/converting-mp4-to-mp3
      # --> for the parameters etc.
      # How can I make ffmpeg be quieter/less verbose?
      # ffmpeg -loglevel panic [rest of your ffmpeg stuff]
      # --> should make it so only serious errors are logged, in theory
      ffmpeg -loglevel panic -i "$FILE" -vn -acodec libmp3lame -ac 2 -ab 160k -ar 48000 "$RENAME.mp3"

    else
      echo "bypassed $FILE $FORMAT --- unsupported file format"
    fi
done

echo "Finished Conversion!"
