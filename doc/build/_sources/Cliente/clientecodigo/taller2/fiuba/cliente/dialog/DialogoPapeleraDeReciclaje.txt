.. java:import:: android.app AlertDialog

.. java:import:: android.app Dialog

.. java:import:: android.app DialogFragment

.. java:import:: android.content DialogInterface

.. java:import:: android.os Bundle

.. java:import:: android.widget Toast

.. java:import:: java.util.logging Logger

.. java:import:: taller2.fiuba.cliente.activity RecycleBinActivity

.. java:import:: taller2.fiuba.cliente.model Request

DialogoPapeleraDeReciclaje
==========================

.. java:package:: taller2.fiuba.cliente.dialog
   :noindex:

.. java:type:: public class DialogoPapeleraDeReciclaje extends DialogFragment

   Dialogo con una opcion: restore.

Methods
-------
onCreateDialog
^^^^^^^^^^^^^^

.. java:method:: @Override public Dialog onCreateDialog(Bundle savedInstanceState)
   :outertype: DialogoPapeleraDeReciclaje

   Dialogo de la actividad correspondiente a la papelera de reciclaje

   :param savedInstanceState:
   :return: Dialogo con un unico elemento, Restore

