
#ver docs.python-requests.org/en/latest/user/quickstart/

#funca con python 2.7, creo que en 3.0 cambiaron el print (por si a alguien no le anda)

import requests
import json
import os

data = ""
path = os.getcwd()
path_files = path + '/files/'

def crear_usuario(nombre, password):
	data = json.dumps({'username':nombre, 'password': password})
	r = requests.post("http://localhost:8000/users/", data=data)
	print "POST", r.url, data
	print "content:", r.content
	print

def hacer_log_in(nombre, password):
	data = json.dumps({'username':nombre, 'password':password})
	r = requests.post("http://localhost:8000/sessions/", data=data)
	print "POST", r.url, data
	print "content:", r.content
	content_temp = json.loads(r.content)
	token = str(content_temp["conn_token"])
	print "token:", token
	print
	return token

def get_usuario(nombre, token):
	data = json.dumps({'username':nombre, 'conn_token':token})
	r = requests.get("http://localhost:8000/users/"+nombre, data=data)
	print "GET", r.url, data
	print "content:", r.content
	print

def log_out(nombre, token):
	data = json.dumps({'username':nombre, 'conn_token':token})
	r = requests.delete("http://localhost:8000/sessions/"+nombre, data=data)
	print "DELETE", r.url, data
	print "content:", r.content
	print

def modificar_perfil(nombre, token, email):
	data = json.dumps({'username':nombre, 'conn_token':token, 'email':email})
	r = requests.put("http://localhost:8000/users/"+nombre, data=data)
	print "PUT", r.url, data
	print "content:", r.content
	print

def subir_archivo(nombre, token, filename, tags, users_with_read_perm, users_with_write_perm, ubicacion):
	content = open(path_files+filename, mode='rb').read()

	data = json.dumps({'content':content, 'filename':filename, 'username':nombre, 'tags':tags, 'conn_token':token, 'ubicacion':ubicacion, 'users_with_read_permission':users_with_read_perm, 'users_with_write_permission':users_with_write_perm})
	r = requests.post("http://localhost:8000/files/"+nombre+"/", data=data)
	print "POST", r.url, data
	print "content:", r.content
	print

def get_file(nombre, token, owner, filename):
	data = json.dumps({'filename':filename, 'username':nombre, 'conn_token':token, 'owner_username':owner})
	r = requests.get("http://localhost:8000/files/"+nombre+"/"+filename, data=data)
	print "GET", r.url, data
	print "content:", r.content
	print

def modify_file(nombre, token, owner, filename, n_filename, tags_add, tags_delete, users_read_add, users_read_delete, users_write_add, users_write_delete, ubicacion):
	n_content = open(path_files+filename, mode='rb').read()

	data = json.dumps({'filename':filename, 'username':nombre, 'owner_username':owner, 'filename_change': n_filename, 'content_change':n_content, 'users_with_read_permission_add':users_read_add, 'users_with_read_permission_remove':users_read_delete, 'users_with_write_permission_add':users_write_add, 'users_with_write_permission_remove':users_write_delete,'tags_add':tags_add, 'tags_delete':tags_delete, 'conn_token':token, 'ubicacion':ubicacion})
	r = requests.put("http://localhost:8000/files/"+nombre+"/"+filename, data=data)
	print "PUT", r.url, data
	print "content:", r.content
	print

def delete_file(nombre, token, filename):
	data = json.dumps({'filename':filename, 'username':nombre, 'conn_token':token})
	r = requests.delete("http://localhost:8000/files/"+nombre+"/"+filename, data=data)
	print "DELETE", r.url, data
	print "content:", r.content
	print

def delete_user(nombre, token):
	data = json.dumps({'username':nombre, 'conn_token':token})
	r = requests.delete("http://localhost:8000/users/"+nombre, data=data)
	print "DELETE", r.url, data
	print "content:", r.content
	print

def borrar_si_existe_y_crear_archivo_fisico(filename, content):
	full_filename = path_files + filename
	
	f = open(full_filename,'w')
	f.write(content)
	f.close()

def modificar_archivo_fisico(filename, content):
	full_filename = path_files + filename
	f = open(full_filename,'w')
	f.write(content)
	f.close()