
#ver docs.python-requests.org/en/latest/user/quickstart/

#funca con python 2.7, creo que en 3.0 cambiaron el print (por si a alguien no le anda)

import requests
import json

r = requests.post("http://localhost:8000/users/", data=json.dumps({'username':'gabriel', 'password': 'gayoso'}))
print "POST", r.url
print "content:", r.content

print

r = requests.get("http://localhost:8000/users/gabriel", data=json.dumps({'username':'gabriel'}))
print "GET", r.url
print "content:", r.content

r = requests.post("http://localhost:8000/files/gabriel/", data=json.dumps({'content':'182769', 'filename':'test.txt', 'owner_username':'gabriel', 'tags':['txt', 'test']}))
print "POST", r.url
print "content:", r.content

r = requests.get("http://localhost:8000/files/gabriel/test.txt", data=json.dumps({'filename':'test.txt', 'owner_username':'gabriel'}))
print "GET", r.url
print "content:", r.content
