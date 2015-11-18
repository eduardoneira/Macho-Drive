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
 * Clase adapter para los tags
 * Determina cómo se van a mostrar.
 */
public class TagsGridAdapter extends BaseAdapter {
    private Context context;
    private final String[] tags;

    /**
     * Constructor, inicializa las variables {@link #context} y {@link #tags}.
     * @param context El contexto de la actividad
     * @param tags La lista de tags
     */
    public TagsGridAdapter(Context context, String[] tags) {
        this.context = context;
        this.tags = tags;
    }

    /**
     * Establece el texto del tag y el ícono para borrarlo.
     * @param position Posición del tag
     * @param convertView
     * @param parent
     * @return
     */
    public View getView(int position, View convertView, ViewGroup parent) {

        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View gridView;

        if (convertView == null) {

            gridView = inflater.inflate(R.layout.tags, null);

            TextView textView = (TextView) gridView.findViewById(R.id.label);

            textView.setText(tags[position]);

            ImageView icon = (ImageView) gridView .findViewById(R.id.deleteTag);

            icon.setImageResource(R.drawable.redcross);


        } else {
            gridView = (View) convertView;
        }

        return gridView;
    }

    @Override
    public int getCount() {
        return tags.length;
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