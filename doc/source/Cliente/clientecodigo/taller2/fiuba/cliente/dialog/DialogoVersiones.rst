.. java:import:: android.app Activity

.. java:import:: android.app AlertDialog

.. java:import:: android.app Dialog

.. java:import:: android.app DialogFragment

.. java:import:: android.content DialogInterface

.. java:import:: android.os Bundle

.. java:import:: android.util Base64

.. java:import:: org.json JSONArray

.. java:import:: org.json JSONObject

.. java:import:: java.io File

.. java:import:: java.io FileOutputStream

.. java:import:: java.util.logging Logger

.. java:import:: taller2.fiuba.cliente.activity FileVersionsActivity

.. java:import:: taller2.fiuba.cliente.model Request

.. java:import:: taller2.fiuba.cliente.model Permissions

DialogoVersiones
================

.. java:package:: taller2.fiuba.cliente.dialog
   :noindex:

.. java:type:: public class DialogoVersiones extends DialogFragment

   Dialogo de activitada de versiones. Tiene dos elementos: Download y Delete.

Methods
-------
deleteVersion
^^^^^^^^^^^^^

.. java:method:: public void deleteVersion(String username, int version)
   :outertype: DialogoVersiones

   Se elimina la version seleccionada. Primero se le pregunta al usuario si esta seguro. Actualiza las versiones mostradas en la actividad madre.

   :param username:
   :param version:

downloadVersion
^^^^^^^^^^^^^^^

.. java:method:: public void downloadVersion(String contenido, String filename)
   :outertype: DialogoVersiones

   Se descarga la version seleccionada. Primero se le pregunta al usuario si esta seguro.

   :param contenido: Contenido de la version que se desea descargar
   :param filename: Nombre del archivo a descargar

onCreateDialog
^^^^^^^^^^^^^^

.. java:method:: @Override public Dialog onCreateDialog(Bundle savedInstanceState)
   :outertype: DialogoVersiones

   Se crea el dialogo para \ :java:ref:`FileVersionsActivity`\  Se llama a \ :java:ref:`downloadVersion(String,String)`\  o a \ :java:ref:`deleteVersion(String,int)`\

   :param savedInstanceState:

