.. java:import:: android.content ActivityNotFoundException

.. java:import:: android.content Intent

.. java:import:: android.location Location

.. java:import:: android.location LocationListener

.. java:import:: android.location LocationManager

.. java:import:: android.net Uri

.. java:import:: android.os Environment

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.util Base64

.. java:import:: android.view View

.. java:import:: android.view Menu

.. java:import:: android.view MenuItem

.. java:import:: android.widget AdapterView

.. java:import:: android.widget ArrayAdapter

.. java:import:: android.widget EditText

.. java:import:: android.widget GridView

.. java:import:: android.widget AdapterView.OnItemClickListener

.. java:import:: android.widget ImageView

.. java:import:: android.widget Spinner

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.io File

.. java:import:: java.io FileInputStream

.. java:import:: java.io FileNotFoundException

.. java:import:: java.io IOException

.. java:import:: java.util Iterator

.. java:import:: java.util ArrayList

.. java:import:: java.util List

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Request

.. java:import:: taller2.fiuba.cliente.dialog DialogoArchivos

.. java:import:: taller2.fiuba.cliente.adapter FileGridAdapter

.. java:import:: taller2.fiuba.cliente.model Permissions

NavigationActivity
==================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class NavigationActivity extends AppCompatActivity

   Actividad principal. Muestra los archivos a los que el usuario tiene acceso. Es la primera actividad que ve el usuario al conectarse al sistema.

Fields
------
archivos
^^^^^^^^

.. java:field:: static List<String> archivos
   :outertype: NavigationActivity

   Lista de archivos del usuario

gridView
^^^^^^^^

.. java:field::  GridView gridView
   :outertype: NavigationActivity

   Grilla de archivos del usuario

Methods
-------
actualizarArchivos
^^^^^^^^^^^^^^^^^^

.. java:method:: public void actualizarArchivos()
   :outertype: NavigationActivity

   Llama a \ :java:ref:`listFiles()`\  y actualizan los archivos mostrados en la cuadricula.

listFiles
^^^^^^^^^

.. java:method:: public JSONArray listFiles()
   :outertype: NavigationActivity

   Pide al server la lista de archivos a los que el usuario tiene acceso. Tanto propios como compartidos.

   :return: Lista de archivos a los que el usuario puede acceder.

logOut
^^^^^^

.. java:method:: public void logOut()
   :outertype: NavigationActivity

   Se desconecta del sistema.

onActivityResult
^^^^^^^^^^^^^^^^

.. java:method:: @Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
   :outertype: NavigationActivity

   Metodo que maneja la finalizacion de actividades. Si se eligio un archivo para ser subido, se llama a \ :java:ref:`uploadFile`\ . Si se elimino el usuario en \ :java:ref:`ProfileSettingsActivity`\ , se vuelve a \ :java:ref:`MainActivity`\ .

   :param requestCode: Código de la actividad iniciada.
   :param resultCode: Código resultado de la actividad.
   :param data: Datos resultados de la actividad.

onBackPressed
^^^^^^^^^^^^^

.. java:method:: @Override public void onBackPressed()
   :outertype: NavigationActivity

   Al presionar el boton Back, se desloggea del sistema. Llama a \ :java:ref:`logOut()`\

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: NavigationActivity

   Constructor de la actividad principal. Inicializa las variables \ :java:ref:`token`\  y \ :java:ref:`username`\ . Inicializa \ :java:ref:`archivos`\  Llama a \ :java:ref:`actualizarArchivos()`\  Inicializa el listener para que los archivos puedan ser clickeados. \ :java:ref:`DialogoArchivos`\  Inicializa el buscador.

   :param savedInstanceState:

onCreateOptionsMenu
^^^^^^^^^^^^^^^^^^^

.. java:method:: @Override public boolean onCreateOptionsMenu(Menu menu)
   :outertype: NavigationActivity

   Metodo que crea el menu cuando se presiona el boton superior derecho.

   :param menu: El menu a crear
   :return: Si salio bien

onOptionsItemSelected
^^^^^^^^^^^^^^^^^^^^^

.. java:method:: @Override public boolean onOptionsItemSelected(MenuItem item)
   :outertype: NavigationActivity

   Método que responde cuando se clickea un item en el menú. Si se presionó el botón UP, se desconecta del sistema y vuelve a la actividad inicial. \ :java:ref:`logOut()`\  Si se presionó Profile Settings, se abre \ :java:ref:`ProfileSettingsActivity`\ . Si se presionó Search User, se abre \ :java:ref:`UserProfileActivity`\  Si se presionó Upload File, se abre una ventana de selección de archivo para ser subido. Si se presionó Deleted Files, se abre \ :java:ref:`RecycleBinActivity`\ .

   :param item: El item presionado

onResume
^^^^^^^^

.. java:method:: @Override public void onResume()
   :outertype: NavigationActivity

   Al resumir la actividad se actualiza la lista de archivos. \ :java:ref:`actualizarArchivos()`\

uploadFile
^^^^^^^^^^

.. java:method:: public void uploadFile(String path)
   :outertype: NavigationActivity

   Se encodea en Base 64 el archivo en el path indicado y se pide al server que lo suba. Se chequean los permisos necesarios. Se piden al usuario en caso de que falten. \ :java:ref:`Permissions`\  Se actualizan \ :java:ref:`ubicacionLoc`\  y \ :java:ref:`ubicacion`\  Llama a \ :java:ref:`actualizarArchivos()`\

   :param path: La ruta del archivo a subir

