package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.DialogoPapeleraDeReciclaje;
import taller2.fiuba.cliente.model.FileGridAdapter;

/**
 * Actividad de papelera de reciclaje.
 * Permite ver, restaurar y suprimir archivos eliminados.
 */
public class RecycleBinActivity extends AppCompatActivity {

    private String token, username;
    GridView grillaDeArchivosEnPapelera;
    static List<String> archivosEnPapelera = new ArrayList();

    /**
     * Constructor de la actividad de papelera de reciclaje.
     * Llama a {@link #actualizarArchivosEnPapelera()}
     * Inicializa las variables {@link #token} y {@link #username}.
     * Inicializa el listener para cuando se clickea un archivo.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recycle_bin);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        //Se pide una lista de los archivos del usuario al server
        actualizarArchivosEnPapelera();
        //Si se clickea un archivo, se abre un dialogo
        grillaDeArchivosEnPapelera.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                DialogoPapeleraDeReciclaje diag = new DialogoPapeleraDeReciclaje();
                Bundle filename = new Bundle();
                //El usuario selecciona una opcion
                filename.putString("filename", archivosEnPapelera.get(position));
                diag.setArguments(filename);
                diag.show(getFragmentManager(), "");

            }
        });
    }

    /**
     * Primero pregunta al usuario si esta seguro de querer vaciar la papelera.
     * En caso afirmativo, pide al server que lo haga y vacia la lista {@link #archivosEnPapelera}.
     * Llama a {@link #actualizarArchivosEnPapelera()}.
     * @param view
     */
    public void vaciarPapelera(View view){
        new AlertDialog.Builder(this)
                .setTitle("Empty Recycle Bin")
                .setMessage("Are you sure you want to empty the Recycle Bin?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Request request = new Request("DELETE", "/files/"+ username + "/recycle_bin/");
                        request.setHeader("conn_token", token);
                        request.send();
                        actualizarArchivosEnPapelera();
                    }
                })
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
    }

    public JSONArray listarArchivosEnPapelera(){
        Request request = new Request("GET", "/files/"+username+"/recycle_bin/");
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        JSONArray availableFiles = new JSONArray();
        try {
            JSONArray myFiles = response.getJSONArray("files_in_bin");
            for(int i = 0; i < myFiles.length(); i++){
                availableFiles.put(availableFiles.length(), myFiles.get(i));
            }
        } catch (JSONException e){}
        return availableFiles;
    }

    /**
     * Actualiza {@link #archivosEnPapelera}
     * Actualiza la lista de archivos mostrada en pantalla.
     */
    public void actualizarArchivosEnPapelera(){
        archivosEnPapelera = new ArrayList();
        JSONArray files = listarArchivosEnPapelera();
        for (int i = 0; i < files.length() ;i++){
            try {
                String next = files.getString(i);
                archivosEnPapelera.add(next);
            } catch(JSONException e){}
        }
        grillaDeArchivosEnPapelera = (GridView) findViewById(R.id.gridView);
        if (archivosEnPapelera != null) {
            grillaDeArchivosEnPapelera.setAdapter(new FileGridAdapter(this, archivosEnPapelera.toArray(new String[archivosEnPapelera.size()])));
        } else {
            grillaDeArchivosEnPapelera.setAdapter(new FileGridAdapter(this, null));
        }
    }
}
