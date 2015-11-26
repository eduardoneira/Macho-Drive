package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.drawable.AnimationDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.dialog.DialogoPapeleraDeReciclaje;
import taller2.fiuba.cliente.adapter.FileGridAdapter;

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
        Log.d("RecycleBinActivity", "Se creo la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recycle_bin);

        ImageView i = (ImageView)findViewById(R.id.machamp);
        i.setBackgroundResource(R.drawable.machamp);

        AnimationDrawable pro = (AnimationDrawable)i.getBackground();
        pro.start();

        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        actualizarArchivosEnPapelera();
        //Si se clickea un archivo, se abre un dialogo
        grillaDeArchivosEnPapelera.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                DialogoPapeleraDeReciclaje diag = new DialogoPapeleraDeReciclaje();
                Bundle filename = new Bundle();
                Log.d("RecycleBinActivity", "El usuario selecciono una opcion");
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
        Log.d("RecycleBinActivity", "Se presiono Empty Recycle Bin");
        new AlertDialog.Builder(this)
                .setTitle("Empty Recycle Bin")
                .setMessage("Are you sure you want to empty the Recycle Bin?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            Log.d("RecycleBinActivity", "El usuario esta seguoro de querer vaciar la papelera");
                            Request request = new Request("DELETE", "/files/" + username + "/recycle_bin/");
                            request.setHeader("conn_token", token);
                            request.send();
                            Log.d("RecycleBinActivity", "Se vacio la papelera");
                            actualizarArchivosEnPapelera();
                        } catch (Exception e){
                            Log.d("RecycleBinActivity", "Error en el request");
                            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
                        }
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
        try {
            Log.d("RecycleBinActivity", "Se pide la lista de archivos en la papelera");
            Request request = new Request("GET", "/files/"+username+"/recycle_bin/");
            request.setHeader("conn_token", token);
            JSONObject response = request.send();
            JSONArray availableFiles = new JSONArray();

            JSONArray myFiles = response.getJSONArray("files_in_bin");
            for(int i = 0; i < myFiles.length(); i++){
                Log.d("RecycleBinActivity", "Se recibio el archivo " + myFiles.get(i));
                availableFiles.put(availableFiles.length(), myFiles.get(i));
            }
            return availableFiles;
        } catch (Exception e){
            Log.d("RecycleBinActivity", "Error en el request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
        return new JSONArray();
    }

    /**
     * Actualiza {@link #archivosEnPapelera}
     * Actualiza la lista de archivos mostrada en pantalla.
     */
    public void actualizarArchivosEnPapelera(){
        Log.d("RecycleBinActivity", "Se actualiza la lista de archivos en papelera");
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
