#Macho Drive

Macho Drive es una aplicacion diseñada para correr en linux 64 bits.

Para ejecutar el Macho Drive se deben tener instaladas las siguientes dependencias:
	. cmake

	. rocksdb

En caso de no tener instalado cmake, ejecutar el siguiente comando:
	
	$ sudo apt-get install cmake

En caso de no tener la libreria de rocksdb, mas adelante se especificará como instalarlo.


# Clonando el github

	Primero se debe clonar nuestro github, usando el siguiente comando:
	
	$ git clone https://github.com/eduardoneira/Macho-Drive.git


# Seteando rocksdb

	Para poder setear rocksdb correctamente, se debe crear una carpeta con el nombre RocksDB, clonar su repositorio y crear una libreria estatica.
	
	$ cd server/lib/
		
	$ mkdir RocksDB

	$ cd RocksDB

	$ git clone https://github.com/facebook/rocksdb.git

	$ cd rocksdb
	
	$ make static_lib

	$ mv librocksdb.a ../


# Compilando el server

	Para compilar el servidor se debe volver a la carpeta de server y correr el bash build.sh

	$ cd ../../

	$ bash build.sh


# Corriendo el server

	Para correr el servidor hay que situarse en la carpeta /server/build/ y correr la aplicacion Server
	
	$ cd build/

	$ ./Server

	Ahora deberia tener un server abierto en el puerto 8080 del localhost
	
# Correr tests de python

	Situado en la carpeta /server/python\ integration\ tests, correr los dos scripts 'server_test.py' y 
	Ambos imprimen en la primer linea el request enviado al servidor con la forma: 'METHOD' 'URI' 'BODY', y en la segunda la respuesta del servidor (no es la definitiva, va a cambiar mas adelante seguro)
	
	$ python server_test.py
	$ python test_shared_files.py