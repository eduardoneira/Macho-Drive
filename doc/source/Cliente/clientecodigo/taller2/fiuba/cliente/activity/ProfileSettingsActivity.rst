.. java:import:: android.app AlertDialog

.. java:import:: android.content ActivityNotFoundException

.. java:import:: android.content DialogInterface

.. java:import:: android.content Intent

.. java:import:: android.graphics Bitmap

.. java:import:: android.graphics BitmapFactory

.. java:import:: android.location Location

.. java:import:: android.location LocationListener

.. java:import:: android.location LocationManager

.. java:import:: android.net Uri

.. java:import:: android.os Environment

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.os Bundle

.. java:import:: android.util Base64

.. java:import:: android.view View

.. java:import:: android.widget EditText

.. java:import:: android.widget ImageView

.. java:import:: android.widget TextView

.. java:import:: android.widget Toast

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: java.io File

.. java:import:: java.io FileInputStream

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Permissions

.. java:import:: taller2.fiuba.cliente.model Request

ProfileSettingsActivity
=======================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class ProfileSettingsActivity extends AppCompatActivity

   Actividad de modificacion de perfil.

Methods
-------
changePicture
^^^^^^^^^^^^^

.. java:method:: public void changePicture(View view)
   :outertype: ProfileSettingsActivity

   Cambia la imagen de perfil del usuario (\ :java:ref:`picture`\ ) No se comunica con el server, el cambio solo persiste si se pide guardar los cambios.

   :param view:

deleteUser
^^^^^^^^^^

.. java:method:: public void deleteUser(View view)
   :outertype: ProfileSettingsActivity

   Pregunta al usuario si esta seguro de que quiere eliminar su cuenta. En caso afirmativo, pide al server que la elimine y vuelve a \ :java:ref:`NavigationActivity`\  con codigo -1.

   :param view:

onActivityResult
^^^^^^^^^^^^^^^^

.. java:method:: @Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
   :outertype: ProfileSettingsActivity

   Metodo encargado de manejar la finalizacion de actividades. Solo se llama al seleccionar una nueva imagen de perfil. Actualiza la imagen de perfil.

   :param requestCode: El código de la actividad que finalizó.
   :param resultCode: El código resultado.
   :param data: Los datos resultados.

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: ProfileSettingsActivity

   Constructor de la actividad de modificacion de perfil. Inicializa \ :java:ref:`username`\ , \ :java:ref:`token`\ ., \ :java:ref:`name`\ , \ :java:ref:`email`\ , \ :java:ref:`picture`\ , \ :java:ref:`ubicacion`\  Inicializa las variables \ :java:ref:`mLocationListener`\  y \ :java:ref:`mLocationManager`\  Pide al servidor la informacion del usuario y la muestra para ser modificada.

   :param savedInstanceState:

saveChanges
^^^^^^^^^^^

.. java:method:: public void saveChanges(View view)
   :outertype: ProfileSettingsActivity

   Lee la informacion ingresada por el usuario y la envía al server para que sea modificada. Se actualiza la \ :java:ref:`ubicacion`\

   :param view:

