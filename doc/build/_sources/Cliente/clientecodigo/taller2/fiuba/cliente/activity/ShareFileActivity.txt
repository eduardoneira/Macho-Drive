.. java:import:: android.app AlertDialog

.. java:import:: android.content DialogInterface

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.view View

.. java:import:: android.widget AdapterView

.. java:import:: android.widget EditText

.. java:import:: android.widget GridView

.. java:import:: android.widget Toast

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.util ArrayList

.. java:import:: java.util List

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Request

.. java:import:: taller2.fiuba.cliente.adapter TagsGridAdapter

ShareFileActivity
=================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class ShareFileActivity extends AppCompatActivity

   Actividad para compartir y descompartir un archivo

Fields
------
usersGrid
^^^^^^^^^

.. java:field::  GridView usersGrid
   :outertype: ShareFileActivity

   Grilla de usuarios con acceso al archivo.

Methods
-------
actualizarUsers
^^^^^^^^^^^^^^^

.. java:method:: protected void actualizarUsers()
   :outertype: ShareFileActivity

   Actualiza la lista de users con los que esta compartido el archivo.

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: ShareFileActivity

   Constructor de la actividad encargada de compartir archivos. Inicializa las variables \ :java:ref:`token`\ , \ :java:ref:`username`\  y \ :java:ref:`filename`\ . Inicializa el listener para cuando se quiere descompartir un archivo. \ :java:ref:`unshare(String)`\

   :param savedInstanceState:

shareButton
^^^^^^^^^^^

.. java:method:: public void shareButton(View view)
   :outertype: ShareFileActivity

   Pide al server que comparta el archivo con el usuario indicado. En caso de que no exista, se informa al usuario.

   :param view:

unshare
^^^^^^^

.. java:method:: protected void unshare(String username)
   :outertype: ShareFileActivity

   Pregunta al usuario si está seguro de querer descompartir el archivo \ :java:ref:`filename`\ . En caso afirmativo, pide al server que lo haga. Llama a \ :java:ref:`actualizarUsers()`\

   :param username:

