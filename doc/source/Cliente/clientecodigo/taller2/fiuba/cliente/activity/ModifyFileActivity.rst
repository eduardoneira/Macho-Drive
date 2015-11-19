.. java:import:: android.app AlertDialog

.. java:import:: android.content DialogInterface

.. java:import:: android.location Location

.. java:import:: android.location LocationListener

.. java:import:: android.location LocationManager

.. java:import:: android.view View

.. java:import:: android.widget AdapterView

.. java:import:: android.widget GridView

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.widget EditText

.. java:import:: android.widget Toast

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.util ArrayList

.. java:import:: java.util List

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.adapter TagsGridAdapter

.. java:import:: taller2.fiuba.cliente.model Permissions

.. java:import:: taller2.fiuba.cliente.model Request

ModifyFileActivity
==================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class ModifyFileActivity extends AppCompatActivity

   Actividad de modificacion de nombre de archivo y tags.

Fields
------
tagsGrid
^^^^^^^^

.. java:field::  GridView tagsGrid
   :outertype: ModifyFileActivity

   Grilla de tags

Methods
-------
actualizarTags
^^^^^^^^^^^^^^

.. java:method:: protected void actualizarTags()
   :outertype: ModifyFileActivity

   Pide al server la lista de tags del archivo y la muestra.

addTag
^^^^^^

.. java:method:: protected void addTag(String tag)
   :outertype: ModifyFileActivity

   Pide al server que agregue el tag solicitado. Llama a \ :java:ref:`actualizarTags()`\

   :param tag: El tag que se quiere agregar

addTagButton
^^^^^^^^^^^^

.. java:method:: public void addTagButton(View view)
   :outertype: ModifyFileActivity

   Metodo llamado al clickear el boton de agregar tag Llama a \ :java:ref:`addTag(String)`\

   :param view:

agregarUbicacion
^^^^^^^^^^^^^^^^

.. java:method:: public JSONObject agregarUbicacion(JSONObject data)
   :outertype: ModifyFileActivity

   Pide la ubicacion actual y la agrega a data. En caso de no tener GPS, devuelve un string vacio.

   :param data: El JSONObject al que se le quiere agregar la ubicacion
   :return: data con la ubicacion agregada

deleteTag
^^^^^^^^^

.. java:method:: public void deleteTag(String tag)
   :outertype: ModifyFileActivity

   Pide al server que elimine el tag solicitado y actualiza la lista de tags.

   :param tag: El tag a ser eliminado

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: ModifyFileActivity

   Constructor de la actividad de modificacion de archivos. Inicializa las variables \ :java:ref:`token`\ , \ :java:ref:`filename`\  y \ :java:ref:`username`\ . Muestra el nombre del archivo. Inicializa la lista de tags. Crea el listener para cuando se quiere eliminar un archivo.

   :param savedInstanceState:

rename
^^^^^^

.. java:method:: public void rename(View view)
   :outertype: ModifyFileActivity

   Renombra el archivo segun el nombre que el usuario ingreso

   :param view:

