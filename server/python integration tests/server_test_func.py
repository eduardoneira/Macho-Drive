
#ver docs.python-requests.org/en/latest/user/quickstart/

#funca con python 2.7, creo que en 3.0 cambiaron el print (por si a alguien no le anda)

"""
Si se corre este archivo solo, se corren las unit tests definidas en el mismo con unittest de python. No hace falta haber abierto el server (pero tiene q estar el ejecutable en /build)
Para correr las pruebas viejas que imprimen en pantalla, correr en una consola el server, y en otra llamar a los otros archivos (no a este)
"""

import requests
import json
import os
import time
import subprocess
import base64
import random
import threading

data = ""
path = os.getcwd()
path_files = path + '/files/'
verbose = False

def create_user(username, password):
	data = json.dumps({'username':username, 'password': password})
	r = requests.post("http://localhost:8000/users/", data=data)
	if verbose:
		print "POST", r.url, data
		print "content:", r.content
		print
	else:
		return r

def log_in(username, password):
	data = json.dumps({'username':username, 'password':password})
	r = requests.post("http://localhost:8000/sessions/", data=data)
	if verbose:
		print "POST", r.url, data
		print "content:", r.content
		content_temp = json.loads(r.content)
		token = str(content_temp["conn_token"])
		print "token:", token
		print
		return token
	else:
		return r

def log_out(username, token):
	r = requests.delete("http://localhost:8000/sessions/"+username, headers={'conn_token' : token})
	if verbose:
		print "DELETE", r.url, data
		print "content:", r.content
		print
	else:
		return r

def delete_user(username, token):
	r = requests.delete("http://localhost:8000/users/"+username, headers={'conn_token' : token})
	if verbose:
		print "DELETE", r.url, data
		print "content:", r.content
		print
	else:
		return r

def get_user(username, token):
	r = requests.get("http://localhost:8000/users/"+username, headers={'conn_token' : token})
	if verbose:
		print "GET", r.url, data
		print "content:", r.content
		print
	else:
		return r

def modificar_perfil(username, token, email):
	data = json.dumps({'email':email})
	r = requests.put("http://localhost:8000/users/"+username, data=data, headers={'conn_token' : token})
	if verbose:
		print "PUT", r.url, data
		print "content:", r.content
		print
	else:
		return r

def subir_archivo(username, token, filename, tags, users_with_read_perm, users_with_write_perm, ubicacion):
	content = base64.b64encode(open(path_files+filename, mode='rb').read())

	data = json.dumps({'content':content, 'filename':filename, 'tags':tags, 'ubicacion':ubicacion, 'users_with_read_permission':users_with_read_perm, 'users_with_write_permission':users_with_write_perm})
	r = requests.post("http://localhost:8000/files/"+username+"/", data=data, headers={'conn_token' : token})
	data = json.loads(data)
	data["content"] = "SE IGNORA ESTO PARA QUE NO LLENE LA CONSOLA"
	data = json.dumps(data)
	if verbose:
		print "POST", r.url, data
		print "content:", r.content
		print
	else:
		return r

def get_file(username, token, filename):
	r = requests.get("http://localhost:8000/files/"+username+"/"+filename, headers={'conn_token' : token})

	json_respuesta = json.loads(r.content, strict=False);
	if "content" in json_respuesta.keys():
		contenido_arch = json_respuesta["content"][len(json_respuesta["content"])-1]
		borrar_si_existe_y_crear_imagen_fisico("devuelto_"+filename, contenido_arch)
		json_respuesta["content"] = "SE IGNORA ESTO PARA QUE NO LLENE LA CONSOLA"
	if verbose:
		print "GET", r.url, data
		print "content:", json.dumps(json_respuesta)
		print
	else:
		return r

def search_files(username, token, metadata, word):
	r = requests.get("http://localhost:8000/files/"+username+"/search/"+metadata+"/"+word, headers={'conn_token' : token})
	if verbose:
		print "GET", r.url, data
		print "content:", r.content
		print
	else:
		return r

def file_change_filename(username, token, owner, filename, n_filename):
	data = json.dumps({'owner_username':owner, 'filename_change': n_filename})
	r = requests.put("http://localhost:8000/files/"+username+"/"+filename, data=data, headers={'conn_token' : token})
	if verbose:
		print "PUT", r.url, data
		print "content:", r.content
		print
	else:
		return r

def file_change_content(username, token, owner, filename, ubicacion):
	n_content = base64.b64encode(open(path_files+filename, mode='rb').read())

	data = json.dumps({'owner_username':owner, 'content_change':n_content, 'ubicacion':ubicacion})
	r = requests.put("http://localhost:8000/files/"+username+"/"+filename, data=data, headers={'conn_token' : token})
	if verbose:
		print "PUT", r.url, data
		print "content:", r.content
		print
	else:
		return r

def file_change_tags(username, token, owner, filename, tags_add, tags_delete):
	data = json.dumps({'owner_username':owner, 'tags_add':tags_add, 'tags_delete':tags_delete})
	r = requests.put("http://localhost:8000/files/"+username+"/"+filename, data=data, headers={'conn_token' : token})
	if verbose:
		print "PUT", r.url, data
		print "content:", r.content
		print
	else:
		return r

def file_change_permissions(username, token, owner, filename, users_read_add, users_read_delete, users_write_add, users_write_delete):
	data = json.dumps({'owner_username':owner, 'users_with_read_permission_add':users_read_add, 'users_with_read_permission_remove':users_read_delete, 'users_with_write_permission_add':users_write_add, 'users_with_write_permission_remove':users_write_delete})
	r = requests.put("http://localhost:8000/files/"+username+"/"+filename, data=data, headers={'conn_token' : token})
	if verbose:
		print "PUT", r.url, data
		print "content:", r.content
		print
	else:
		return r

def file_delete_versions(username, token, owner, filename, versions_delete):
	data = json.dumps({'owner_username':owner, 'delete_versions':versions_delete})
	r = requests.put("http://localhost:8000/files/"+username+"/"+filename, data=data, headers={'conn_token' : token})
	if verbose:
		print "PUT", r.url, data
		print "content:", r.content
		print
	else:
		return r

def delete_file(username, token, filename):
	r = requests.delete("http://localhost:8000/files/"+username+"/"+filename, headers={'conn_token' : token})
	if verbose:
		print "DELETE", r.url, data
		print "content:", r.content
		print
	else:
		return r

def borrar_si_existe_y_crear_archivo_fisico(filename, content):
	full_filename = path_files + filename
	
	f = open(full_filename,'w')
	f.write(content)
	f.close()

def borrar_si_existe_y_crear_imagen_fisico(filename, content):
	full_filename = path_files + filename
	
	f = open(full_filename,'wb')
	f.write(base64.b64decode(content))
	f.close()

def modificar_archivo_fisico(filename, content):
	full_filename = path_files + filename
	f = open(full_filename,'w')
	f.write(content)
	f.close()

import unittest

class TestServerIntegration(unittest.TestCase):

	server = ""

	# aca se deberia poner que arrance el server en modo debug (o nivel de log de debug), especialmente para ver cuantos threads corren en paralelo
	def setUp(self):
		with open(os.devnull, 'w') as devnull:
			subprocess.Popen(args=["rm", "-rf", "/tmp/py_integration_tests/"], stdout=devnull)
			self.server = subprocess.Popen(args=[path+"/../build/Server",  "-Ddb_path,/tmp/py_integration_tests"], stdout=devnull)
			devnull.close()
		time.sleep(1) # esto es medio feo, pero si no corre los tests antes de que se haya abierto posta el server, y fallan pq no les responde

	def tearDown(self):
		self.server.terminate()
		with open(os.devnull, 'w') as devnull:
			subprocess.Popen(args=["rm", "-rf", "/tmp/py_integration_tests/"], stdout=devnull)
			devnull.close()

	def test_multithreading(self):
		global num_requests
		users = ["gabriel", "eduardo", "nicolas", "cristian"]
		tokens = []
		stop_events = []
		num_requests = 0

		def do_stuff(num, stop_event):
			global num_requests
			while not stop_event.is_set():
				index = random.randint(0, len(users)-1)
				if index < len(users) and index < len(tokens):
					r = get_user(users[index], tokens[index])
					self.assertTrue(r.status_code == requests.codes.ok)
					num_requests += 1

		i = 0
		for user in users:
			r = create_user(user, user)
			self.assertTrue(r.status_code == requests.codes.ok)
			r = log_in(user, user)
			self.assertTrue(r.status_code == requests.codes.ok)
			response_json = json.loads(r.content, strict = False)
			self.assertTrue("conn_token" in response_json.keys())
			tokens.append(response_json["conn_token"])

			t_stop = threading.Event()
			stop_events.append(t_stop)
			t = threading.Thread(target=do_stuff, args=(i, t_stop))
			t.daemon = True
			t.start()
			i += 1

		while True:
			if num_requests > 1000:
				i = 0
				for user in users:
					stop_events[i].set()
				break

	def test_integracion_un_usuario(self):
		user = 'gabriel'
		password = 'gayoso'
		email = 'ga-yo-so@hotmail.com'

		r = create_user(user, password)
		self.assertTrue(r.status_code == requests.codes.ok)
		
		r = log_in(user, password)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("conn_token" in response_json.keys())
		token = response_json["conn_token"]

		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		json_response = json.loads(r.content, strict = False)
		self.assertTrue(len(json_response["my_file_tokens"]) == 0)
		self.assertTrue(len(json_response["shared_file_tokens"].keys()) == 0)
		cuota_sin_archs = float(json_response["cuota_actual"])
		
		r = modificar_perfil(user, token, email)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		json_response = json.loads(r.content, strict = False)
		self.assertTrue(json_response["email"] == email)

		r = log_out(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_user(user, token)
		self.assertFalse(r.status_code == requests.codes.ok)

		r = log_in(user, password)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("conn_token" in response_json.keys())
		token = response_json["conn_token"]

		file1 = 'test.txt'
		borrar_si_existe_y_crear_archivo_fisico(file1, "este es un arch de prueba\na ver si funca")

		r = subir_archivo(user, token, file1, [ 'test', 'txt' ], [], [], 'bs as')
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user, token, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user)
		self.assertTrue(response_json["filename"] == file1)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue("txt" in response_json["tags"])
		self.assertTrue("test" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 0)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["my_file_tokens"]) == 1)
		cuota_con_1_arch = float(response_json["cuota_actual"])
		self.assertTrue(cuota_con_1_arch >= cuota_sin_archs)

		file_img = 'test.jpg'
		r = subir_archivo(user, token, file_img, [ 'img' ], [], [], 'colombia')
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user, token, file_img)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user)
		self.assertTrue(response_json["filename"] == file_img)
		self.assertTrue(response_json["extension"] == "jpg")
		self.assertTrue("img" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 0)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["my_file_tokens"]) == 2)
		cuota_con_2_arch = float(response_json["cuota_actual"])
		self.assertTrue(cuota_con_2_arch >= cuota_con_1_arch)

		r = search_files(user, token, "TAG", 'test');
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["search_result"]) == 1)
		self.assertTrue("test.txt" in response_json["search_result"])

		r = delete_file(user, token, file_img)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["my_file_tokens"]) == 1)
		self.assertTrue(float(response_json["cuota_actual"]) == cuota_con_1_arch)

		n_file1 = 'teste.txt'
		borrar_si_existe_y_crear_archivo_fisico(n_file1, "asdasdasd cambios\n")

		r = file_change_filename(user, token, user, file1, n_file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user, token, file1)
		self.assertFalse(r.status_code == requests.codes.ok)
		r = get_file(user, token, n_file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["filename"] == n_file1)

		r = file_change_content(user, token, user, n_file1, 'chile')
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user, token, n_file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["cuota_actual"] != cuota_con_1_arch)
		cuota_con_1_arch = float(response_json["cuota_actual"])
		self.assertTrue(response_json["ultima_ubicacion"] == "chile")
		self.assertTrue(file1 not in response_json["my_file_tokens"])
		
		r = file_change_tags(user, token, user, n_file1, [ 'modif' ], [ 'test' ])
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user, token, n_file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("modif" in response_json["tags"])
		self.assertTrue("test" not in response_json["tags"])
		
		r = file_delete_versions(user, token, user, n_file1, [ 0 ])
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(float(response_json["cuota_actual"]) <= cuota_con_1_arch)
		
		r = file_delete_versions(user, token, user, n_file1, [ 0 ])
		self.assertFalse(r.status_code == requests.codes.ok)

		r = delete_file(user, token, n_file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user, token, n_file1)
		self.assertFalse(r.status_code == requests.codes.ok)
		r = get_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(n_file1 not in response_json["my_file_tokens"])

		r = delete_user(user, token)
		self.assertTrue(r.status_code == requests.codes.ok)

		r = get_user(user, token)
		self.assertFalse(r.status_code == requests.codes.ok)

	def test_integracion_3_usuarios(self):
		user1 = 'gabriel'
		password1= 'gayoso'

		user2 = 'eduardo'
		password2 = 'neira'

		user3 = "cristian"
		password3 = "salas"

		r = create_user(user1, password1)
		self.assertTrue(r.status_code == requests.codes.ok)

		r = log_in(user1, password1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("conn_token" in response_json.keys())
		token1 = response_json["conn_token"]

		create_user(user2, password2)
		r = log_in(user2, password2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("conn_token" in response_json.keys())
		token2 = response_json["conn_token"]

		file1 = 'test1.txt'
		borrar_si_existe_y_crear_archivo_fisico(file1, "asdkhgasjkdas arch de prueba del usr 1\n")

		file2 = 'test2.txt'
		borrar_si_existe_y_crear_archivo_fisico(file2, "este es un arch de prueba del usuario 2\n")

		r = subir_archivo(user1, token1, file1, [ 'test', 'txt' ], [ user2 ], [], 'bs as')
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user1, token1, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user1)
		self.assertTrue(response_json["filename"] == file1)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue("txt" in response_json["tags"])
		self.assertTrue("test" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["my_file_tokens"]) == 1)
		r = get_file(user2, token2, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user1)
		self.assertTrue(response_json["filename"] == file1)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue("txt" in response_json["tags"])
		self.assertTrue("test" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_user(user2, token2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["shared_file_tokens"]) == 1)

		r = subir_archivo(user2, token2, file2, [], [ user1 ], [ user1 ], 'cordoba')
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user2, token2, file2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user2)
		self.assertTrue(response_json["filename"] == file2)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue(len(response_json["tags"]) == 0)
		self.assertTrue(len(response_json["users_with_write_permission"]) == 1)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_user(user2, token2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["my_file_tokens"]) == 1)
		r = get_file(user1, token1, file2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user2)
		self.assertTrue(response_json["filename"] == file2)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue(len(response_json["tags"]) == 0)
		self.assertTrue(len(response_json["users_with_write_permission"]) == 1)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["shared_file_tokens"]) == 1)

		n_file2 = "teste2.md"
		r = file_change_filename(user1, token1, user2, file2, n_file2)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = file_change_tags(user1, token1, user2, n_file2, [ 'modificado_por_user1' ], [])
		self.assertTrue(r.status_code == requests.codes.ok)
		
		r = get_file(user1, token1, file2)
		self.assertTrue(r.status_code != requests.codes.ok)
		file2 = n_file2
		r = get_file(user2, token2, file2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user2)
		self.assertTrue(response_json["filename"] == file2)
		self.assertTrue(response_json["extension"] == "md")
		self.assertTrue("modificado_por_user1" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 1)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(file2 in response_json["shared_file_tokens"].values())
		r = get_user(user2, token2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(file2 in response_json["my_file_tokens"])

		r = file_change_tags(user2, token2, user1, file1, [ 'modificado_por_user2' ], [ 'test' ])
		self.assertTrue(r.status_code != requests.codes.ok)
		r = get_file(user1, token1, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user1)
		self.assertTrue(response_json["filename"] == file1)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue("txt" in response_json["tags"])
		self.assertTrue("test" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_file(user2, token2, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user1)
		self.assertTrue(response_json["filename"] == file1)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue("txt" in response_json["tags"])
		self.assertTrue("test" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)
		r = get_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("test1.txt" in response_json["my_file_tokens"])
		r = get_user(user2, token2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("test1.txt" in response_json["shared_file_tokens"].values())

		r = delete_file(user1, token1, file2)
		self.assertTrue(r.status_code != requests.codes.ok)
		r = get_file(user2, token2, file2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user2)
		self.assertTrue(response_json["filename"] == n_file2)
		self.assertTrue(response_json["extension"] == "md")
		self.assertTrue("modificado_por_user1" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 1)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)

		r = delete_file(user2, token2, file1)
		self.assertTrue(r.status_code != requests.codes.ok)
		r = get_file(user1, token1, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user1)
		self.assertTrue(response_json["filename"] == file1)
		self.assertTrue(response_json["extension"] == "txt")
		self.assertTrue("txt" in response_json["tags"])
		self.assertTrue("test" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 0)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)

		r = delete_file(user1, token1, file1)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_file(user1, token1, file1)
		self.assertTrue(r.status_code != requests.codes.ok)
		r = get_file(user2, token2, file1)
		self.assertTrue(r.status_code != requests.codes.ok)
		r = get_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["my_file_tokens"]) == 0)
		r = get_user(user2, token2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["shared_file_tokens"]) == 0)

		r = create_user(user3, password3)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = log_in(user3, password3)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue("conn_token" in response_json.keys())
		token3 = response_json["conn_token"]

		r = file_change_permissions(user1, token1, user2, file2, [ user3 ], [], [], [])
		self.assertTrue(r.status_code != requests.codes.ok)
		r = get_file(user1, token1, file2)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(response_json["username"] == user2)
		self.assertTrue(response_json["filename"] == n_file2)
		self.assertTrue(response_json["extension"] == "md")
		self.assertTrue("modificado_por_user1" in response_json["tags"])
		self.assertTrue(len(response_json["users_with_write_permission"]) == 1)
		self.assertTrue(len(response_json["users_with_read_permission"]) == 1)

		r = delete_user(user2, token2)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = get_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		response_json = json.loads(r.content, strict = False)
		self.assertTrue(len(response_json["shared_file_tokens"]) == 0)
		r = delete_user(user1, token1)
		self.assertTrue(r.status_code == requests.codes.ok)
		r = delete_user(user3, token3)
		self.assertTrue(r.status_code == requests.codes.ok)

if __name__ == '__main__':
	unittest.main()
else:
	verbose = True


