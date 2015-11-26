package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.adapter.TagsGridAdapter;
import taller2.fiuba.cliente.model.Permissions;
import taller2.fiuba.cliente.model.Request;

/**
 * Actividad de modificacion de nombre de archivo y tags.
 */
public class ModifyFileActivity extends AppCompatActivity {

    private String token, filename, username;
    /**
     * Tags del archivo
     */
    private List<String> tags;
    /**
     * Grilla de tags
     */
    GridView tagsGrid;
    private Location ubicacionLoc;
    private String ubicacion;
    /**
     * Variable encargada de ir actualizando la posicion actual.
     */
    private final LocationListener mLocationListener = new LocationListener() {
        @Override
        public void onStatusChanged(String s, int i, Bundle b){}
        public void onProviderEnabled(String s){}
        public void onProviderDisabled(String s){}
        @Override
        public void onLocationChanged(final Location location) {
            ubicacionLoc = location;
            ubicacion = String.valueOf(location.getLatitude()) + " " + String.valueOf(location.getLongitude());
        }
    };
    /**
     * Variable encargada de proveer datos sobre la posicion actual.
     */
    private LocationManager mLocationManager;

    /**
     * Constructor de la actividad de modificacion de archivos.
     * Inicializa las variables {@link #token}, {@link #filename} y {@link #username}.
     * Muestra el nombre del archivo.
     * Inicializa la lista de tags.
     * Crea el listener para cuando se quiere eliminar un archivo.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("ModifyFileActivity", "Se crea la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_modify_file);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        ((EditText)findViewById(R.id.filename)).setText(filename);
        if(!actualizarTags()){
            return;
        }
        tagsGrid.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                deleteTag(tags.get(position));

            }
        });
        mLocationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000,
                    50, mLocationListener);
        } catch (SecurityException e){}


    }

    /**
     * Metodo llamado al clickear el boton de agregar tag
     * Llama a {@link #addTag(String)}
     * @param view
     */
    public void addTagButton(View view){
        Log.d("ModifyFileActivity", "Se presiono el boton Add Tag");
        addTag(((EditText) findViewById(R.id.tagToAdd)).getText().toString());
    }

    /**
     * Pide al server que agregue el tag solicitado.
     * Llama a {@link #actualizarTags()}
     * @param tag El tag que se quiere agregar
     */
    protected void addTag(String tag){
        try {
            tag = tag.replaceAll(" ", "_");
            Log.d("ModifyFileActivity", "Se quiere agregar el tag "+tag);
            JSONObject data = new JSONObject();
            JSONArray tagAagregar = new JSONArray();
            tagAagregar.put(tag);
            data.put("owner_username", username);
            data.put("tags_add", tagAagregar);
            data = agregarUbicacion(data);
            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
            request.setHeader("conn_token", token);
            request.send();
            actualizarTags();
        } catch (Exception e){
            Log.d("ModifyFileActivity", "Error en el request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Pide al server la lista de tags del archivo y la muestra.
     */
    protected boolean actualizarTags(){
        Log.d("ModifyFileActivity", "Se actualiza la lista de tags");

        try {

            Request getfile = new Request("GET", "/files/"+username+"/"+filename+"/metadata");
            getfile.setHeader("conn_token", token);
            JSONObject response = getfile.send();

            if(getfile.getStatusCode() != HttpURLConnection.HTTP_OK){
                Log.d("ModifyFileActivity", "Algo se corrompio, el archivo esta en la lista del usuario pero no funciona el GET");
                Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
                finish();
                return false;
            }

            JSONArray tagsJson = response.getJSONArray("tags");
            tags = new ArrayList();
            for (int i = 0; i < tagsJson.length() ;i++){
                try {
                    String next = tagsJson.getString(i);
                    Log.d("ModifyFileActivity", "Se recibio el tag " + next);
                    tags.add(next);
                } catch(JSONException e){}
            }
            tagsGrid = (GridView) findViewById(R.id.tagsGrid);
            if (tags != null) {
                tagsGrid.setAdapter(new TagsGridAdapter(this, tags.toArray(new String[tags.size()])));
            } else {
                Log.d("ModifyFileActivity", "No se recibieron tags");
                tagsGrid.setAdapter(new TagsGridAdapter(this, null));
            }
            return true;

        } catch ( Exception e){
            Log.d("ModifyFileActivity", "Error en el request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
            return false;
        }
    }

    /**
     * Pide al server que elimine el tag solicitado y actualiza la lista de tags.
     * @param tag El tag a ser eliminado
     */
    public void deleteTag(final String tag){
        Log.d("ModifyFileActivity", "Se presiono el boton Delete Tag");
        new AlertDialog.Builder(this)
                .setTitle("Delete tag")
                .setMessage("Are you sure you want to delete this tag?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            Log.d("ModifyFileActivity", "El usuario esta seguro de querer eliminar el tag " + tag);
                            JSONObject data = new JSONObject();
                            JSONArray tagAEliminar = new JSONArray();
                            tagAEliminar.put(tag);
                            data.put("tags_delete", tagAEliminar);
                            data.put("owner_username", username);
                            data = agregarUbicacion(data);
                            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
                            request.setHeader("conn_token", token);
                            request.send();
                            actualizarTags();
                        } catch (Exception e){
                            Log.d("ModifyFileActivity", "Error en el request");
                            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
                        }
                    }
                })
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {}
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();

    }

    /**
     * Renombra el archivo segun el nombre que el usuario ingreso
     * @param view
     */
    public void rename(View view){
        Log.d("ModifyFileActivity", "Se presiono el boton Rename");
        try {
            String newFilename = ((EditText) findViewById(R.id.filename)).getText().toString();
            Log.d("ModifyFileActivity", "El nuevo nombre del archivo es " + newFilename);
            JSONObject data = new JSONObject();
            data.put("owner_username", username);
            data.put("filename_change", newFilename);
            data = agregarUbicacion(data);
            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
            request.setHeader("conn_token", token);
            JSONObject response = request.send();
            //Toast.makeText(getApplicationContext(), "Successfully renamed", Toast.LENGTH_SHORT).show();
            Log.d("ModifyFileActivity", "Se recibio status " + response.getString("status"));
            Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
            if(request.getStatusCode() == HttpURLConnection.HTTP_OK){
                filename = newFilename;
                Log.d("ModifyFileActivity", "Se renombro exitosamente");
            } else {
                Log.d("ModifyFileActivity", "No se pudo renombrar");
            }
        } catch(Exception e){
            Log.d("ModifyFileActivity", "Error en el request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Pide la ubicacion actual y la agrega a data.
     * En caso de no tener GPS, devuelve un string vacio.
     *
     * @param data El JSONObject al que se le quiere agregar la ubicacion
     * @return data con la ubicacion agregada
     */
    public JSONObject agregarUbicacion(JSONObject data){
        Permissions.verifyLocationPermissions(this);
        try {
            ubicacionLoc = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if (ubicacionLoc != null) {
                ubicacion = String.valueOf(ubicacionLoc.toString());
            }
            data.put("ubicacion", ubicacion);
        } catch (Exception e){}
        return data;
    }
}
