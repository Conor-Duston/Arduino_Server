import requests

from os import path


def test_get():
    res = requests.get('http://10.0.0.2/')
    
    abs_path : str = path.dirname(__file__)
    rel_path : str = "../Webpage/index.html"
    full_path : str = path.join(abs_path, rel_path)

    print("Webpage Text:")
    print(res.text)

    with open(full_path, "r") as file:
        
        disk_server = file.read()
        print("Disk text:")
        print(disk_server)
        assert disk_server == res.text