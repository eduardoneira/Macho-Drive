package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.Spinner;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.dialogoArchivos;
import taller2.fiuba.cliente.model.dialogoRecycleBin;
import taller2.fiuba.cliente.model.fileGridAdapter;

public class RecycleBinActivity extends AppCompatActivity {

    private String token, username;
    GridView gridView;
    static List<String> archivos = new ArrayList();

    /**
     * Constructor de la actividad de papelera de reciclaje.
     * Muestra los archivos en papelera.
     * Inicializa las variables token y username.
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
        actualizarArchivos();
        //Si se clickea un archivo, se abre un dialogo
        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                dialogoRecycleBin diag = new dialogoRecycleBin();
                Bundle filename = new Bundle();
                //El usuario selecciona una opcion
                filename.putString("filename", archivos.get(position));
                diag.setArguments(filename);
                diag.show(getFragmentManager(),"");

            }
        });


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_recycle_bin, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * Pregunta al usuario si está seguro de querer vaciar la papelera.
     * En caso afirmativo, pide al server que lo haga y vacía la lista de archivos.
     * @param view
     */
    public void emptyRecycleBin(View view){
        new AlertDialog.Builder(this)
                .setTitle("Empty Recycle Bin")
                .setMessage("Are you sure you want to empty the Recycle Bin?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Request request = new Request("DELETE", "/files/"+ username + "/recycle_bin/");
                        request.setHeader("conn_token", token);
                        request.send();
                    }
                })
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
    }

    /**
     * Pide al server la lista de archivos en la papelera
     * @return Lista de archivos en la papelera
     */
    public JSONArray listFiles(){
        System.out.println("/files/" + username + "/");

        Request request = new Request("GET", "/files/"+username+"/recycle_bin/");
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        System.out.println(response);
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
     * Actualiza la lista de archivos en pantalla.
     */
    public void actualizarArchivos(){
        archivos = new ArrayList();
        JSONArray files = listFiles();
        for (int i = 0; i < files.length() ;i++){
            try {
                String next = files.getString(i);
                System.out.println(next); // Debug
                archivos.add(next);
            } catch(JSONException e){}
        }
        //Se muestran los archivos en una cuadricula
        gridView = (GridView) findViewById(R.id.gridView);
        if (archivos != null) {
            gridView.setAdapter(new fileGridAdapter(this, archivos.toArray(new String[archivos.size()])));
        } else {
            gridView.setAdapter(new fileGridAdapter(this, null));
        }
    }
}
