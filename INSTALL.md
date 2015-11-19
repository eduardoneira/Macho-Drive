#Macho Drive

============================================================================================================
Manual de instalación y uso para el Servidor
============================================================================================================

La aplicación Macho-Drive está pensada para correr en linux 64 bits. A su vez se requieren tener ciertas librerías y programas instalados.
Esta guía de instalación detalla como conseguir esto, orientando las consignas para la versión Ubuntu 14.04. Sin embargo esto debería poder funcionar con otras versiones de Ubuntu y ser análoga a otras versiones de Linux.


CMake
------------------------------------------------------------------------------------------------------------
   En caso de no tener cmake instalado:

.. code-block:: bash
   
	$ sudo apt-get update
	$ sudo apt-get install cmake


Clonando el github
------------------------------------------------------------------------------------------------------------
Primero se debe clonar nuestro github, usando el siguiente comando:

.. code-block:: bash

	$ git clone https://github.com/eduardoneira/Macho-Drive.git

En el caso de no tener git instalado:
   
   $ sudo apt-get install git

Seteando rocksdb
------------------------------------------------------------------------------------------------------------
Para poder setear rocksdb correctamente, se debe tener dentro de la carpeta  server/lib/RocksDB/ la libreria librocksdb.a. En caso de no ser así, correr el siguiente comando:

.. code-block:: bash

   $ ./unpack_rocksdb


Compilando el server
------------------------------------------------------------------------------------------------------------
Para compilar  el servidor hay que situarse en la carpeta /server/ y correr el script build.sh

.. code-block:: bash

   $ cd server/
   $ ./build.sh

Corriendo el server
------------------------------------------------------------------------------------------------------------
Para correr el servidor hay que situarse en la carpeta /server/build/ y ejecutar ./Server. Si se quiere cambiar el directorio donde se va a crear la base de datos, -Ddb_path,/home/mi_path. Tambien se puede cambiar el puerto de la siguiente manera, -Dport,:8000. A continuacion se mostrará un ejemplo de un server abierto en el puerto 8080 con la base de datos en /home/new_path

.. code-block:: bash

   $ cd server/
   $ ./build.sh -Ddb_path,/home/new_path -Dport,:8080



Correr tests de python
------------------------------------------------------------------------------------------------------------
Situado en la carpeta /server/python\ integration\ tests, correr el script test_server_func.py. Esto ejecutara los test de integracion de python. Se debe tener instalado python 2.7

.. code-block:: bash

   $ python server_test.py


Correr unit test del server
------------------------------------------------------------------------------------------------------------
Se debe estar situado en la carpeta server. Para correr las unit test se debe haber compilado el server con ./build.sh. A su vez, se debe tener instalado lcov y coveralls-lcov. De no ser el caso, correr el script install-coveralls. Luego se podrá correr el script run_tests.sh el cual ejecuta todas las unit test.

.. code-block:: bash
   
   $ ./build.sh
   $ sudo ./install_coveralls
   $ ./run_tests.sh

Correr coverage del server
------------------------------------------------------------------------------------------------------------
Se debe estar situado en la carpeta server. Para correr el coverage se debe haber compilado el server con ./build.sh. A su vez, se debe tener instalado lcov y coveralls-lcov. De no ser el caso, correr el script install-coveralls como root. Luego se podrá correr el script run_coverage.sh el cual ejecuta todas las unit test y se realiza el coverage del server.

.. code-block:: bash
   
   $ ./build.sh
   $ sudo ./install_coveralls
   $ ./run_coverage.sh
