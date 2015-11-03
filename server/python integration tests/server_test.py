from server_test_func import *
import os

print "/********************************************************************************"
print "/********************************************************************************"
print "/********************************************************************************"

user = 'gabriel'
password = 'gayoso'
email = 'ga-yo-so@hotmail.com'

crear_usuario(user, password)
token = hacer_log_in(user, password)
get_usuario(user, token)

modificar_perfil(user, token, email)
get_usuario(user, token)

log_out(user, token)
get_usuario(user, token)

token = hacer_log_in(user, password)

file1 = 'test.txt'
borrar_si_existe_y_crear_archivo_fisico(file1, "este es un arch de prueba\n")

subir_archivo(user, token, file1, [ 'test', 'txt' ], [], [], 'bs as')
get_file(user, token, file1)
get_usuario(user, token)

file_img = 'test.jpg'
subir_imagen(user, token, file_img, [ 'img' ], [], [], 'colombia')
get_imagen(user, token, file_img)
get_usuario(user, token)

search_files(user, token, "TAG", 'test');

n_file1 = 'teste.txt'
borrar_si_existe_y_crear_archivo_fisico(n_file1, "asdasdasd cambios\n")
modify_file(user, token, user, file1, n_file1, "", [ 'modif'], [ 'test'], [], [], [], [], 'chile')
get_file(user, token, file1)
get_file(user, token, n_file1)
get_usuario(user, token)

delete_file(user, token, n_file1)
get_file(user, token, n_file1)
get_usuario(user, token)

delete_user(user, token)

get_usuario(user, token)

print "/********************************************************************************"
print "/********************************************************************************"
print "/********************************************************************************"