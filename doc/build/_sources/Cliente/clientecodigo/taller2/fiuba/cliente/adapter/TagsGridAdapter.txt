.. java:import:: android.content Context

.. java:import:: android.view LayoutInflater

.. java:import:: android.view View

.. java:import:: android.view ViewGroup

.. java:import:: android.widget BaseAdapter

.. java:import:: android.widget ImageView

.. java:import:: android.widget TextView

.. java:import:: taller2.fiuba.cliente R

TagsGridAdapter
===============

.. java:package:: taller2.fiuba.cliente.adapter
   :noindex:

.. java:type:: public class TagsGridAdapter extends BaseAdapter

   Clase adapter para los tags Determina cómo se van a mostrar.

Constructors
------------
TagsGridAdapter
^^^^^^^^^^^^^^^

.. java:constructor:: public TagsGridAdapter(Context context, String[] tags)
   :outertype: TagsGridAdapter

   Constructor, inicializa las variables \ :java:ref:`context`\  y \ :java:ref:`tags`\ .

   :param context: El contexto de la actividad
   :param tags: La lista de tags

Methods
-------
getCount
^^^^^^^^

.. java:method:: @Override public int getCount()
   :outertype: TagsGridAdapter

getItem
^^^^^^^

.. java:method:: @Override public Object getItem(int position)
   :outertype: TagsGridAdapter

getItemId
^^^^^^^^^

.. java:method:: @Override public long getItemId(int position)
   :outertype: TagsGridAdapter

getView
^^^^^^^

.. java:method:: public View getView(int position, View convertView, ViewGroup parent)
   :outertype: TagsGridAdapter

   Establece el texto del tag y el ícono para borrarlo.

   :param position: Posición del tag
   :param convertView:
   :param parent:

