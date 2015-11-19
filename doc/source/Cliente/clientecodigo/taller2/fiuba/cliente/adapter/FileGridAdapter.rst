.. java:import:: android.content Context

.. java:import:: android.view LayoutInflater

.. java:import:: android.view View

.. java:import:: android.view ViewGroup

.. java:import:: android.widget BaseAdapter

.. java:import:: android.widget ImageView

.. java:import:: android.widget TextView

.. java:import:: taller2.fiuba.cliente R

FileGridAdapter
===============

.. java:package:: taller2.fiuba.cliente.adapter
   :noindex:

.. java:type:: public class FileGridAdapter extends BaseAdapter

   Adaptador para la grilla de archivos. Establece el ícono de los archivos y sus nombres.

Constructors
------------
FileGridAdapter
^^^^^^^^^^^^^^^

.. java:constructor:: public FileGridAdapter(Context context, String[] files)
   :outertype: FileGridAdapter

   Contructor, inicializa las variables \ :java:ref:`context`\  y \ :java:ref:`files`\ .

   :param context: Contexto actual
   :param files: Lista de archivos a ser mostrados

Methods
-------
getCount
^^^^^^^^

.. java:method:: @Override public int getCount()
   :outertype: FileGridAdapter

getItem
^^^^^^^

.. java:method:: @Override public Object getItem(int position)
   :outertype: FileGridAdapter

getItemId
^^^^^^^^^

.. java:method:: @Override public long getItemId(int position)
   :outertype: FileGridAdapter

getView
^^^^^^^

.. java:method:: public View getView(int position, View convertView, ViewGroup parent)
   :outertype: FileGridAdapter

   Establece el ícono y el texto del archivo en cuestion

   :param position: posición del archivo
   :param convertView:
   :param parent:

