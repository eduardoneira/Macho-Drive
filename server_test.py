
#ver docs.python-requests.org/en/latest/user/quickstart/

#funca con python 2.7, creo que en 3.0 cambiaron el print (por si a alguien no le anda)

import requests
r = requests.get("http://localhost:8000/datos")

print "headers:", r.headers.keys()
print "headers content:", r.headers.values()
print "content:", r.content
