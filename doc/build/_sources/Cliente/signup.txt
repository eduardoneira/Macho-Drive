======================================
Sign Up
======================================

Sign Up
======================================

 * Precondicion: Macho-Drive debe estar ejecutandose y el cliente no debe tener una cuenta.
 * Postcondicion: El cliente tiene una cuenta con usuario y contraseña de su elección.
 * Actores: Usuario y Administrador.
 * Flujo principal:

  1. Usuario completa los datos de nombre de usuario y contraseña
  #. Usuario selecciona el botón Sign Up. 
  #. Administrador valida los datos ingresados por Usuario con el servidor.
  #. Si los datos fueron correctos, Administrador guarda los datos de Usuario en el servidor.
  #. Administrador indica a Usuario que el registro fue exitoso.

 * Flujo alternativo 1:

  4.  Si el usuario ya existe o se ingresaron caracteres no permitidos, Administrador indica a Usuario que el registro no fue exitoso.

