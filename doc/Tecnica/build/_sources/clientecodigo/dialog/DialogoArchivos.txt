.. java:import:: android Manifest

.. java:import:: android.app Activity

.. java:import:: android.app AlertDialog

.. java:import:: android.app Dialog

.. java:import:: android.app DialogFragment

.. java:import:: android.content DialogInterface

.. java:import:: android.content Intent

.. java:import:: android.content.pm PackageManager

.. java:import:: android.os Bundle

.. java:import:: android.support.v4.app ActivityCompat

.. java:import:: android.util Base64

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONObject

.. java:import:: java.io File

.. java:import:: java.io FileOutputStream

.. java:import:: java.util.logging Logger

.. java:import:: taller2.fiuba.cliente.activity FileVersionsActivity

.. java:import:: taller2.fiuba.cliente.activity ModifyFileActivity

.. java:import:: taller2.fiuba.cliente.activity ShareFileActivity

.. java:import:: taller2.fiuba.cliente.model Request

.. java:import:: taller2.fiuba.cliente.activity NavigationActivity

.. java:import:: taller2.fiuba.cliente.model Permissions

DialogoArchivos
===============

.. java:package:: taller2.fiuba.cliente.dialog
   :noindex:

.. java:type:: public class DialogoArchivos extends DialogFragment

   Dialogo con cinco opciones: Download, Edit Details, Delete, Share y Versions

Fields
------
activity
^^^^^^^^

.. java:field::  Activity activity
   :outertype: DialogoArchivos

Methods
-------
deletefile
^^^^^^^^^^

.. java:method:: public void deletefile(String filename)
   :outertype: DialogoArchivos

   Pregunta al usuario si está seguro de querer eliminar el archivo. En caso afirmativo, pide al server que lo mande a la papelera de reciclaje.

   :param filename: Archivo a ser eliminado

fileVersions
^^^^^^^^^^^^

.. java:method:: public void fileVersions(String filename)
   :outertype: DialogoArchivos

   Inicia \ :java:ref:`FileVersionsActivity`\

   :param filename: El nombre del archivo cuyas versiones se desean ver

getFile
^^^^^^^

.. java:method:: public JSONObject getFile(String filename)
   :outertype: DialogoArchivos

   Pide el archivo solicitado al server.

   :param filename: Archivo a pedir
   :return: Respuesta del server

modifyFile
^^^^^^^^^^

.. java:method:: public void modifyFile(String filename)
   :outertype: DialogoArchivos

   Inicia \ :java:ref:`ModifyFileActivity`\ .

   :param filename: El nombre del archivo cuyos detalles se desea editar

onAttach
^^^^^^^^

.. java:method:: @Override public void onAttach(Activity activity)
   :outertype: DialogoArchivos

   Guarda una referencia a la \ :java:ref:`activity`\  que lo abrió.

   :param activity: La actividad que abrió el diálogo

onCreateDialog
^^^^^^^^^^^^^^

.. java:method:: @Override public Dialog onCreateDialog(Bundle savedInstanceState)
   :outertype: DialogoArchivos

   Muestra las distintas opciones al clickear un archivo: Download, Edit details, Delete, Share, Versions. Si se presionó Download, se descarga el archivo en cuestión, pidiéndose permisos en caso de ser necesario. Si se presionó Edit details, se llama a \ :java:ref:`modifyFile`\ . Si se presionó Delete, se llama a \ :java:ref:`deletefile`\ . Si se presionó Share, se llama a \ :java:ref:`shareFile`\ .

   :param savedInstanceState:
   :return: El diálogo en cuestión

shareFile
^^^^^^^^^

.. java:method:: public void shareFile(String filename)
   :outertype: DialogoArchivos

   Inicia \ :java:ref:`ShareFileActivity`\ .

   :param filename: El nombre del archivo que se desea compartir/descompartir.

