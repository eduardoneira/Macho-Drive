package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
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
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_modify_file);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        ((EditText)findViewById(R.id.filename)).setText(filename);
        actualizarTags();
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
        addTag(((EditText)findViewById(R.id.tagToAdd)).getText().toString());
    }

    /**
     * Pide al server que agregue el tag solicitado.
     * Llama a {@link #actualizarTags()}
     * @param tag El tag que se quiere agregar
     */
    protected void addTag(String tag){
        try {
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
        } catch (JSONException e){}
    }

    /**
     * Pide al server la lista de tags del archivo y la muestra.
     */
    protected void actualizarTags(){
        Request getfile = new Request("GET", "/files/"+username+"/"+filename);
        getfile.setHeader("conn_token", token);
        JSONObject response = getfile.send();
        try {
            JSONArray tagsJson = response.getJSONArray("tags");
            tags = new ArrayList();
            for (int i = 0; i < tagsJson.length() ;i++){
                try {
                    String next = tagsJson.getString(i);
                    System.out.println(next); // Debug
                    tags.add(next);
                } catch(JSONException e){}
            }
            tagsGrid = (GridView) findViewById(R.id.tagsGrid);
            if (tags != null) {
                tagsGrid.setAdapter(new TagsGridAdapter(this, tags.toArray(new String[tags.size()])));
            } else {
                tagsGrid.setAdapter(new TagsGridAdapter(this, null));
            }

        } catch ( JSONException e){}
    }

    /**
     * Pide al server que elimine el tag solicitado y actualiza la lista de tags.
     * @param tag El tag a ser eliminado
     */
    public void deleteTag(final String tag){
        System.out.println("delete tag pressed");
        new AlertDialog.Builder(this)
                .setTitle("Delete tag")
                .setMessage("Are you sure you want to delete this tag?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
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
                        } catch (JSONException e){}
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
        try {
            String newFilename = ((EditText) findViewById(R.id.filename)).getText().toString();
            JSONObject data = new JSONObject();
            data.put("owner_username", username);
            data.put("filename_change", newFilename);
            data = agregarUbicacion(data);
            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
            request.setHeader("conn_token", token);
            request.send();
            filename = newFilename;
            Toast.makeText(getApplicationContext(), "Successfully renamed", Toast.LENGTH_SHORT).show();
        } catch(JSONException e){}
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
