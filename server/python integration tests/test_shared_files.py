from server_test_func import *
import os

print "/********************************************************************************"
print "/********************************************************************************"
print "/********************************************************************************"

user1 = 'gabriel'
password1= 'gayoso'

user2 = 'eduardo'
password2 = 'neira'

user3 = "cristian"
password3 = "salas"

crear_usuario(user1, password1)
token1 = hacer_log_in(user1, password1)

crear_usuario(user2, password2)
token2 = hacer_log_in(user2, password2)

file1 = 'test1.txt'
borrar_si_existe_y_crear_archivo_fisico(file1, "asdkhgasjkdas arch de prueba del usr 1\n")

file2 = 'test2.txt'
borrar_si_existe_y_crear_archivo_fisico(file2, "este es un arch de prueba del usuario 2\n")

subir_archivo(user1, token1, file1, [ 'test', 'txt' ], [ user2 ], [], 'bs as')
get_file(user1, token1, user1, file1)
get_usuario(user1, token1)
get_file(user2, token2, user1, file1)
get_usuario(user2, token2)

subir_archivo(user2, token2, file2, [], [ user1 ], [ user1 ], 'cordoba')
get_file(user2, token2, user2, file2)
get_usuario(user2, token2)
get_file(user1, token1, user2, file2)
get_usuario(user1, token1)

n_file2 = "teste2.md"
modify_file(user1, token1, user2, file2, n_file2, "", [ 'modificado_por_user1' ], [], [], [], [], [], 'francia')
get_file(user2, token2, user2, file2)
file2 = n_file2
get_file(user1, token1, user2, file2)
get_file(user2, token2, user2, file2)
get_usuario(user1, token1)
get_usuario(user2, token2)

modify_file(user2, token2, user1, file1, "", "", [ 'modificado_por_user2' ], [ 'test' ], [], [], [], [], 'bolivia')
get_file(user1, token1, user1, file1)
get_file(user2, token2, user1, file1)
get_usuario(user1, token1)
get_usuario(user2, token2)

delete_file(user1, token1, file2)
get_file(user2, token2, user2, file2)

delete_file(user2, token2, file1)
get_file(user1, token1, user1, file1)

delete_file(user1, token1, file1)
get_file(user1, token1, user1, file1)
get_file(user2, token2, user1, file1)
get_usuario(user1, token1)
get_usuario(user2, token2)

crear_usuario(user3, password3)
token3 = hacer_log_in(user3, password3)

modify_file(user1, token1, user2, file2, "", "", [], [], [ user3 ], [], [], [], 'peru')
get_file(user1, token1, user2, file2)

delete_user(user2, token2)
get_usuario(user1, token1)
delete_user(user1, token1)
delete_user(user3, token3)

print "/********************************************************************************"
print "/********************************************************************************"
print "/********************************************************************************"
