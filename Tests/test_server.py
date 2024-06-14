import requests
import io

from os import path

ARDUINO_URL : str = 'http://10.0.0.2/'

#Test get function to make sure that files are returned from arduino correctly
def test_get():
    #Get the bytes from the home page on arduino server
    res = requests.get(ARDUINO_URL)
    webpage_bytes : bytes = res.content

    #Get pathway to same page in storage
    abs_path : str = path.dirname(__file__)
    rel_path : str = "../Webpage/index.html"
    full_path : str = path.join(abs_path, rel_path)

    #Open and read file
    with io.open(full_path, "rb") as file:
        disk_bytes : bytes = file.read()
        
        #Compare bytes between what we have and what we got in storage
        assert disk_bytes == webpage_bytes, "Bytes received are not equivalent to bytes on disk"

# Test to make sure that correct error information is returned from files that do not exists
def test_get_nonexistant_file():
    # Do a GET request on a file that does not exist
    res = requests.get(ARDUINO_URL + 'nonexistant_file')

    #Check status code
    status : int = res.status_code

    #Check for correct status code, 404
    assert status == 404, "Error code other than ERROR 404 returned"

    #Could be expanded further for error code webpage on request

# Connect is for making tunnels between this device and a remote location, and should not be supported by arduino
def test_connect_unsupported():
    #Configure server to try and use CONNECT header
    res = requests.request('CONNECT', ARDUINO_URL)

    #500 - generic server response code for server side issue
    assert res.status_code == 500, "Response code is not 500, Server error (generic)"

    assert "Unsuported action" in res.text, "Reason for code is not given"

#Tests server to make sure that unkown HTTP verbs get appropraite response
def test_unknown_verb():
    #Use unkown verb with same verb length as other actions
    res = requests.request('PET',  ARDUINO_URL)
    #Use unkown verb of unkown verb length
    res2 = requests.request('INVALIDVERB', ARDUINO_URL)

    assert res.status_code == 500, "Invalid HTTP Verb of length 3 (like GET, PUT) has been accapted as valid verb"
    assert res2.status_code == 500, "HTTP Verb with unique length  "

'''
    TESTS BELOW THIS LINE ARE NOT YET WORKING OR ARE NOT YET IMPLEMENTED BY SERVER
'''
#Test to make sure that put works to create resource
def test_put():
    return
    res = requests.put(ARDUINO_URL + 'test_file')
    

#Test to make sure that delete works on removing resource
def test_delete():
    return
    res = requests.delete(ARDUINO_URL + 'test_file')



#Test function to make sure supported mime types are correctly returned.
def test_mime_type():
    return