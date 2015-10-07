from server_test_func import *
import os

user = 'gabriel'
password = 'gayoso'
email = 'ga-yo-so@hotmail.com'
path = os.getcwd()
path_files = path + '/files/'

crear_usuario(user, password)
token = hacer_log_in(user, password)
get_usuario(user, token)

modificar_perfil(user, token, email)
get_usuario(user, token)

log_out(user, token)
get_usuario(user, token)

token = hacer_log_in(user, password)

file1 = 'test.txt'
tags = [ 'test', 'txt' ]
subir_archivo(user, token, path_files+file1, tags, [], [], "bs as")
get_file(user, token, user, file1)
get_usuario(user, token)

log_out(user, token)

print "/*****************************************************"