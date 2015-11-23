.. java:import:: android.content ActivityNotFoundException

.. java:import:: android.content Intent

.. java:import:: android.net Uri

.. java:import:: android.os Environment

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.util Base64

.. java:import:: android.view Menu

.. java:import:: android.view MenuItem

.. java:import:: android.view View

.. java:import:: android.widget AdapterView

.. java:import:: android.widget GridView

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.io File

.. java:import:: java.io FileInputStream

.. java:import:: java.io FileNotFoundException

.. java:import:: java.io IOException

.. java:import:: java.util ArrayList

.. java:import:: java.util List

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Request

.. java:import:: taller2.fiuba.cliente.dialog DialogoVersiones

.. java:import:: taller2.fiuba.cliente.adapter FileGridAdapter

.. java:import:: taller2.fiuba.cliente.model Permissions

FileVersionsActivity
====================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class FileVersionsActivity extends AppCompatActivity

   Actividad de manejo de versiones de un archivo.

Fields
------
contenidoVersiones
^^^^^^^^^^^^^^^^^^

.. java:field:: static List<String> contenidoVersiones
   :outertype: FileVersionsActivity

   Contenidos de las versiones

gridView
^^^^^^^^

.. java:field::  GridView gridView
   :outertype: FileVersionsActivity

   Grilla de versiones

versiones
^^^^^^^^^

.. java:field:: static List<String> versiones
   :outertype: FileVersionsActivity

   Lista de versiones

Methods
-------
mostrarVersiones
^^^^^^^^^^^^^^^^

.. java:method:: public void mostrarVersiones()
   :outertype: FileVersionsActivity

   Actualiza la lista de versiones que se muestra en pantalla. Pide al server la lista y la muestra en una cuadricula \ :java:ref:`gridView`\

onActivityResult
^^^^^^^^^^^^^^^^

.. java:method:: @Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
   :outertype: FileVersionsActivity

   Si se eligio un archivo para ser subido, se lo sube. Llama a \ :java:ref:`uploadVersion(String)`\

   :param requestCode: Codigo de pedido
   :param resultCode: Codigo resultado
   :param data: Datos devueltos

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: FileVersionsActivity

   Constructor. Inicializa las variables de la actividad. Inicializa \ :java:ref:`username`\ , \ :java:ref:`token`\  y \ :java:ref:`filename`\  Llama a \ :java:ref:`mostrarVersiones()`\  Establece el listener para cuando se clickea una version. \ :java:ref:`DialogoVersiones`\

   :param savedInstanceState:

onCreateOptionsMenu
^^^^^^^^^^^^^^^^^^^

.. java:method:: @Override public boolean onCreateOptionsMenu(Menu menu)
   :outertype: FileVersionsActivity

   Inicializador del menu de opciones

   :param menu: El menu a inicializar
   :return: Si salio bien

onOptionsItemSelected
^^^^^^^^^^^^^^^^^^^^^

.. java:method:: @Override public boolean onOptionsItemSelected(MenuItem item)
   :outertype: FileVersionsActivity

   Abre un navegador de archivos para elegir la nueva version a ser subida.

   :param item:

uploadVersion
^^^^^^^^^^^^^

.. java:method:: public void uploadVersion(String path)
   :outertype: FileVersionsActivity

   Sube al servidor una nueva version del archivo. Llama a \ :java:ref:`Permissions`\  Encodea el contenido del archivo en Base64. Llama a \ :java:ref:`mostrarVersiones()`\

   :param path: Ruta del archivo que se desea subir

