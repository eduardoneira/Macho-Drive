Documentación de REST API
==============================================================================


USERS
------------------------------------------------------------------------------

COLLECTION
        * POST /users/ quiere decir sign up
        * GET /users/'username'/profile/

ELEMENT
        * GET /users/'username' quiere decir pedir info del usuario.
        * DELETE /users/'username' quiere decir borrar el usuario.
        * PUT /users/'username' quiere decir modificar perfil de tal usuario.

SESSIONS
------------------------------------------------------------------------------

COLLECTION
        * POST /sessions/ quiere decir log in.

ELEMENT
        * DELETE /sessions/'username' quiere decir log out.


FILES
------------------------------------------------------------------------------

COLLECTION
        * POST /files/'username'/ quiere decir subir archivo de tal usuario.
        * GET /files/'username'/ devuelve un arbol de archivos.
        * GET /files/'username'/search/val1/val2 busca en los archivos del usuario segun el campo val1 (tag, nombre o extension) y valor val2(.exe, test, miarch.txt, etc).
        * GET /files/'username'/recycle_bin/ devuelve los archivos de la recycle bin del usuario.
        * DELETE /files/'username'/recycle_bin/ limpia la recycle bin del usuario.
        * PUT /files/'username'/recycle_bin/'filename' restaura el archivo de la recycle bin del usuario.

ELEMENT
        * GET /files/'username'/'filename' quiere decir pedir archivo de tal usuario
        * DELETE /files/'username'/'filename' quiere decir borrar archivo de tal usuario
        * PUT /files/'username'/'filename' quiere decir modificar archivo de tal usuario

OTHER
-----------------------------------------------------------------------------
    * cualquier otro request es invalido, y se devuelve error

