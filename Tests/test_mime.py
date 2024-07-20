import requests
import io

from os import path, listdir

from test_server import ARDUINO_URL

#Super types for MIME type lookups
audio_super : str = "audio/"
text_super : str = "text/"
image_super : str = "image/"
video_super : str = "video/"
application_super : str = "application/"
font_super : str = "font/"

#Mime type dictionary for looking up the mime type corrisponding to specific file extensions
mime_type_dictionary : dict = {
    "aac": audio_super + "aac",
    
    "css" : text_super + "css",

    "gz" : application_super + "gzip",
    "gif" : image_super + "gif",

    "html": text_super + "html",
    "htm": text_super + "html",

    "ico" : image_super + "vnd.microsoft.ico",

    "jpeg": image_super + "jpeg",
    "jpg" : image_super + "jpeg",
    "js" : text_super + "javascript",
    "json" : application_super + "json",

    "mjs" : text_super + "javascript",
    "mp3" : audio_super + "mpeg",
    "mpeg" : video_super + "mpeg",
    
    "png" : image_super + "png",

    "rtf" : application_super + "rtf",

    "svg" : image_super + "svg+xml",

    "txt" : text_super + "plain",

    "wav" : audio_super + "wav",
    "weba" : audio_super + "webm",
    "webm" : video_super + "webm",
    "webp" : image_super + "webp",

    "zip" : application_super + "zip",
    
    "7z" : application_super + "x-7z-compressed",
    
}

#Test mime type retrieval
def test_mime_and_file_retrevial():
    # Get pathway to same page in storage
    abs_path : str = path.dirname(__file__)
    print(abs_path)
    rel_path : str = ".\Test_Files"
    full_path : str = path.join(abs_path, rel_path)

    files : list[str] = [f for f in listdir(full_path) if path.isfile(path.join(full_path, f))]

    # print(files)

    for file in files:
        response = requests.get(ARDUINO_URL + file)
        
        file_extension : str = file.partition(".")[2]
        
        expected_MIME : str = mime_type_dictionary[file_extension]
        if expected_MIME == "":
            expected_MIME = application_super + "ocet-stream"
        
        actual_MIME : str = response.headers["Content-Type"]

        assert expected_MIME == actual_MIME, "Expected MIME: " + expected_MIME + "\n" + "Got MIME: " + actual_MIME + "\n File Extension" + file_extension

        with io.open(path.join(full_path, file), "rb") as stored_file:
            expected_bytes : bytes = stored_file.read()

            assert expected_bytes == response.content, "Received different or corupt file for " + file
