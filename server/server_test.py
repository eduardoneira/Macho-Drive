
#ver docs.python-requests.org/en/latest/user/quickstart/

#funca con python 2.7, creo que en 3.0 cambiaron el print (por si a alguien no le anda)

import requests
import json

print "creo usuario"
r = requests.post("http://localhost:8000/users/", data=json.dumps({'username':'gabriel', 'password': 'gayoso'}))
print "POST", r.url
print "content:", r.content
print

print "hago log in"
r = requests.post("http://localhost:8000/sessions/", data=json.dumps({'username':'gabriel', 'password':'gayoso'}))
print "POST", r.url
print "content:", r.content
content_temp = json.loads(r.content)
token = str(content_temp["conn_token"])
print "token:", token
print

print "veo usuario"
r = requests.get("http://localhost:8000/users/gabriel", data=json.dumps({'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "subo archivo de ese usuario"
r = requests.post("http://localhost:8000/files/gabriel/", data=json.dumps({'content':'182769', 'filename':'test.txt', 'username':'gabriel', 'tags':['txt', 'test'], 'conn_token':token}))
print "POST", r.url
print "content:", r.content
print

print "veo ese archivo"
r = requests.get("http://localhost:8000/files/gabriel/test.txt", data=json.dumps({'filename':'test.txt', 'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "veo el usuario de nuevo"
r = requests.get("http://localhost:8000/users/gabriel", data=json.dumps({'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "modifico el archivo (cambio contenido y agrego un tag)"
r = requests.put("http://localhost:8000/files/gabriel/test.txt", data=json.dumps({'filename':'test.txt', 'username':'gabriel', 'content_change':'999', 'tags_add':['modif'], 'tags_delete':['test'], 'user_who_modified':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "veo ese archivo"
r = requests.get("http://localhost:8000/files/gabriel/test.txt", data=json.dumps({'filename':'test.txt', 'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "borro ese archivo"
r = requests.delete("http://localhost:8000/files/gabriel/test.txt", data=json.dumps({'filename':'test.txt', 'username':'gabriel', 'conn_token':token}))
print "DELETE", r.url
print "content:", r.content
print

print "veo ese archivo"
r = requests.get("http://localhost:8000/files/gabriel/test.txt", data=json.dumps({'filename':'test.txt', 'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "veo el usuario de nuevo"
r = requests.get("http://localhost:8000/users/gabriel", data=json.dumps({'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print

print "borro el usuario 'gabriel'"
r = requests.delete("http://localhost:8000/users/gabriel", data=json.dumps({'username':'gabriel', 'conn_token':token}))
print "DELETE", r.url
print "content:", r.content
print

print "veo el usuario de nuevo"
r = requests.get("http://localhost:8000/users/gabriel", data=json.dumps({'username':'gabriel', 'conn_token':token}))
print "GET", r.url
print "content:", r.content
print
