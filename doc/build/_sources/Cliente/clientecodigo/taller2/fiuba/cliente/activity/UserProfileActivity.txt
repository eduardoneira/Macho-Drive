.. java:import:: android.graphics Bitmap

.. java:import:: android.graphics BitmapFactory

.. java:import:: android.os Bundle

.. java:import:: android.support.v7.app AppCompatActivity

.. java:import:: android.util Base64

.. java:import:: android.view View

.. java:import:: android.widget ImageView

.. java:import:: android.widget TextView

.. java:import:: android.widget Toast

.. java:import:: org.json JSONException

.. java:import:: org.json JSONObject

.. java:import:: taller2.fiuba.cliente R

.. java:import:: taller2.fiuba.cliente.model Request

UserProfileActivity
===================

.. java:package:: taller2.fiuba.cliente.activity
   :noindex:

.. java:type:: public class UserProfileActivity extends AppCompatActivity

   Actividad de busqueda de perfiles.

Methods
-------
Search
^^^^^^

.. java:method:: public void Search(View view)
   :outertype: UserProfileActivity

   Busca el usuario ingresado y muestra su perfil. En caso de no existir, se informa al usuario por pantalla. Setea las variables \ :java:ref:`name`\ , \ :java:ref:`email`\ , \ :java:ref:`ubicacion`\  y \ :java:ref:`picture`\

   :param view:

onCreate
^^^^^^^^

.. java:method:: @Override protected void onCreate(Bundle savedInstanceState)
   :outertype: UserProfileActivity

   Constructor de la actividad de buscador de perfiles. Inicializa la variable \ :java:ref:`token`\ .

   :param savedInstanceState:

