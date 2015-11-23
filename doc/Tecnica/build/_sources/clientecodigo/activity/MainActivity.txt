.. java:import:: android.content Intent

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.view View

.. java:import:: android.widget EditText

.. java:import:: android.widget Toast

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.util HashMap

.. java:import:: java.util Map

.. java:import:: java.util.regex Pattern

.. java:import:: java.security MessageDigest

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Request

MainActivity
============

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class MainActivity extends AppCompatActivity

   Actividad inicial. Permite registrarse e ingresar al sistema.

Methods
-------
logIn
^^^^^

.. java:method:: public void logIn(View view)
   :outertype: MainActivity

   Metodo de conexion al sistema. Toma el usuario y passwords que se ingresaron e intenta conectarse al sistema. Se pide que los campos no sean nulos y sean alfanumericos. En caso de ingresar un campo con caracteres inválidos, se informa al usuario de ello. En caso de ingresar nombre de usuario o password incorrecta, se le informa al usuario. Si todo sale bien, inicializa \ :java:ref:`NavigationActivity`\

   :param view:

md5
^^^

.. java:method:: public static String md5(String password)
   :outertype: MainActivity

   Hashea la password segun el algoritmo md5.

   :param password:
   :return: Hash md5 de password

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: MainActivity

   Constructor. Deja invisibles los mensajes de error ("Solo se permiten letras o numeros".

   :param savedInstanceState:

signUp
^^^^^^

.. java:method:: public void signUp(View view)
   :outertype: MainActivity

   Metodo llamado al presionar el boton Sign Up. Toma los campos ingresados por el usuario e intenta registrarse en el sistema. Se pide que los campos no sean nulos y que sean alfanumericos. En caso de ingresar caracteres invalidos, se informa al usuario. En caso de que ya exista el nombre de usuario, se informa al usuario.

   :param view:

