Diseño
===========================================================

Para poder visualizar la arquitectura del proyecto se presenta aquí un diagrama UML que muestra las clases más importantes del programa.


.. figure:: diagrama/Diagrama_de_Clases.png
	:scale: 100%



Aquí se puede ver como Server es quien se encarga de mantener la conexión, pero HandlerManager es la clase que maneja todas las httprequests, seleccionando cada handler acorde a la acción que deba hacer. A su vez, cada handler empleará los DBElements que necesite par subir, modificar o extraer datos de la clase Database, que es quien se encarga de manejar la base de datos.
