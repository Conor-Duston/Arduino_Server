import requests
import io

from os import path

#Test get function to make sure that files are returned from arduino correctly
def test_get():
    res = requests.get('http://10.0.0.2/')
    
    abs_path : str = path.dirname(__file__)
    rel_path : str = "../Webpage/index.html"
    full_path : str = path.join(abs_path, rel_path)

    print("Webpage Text:")
    print(res.text)
    webpage_text : str = res.text
    
    #Remove return line, it is placed into web response
    webpage_text = webpage_text.replace('\r', '')

    with io.open(full_path, "r", encoding='utf8') as file:
        
        disk_server = file.read()
        print("Disk text:")
        print(disk_server)
        assert disk_server == webpage_text