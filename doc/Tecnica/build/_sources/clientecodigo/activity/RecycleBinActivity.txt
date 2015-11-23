.. java:import:: android.app AlertDialog

.. java:import:: android.content DialogInterface

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.view View

.. java:import:: android.widget AdapterView

.. java:import:: android.widget GridView

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.util ArrayList

.. java:import:: java.util List

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Request

.. java:import:: taller2.fiuba.cliente.dialog DialogoPapeleraDeReciclaje

.. java:import:: taller2.fiuba.cliente.adapter FileGridAdapter

RecycleBinActivity
==================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class RecycleBinActivity extends AppCompatActivity

   Actividad de papelera de reciclaje. Permite ver, restaurar y suprimir archivos eliminados.

Fields
------
archivosEnPapelera
^^^^^^^^^^^^^^^^^^

.. java:field:: static List<String> archivosEnPapelera
   :outertype: RecycleBinActivity

grillaDeArchivosEnPapelera
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. java:field::  GridView grillaDeArchivosEnPapelera
   :outertype: RecycleBinActivity

Methods
-------
actualizarArchivosEnPapelera
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. java:method:: public void actualizarArchivosEnPapelera()
   :outertype: RecycleBinActivity

   Actualiza \ :java:ref:`archivosEnPapelera`\  Actualiza la lista de archivos mostrada en pantalla.

listarArchivosEnPapelera
^^^^^^^^^^^^^^^^^^^^^^^^

.. java:method:: public JSONArray listarArchivosEnPapelera()
   :outertype: RecycleBinActivity

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: RecycleBinActivity

   Constructor de la actividad de papelera de reciclaje. Llama a \ :java:ref:`actualizarArchivosEnPapelera()`\  Inicializa las variables \ :java:ref:`token`\  y \ :java:ref:`username`\ . Inicializa el listener para cuando se clickea un archivo.

   :param savedInstanceState:

vaciarPapelera
^^^^^^^^^^^^^^

.. java:method:: public void vaciarPapelera(View view)
   :outertype: RecycleBinActivity

   Primero pregunta al usuario si esta seguro de querer vaciar la papelera. En caso afirmativo, pide al server que lo haga y vacia la lista \ :java:ref:`archivosEnPapelera`\ . Llama a \ :java:ref:`actualizarArchivosEnPapelera()`\ .

   :param view:

