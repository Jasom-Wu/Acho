import json

import django
import os
import requests

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'djangoProject.settings')
django.setup()

from appWelcome import models as user_model

from appAcho import models

ULR = "http://192.168.42.55:8787"
dev_tok = ""
if __name__ == "__main__":
    key = {"mac": "48:55:19:6A:63:D5", "ip": "192.168.43.248", "type": "2"}
    response = requests.get(ULR + "/home/device/init/", params=key)
    data = json.loads(response.text)
    if data['status'] == "successful":
        dev_tok = data["token"]
        key = {"tok": dev_tok}
        response = requests.get(ULR + "/home/device/getbindmsg/", params=key)
        data = json.loads(response.text)
        if data["status"] == "successful":
            print(data["data"])
