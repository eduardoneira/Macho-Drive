
#ver docs.python-requests.org/en/latest/user/quickstart/

#funca con python 2.7, creo que en 3.0 cambiaron el print (por si a alguien no le anda)

import requests
import json
r = requests.get("http://localhost:8000/datos")

print "headers:", r.headers.keys()
print "headers content:", r.headers.values()
print "content:", r.content

r = requests.post("http://localhost:8000/casa")

print "headers:", r.headers.keys()
print "headers content:", r.headers.values()
print "content:", r.content

r = requests.post("http://localhost:8000/usuarios", data=json.dumps({'user':'gabriel', 'password': 'gayoso','handlerType': 0})) #0 es SignUp

print "headers:", r.headers.keys()
print "headers content:", r.headers.values()
print "content:", r.content

r = requests.post("http://localhost:8000/usuarios", data=json.dumps({'user':'gabriel', 'password': 'gayoso','handlerType': 1})) #1 es LogIn

print "headers:", r.headers.keys()
print "headers content:", r.headers.values()
print "content:", r.content