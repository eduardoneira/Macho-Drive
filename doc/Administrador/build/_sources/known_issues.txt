Known Issues
================================================================================================
Si el servidor se cae mientras corre el cliente, el cliente no se entera que el server murió

Los archivos no se envian por multipart, sino con us contenido en el cuerpo del request http. Por esto, al subir un archivo grande (>100mb) el cliente se queda esperando un rato.

En el cliente falla la operacion de seleccionar de seleccionar archivos para subir (bug independiente del servidor, nunca llega a enviar el archivo)

La funcionalidad de GetUserProfile que deberia brindar la informacion publica del perfil de un usuario no funciona correctamente.


