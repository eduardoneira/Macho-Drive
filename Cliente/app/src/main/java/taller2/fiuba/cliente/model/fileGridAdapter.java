package taller2.fiuba.cliente.model;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import taller2.fiuba.cliente.R;

/**
 * Adaptador para la grilla de archivos.
 * Establece el ícono de los archivos y sus nombres.
 */
public class fileGridAdapter extends BaseAdapter {
    private Context context;
    private final String[] files;

    /**
     * Contructor, inicializa las variables {@link #context} y {@link #files}.
     * @param context Contexto actual
     * @param files Lista de archivos a ser mostrados
     */
    public fileGridAdapter(Context context, String[] files) {
        this.context = context;
        this.files = files;
    }

    /**
     * Establece el ícono y el texto del archivo en cuestión
     * @param position posición del archivo
     * @param convertView
     * @param parent
     * @return
     */
    public View getView(int position, View convertView, ViewGroup parent) {

        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View gridView;

        if (convertView == null) {

            gridView = new View(context);

            gridView = inflater.inflate(R.layout.files, null);

            TextView textView = (TextView) gridView.findViewById(R.id.label);

            textView.setText(files[position]);

            ImageView icon = (ImageView) gridView .findViewById(R.id.icon);

            String mobile = files[position];

            icon.setImageResource(R.drawable.file);


        } else {
            gridView = (View) convertView;
        }

        return gridView;
    }

    @Override
    public int getCount() {
        return files.length;
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

}