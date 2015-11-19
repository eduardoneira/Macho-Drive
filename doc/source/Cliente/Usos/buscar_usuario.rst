======================================
Búsqueda de usuario
======================================

Buscar Usuario
======================================

 * Precondicion: Macho-Drive debe estar ejecutandose y en la pantalla principal.
 * Postcondicion: Se presentan los datos del usuario buscado.
 * Actores: Usuario y Administrador.
 * Flujo principal:

  1. Usuario presiona sobre el menu arriba a la derecha.
  #. Usuario presiona Search User Profile. 
  #. Administrador muestra la pantalla de visualización de perfiles de usuario.
  #. Usuario ingresa nombre del usuario que desea.
  #. Usuario presiona Search.
  #. Administrador busca el usuario pedido.
  #. Si existe, Administrador muestra por pantalla nombre, email, imagen y última posición conocida del usuario.

 * Flujo alternativo 1:

  7. Si el usuarion no existe Administrador notifica por pantalla.
