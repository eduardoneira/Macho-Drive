Librerías
===========================================================


Para manejar distintos recursos en el proceso de desarrollo se emplearon librerías open source.

Servidor http
---------------------------------------------------------------------

Para realizar las operaciones para manejar el servidor http se empleó `mongoose <https://github.com/cesanta/mongoose>`_, el cual facilitó en gran medida el manejo de http requests.


Base de datos
---------------------------------------------------------------------

Para el manejo de la base de datos por parte del servidor, se utilizó una base de datos no relacional del tipo *clave/valor* llamada `rocksdb <https://github.com/facebook/rocksdb>`_.


Datos json
---------------------------------------------------------------------

Para la manipulación de datos json se hizo uso de la librerías `jsoncpp <https://github.com/open-source-parsers/jsoncpp>`_. Los json se usaron tanto para el envío de datos a través de los mensajes http como en la persistencia de datos del lado del servidor.


