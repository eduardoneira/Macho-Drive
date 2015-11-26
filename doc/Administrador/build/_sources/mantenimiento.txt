============================================================================================================
Mantenimiento
============================================================================================================

El mantenimiento del Server de Macho Drive es un aspecto de suma importancia para el correcto funcionamiento del sistema. Para esto se deben considerar dos factores:
					+ La base de datos
					+ El Log

La base de datos es una parte fundamental en cuento al desarrollo del sistema ya que es la encargada de mantener todos los archivos y datos de los usuarios. Esta suele encontrarse en la carpeta /tmp/prod/ pero puede cambiarse su ubicación como fue indicado en la sección  Instalación , más especificamente en "Corriento el Server". Si se cambia de directorio base, los datos no se trasladan así que al hacer este cambio se debe tener en cuenta que se tiene una base de datos nueva.


Base de Datos
<<<<<<<<<<<<<<<<
Como se aclaró anteriormente, la base de datos se encarga de guardar archivos y datos de usuarios al mismo tiempo, además de las sesiones. La manera de identificar rapidamente con que tipo de archivo estamos tratando es al ver la clave que poseen en la base de datos. Los elementos de la base de datos son:

			+ User : (clave : "$username") Guarda el nombre de usuario y su contraseña.
			+ User Metadata : (clave : "$username.usertoken") : Guarda todos los datos del usuario, desde el perfil hasta la cantidad de archivos que tiene y cuales puede acceder.
			+ File Data : (clave : "md5($owner_username$filename)") : Guarda el archivo con su contenido, tags, extension, etc. La clave esta encriptada con md5
 

Log
<<<<<<<<
Dentro de la carpeta *build* se encuentra un archivo llamado log.txt en el cual se encuentran todas las actividades importantes que debe realizar el servidor. A medida que se agregan funcionalidades o se actualiza el código es fundamental seguir logeando para poder detectar errores o fallas en el server. 
