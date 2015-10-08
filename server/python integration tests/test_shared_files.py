from server_test_func import *
import os

print "/********************************************************************************"
print "/********************************************************************************"
print "/********************************************************************************"

user1 = 'gabriel'
password1= 'gayoso'

user2 = 'eduardo'
password2 = 'neira'

crear_usuario(user1, password1)
token1 = hacer_log_in(user1, password1)

crear_usuario(user2, password2)
token2 = hacer_log_in(user2, password2)

file1 = 'test1.txt'
borrar_si_existe_y_crear_archivo_fisico(file1, "asdkhgasjkdas arch de prueba del usr 1\n")

file2 = 'test2.txt'
borrar_si_existe_y_crear_archivo_fisico(file1, "este es un arch de prueba del usuario 2\n")