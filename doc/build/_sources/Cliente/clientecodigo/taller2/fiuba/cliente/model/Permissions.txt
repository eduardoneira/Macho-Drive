.. java:import:: android Manifest

.. java:import:: android.app Activity

.. java:import:: android.content.pm PackageManager

.. java:import:: android.support.v4.app ActivityCompat

Permissions
===========

.. java:package:: taller2.fiuba.cliente.model
   :noindex:

.. java:type:: public class Permissions

Methods
-------
verifyLocationPermissions
^^^^^^^^^^^^^^^^^^^^^^^^^

.. java:method:: public static void verifyLocationPermissions(Activity activity)
   :outertype: Permissions

verifyStoragePermissions
^^^^^^^^^^^^^^^^^^^^^^^^

.. java:method:: public static void verifyStoragePermissions(Activity activity)
   :outertype: Permissions

   Chequea si la aplicación tiene permiso para escribir el almacenamiento externo. Si la aplicación no tiene permiso, se le pide al usuario que lo conceda.

   :param activity:

