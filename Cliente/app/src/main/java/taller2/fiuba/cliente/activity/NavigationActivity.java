package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.dialog.DialogoArchivos;
import taller2.fiuba.cliente.adapter.FileGridAdapter;
import taller2.fiuba.cliente.model.Permissions;

/**
 * Actividad principal. Muestra los archivos a los que el usuario tiene acceso.
 * Es la primera actividad que ve el usuario al conectarse al sistema.
 */
public class NavigationActivity extends AppCompatActivity {

    /**
     * Codigo resultado de elegir un archivo en el navegador. {@link #uploadFile(String)}
     */
    private static final int PICKFILE_RESULT_CODE = 101;
    /**
     * Codigo resultado de {@link ProfileSettingsActivity}
     */
    private static final int PROFILE_SETTINGS_RESULT_CODE = 105;
    private String token, username;
    /**
     * Grilla de archivos del usuario
     */
    GridView gridView;
    /**
     * Lista de archivos del usuario
     */
    static List<String> archivos = new ArrayList();
    /**
     * Selector de criterio de busqueda avanzada
     */
    private Spinner spinner;
    /**
     * Criterios posibles para la busqueda avanzada
     */
    private static final String[] criterios = {"Name", "Owner", "Tag", "Extension"};
    private String ubicacion;
    /**
     * Ubicacion del usuario en otro formato.
     */
    private Location ubicacionLoc;
    /**
     * Variable encargada de ir actualizando la posición actual.
     * Actualiza las variables {@link #ubicacion} y {@link #ubicacionLoc}
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
     * Variable encargada de proveer datos sobre la posición actual.
     */
    private LocationManager mLocationManager;

    /**
     * Constructor de la actividad principal.
     * Inicializa las variables {@link #token} y {@link #username}.
     * Inicializa {@link #archivos}
     * Llama a {@link #actualizarArchivos()}
     * Inicializa el listener para que los archivos puedan ser clickeados. {@link DialogoArchivos}
     * Inicializa el buscador.
     *
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        Log.d("NavigationActivity", "Se crea la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_navigation);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        actualizarArchivos();

        gridView.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                DialogoArchivos diag = new DialogoArchivos();
                Bundle filename = new Bundle();
                Log.d("NavigationActivity", "Se creo un dialogo");
                filename.putString("filename", archivos.get(position));
                diag.setArguments(filename);
                diag.show(getFragmentManager(), "");

            }
        });


        ImageView lupa = (ImageView) findViewById(R.id.lupa);
        lupa.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                actualizarArchivos();
                try {
                    Log.d("NavigationActivity", "Se clickeo la lupa");
                    String busqueda = ((EditText) findViewById(R.id.searchBar)).getText().toString();
                    String key = ((String) spinner.getSelectedItem()).toUpperCase();
                    Request request = new Request("GET", "/files/" + username + "/search/" + key + "/" + busqueda);
                    request.setHeader("conn_token", token);
                    Log.d("NavigationActivity", "Se desea buscar "+ busqueda + " por el campo " + key);
                    JSONObject response = request.send();
                    JSONArray resultados = response.getJSONArray("search_result");
                    archivos = new ArrayList<String>();
                    for(int i = 0; i < resultados.length() ; i++){
                        Log.d("NavigationActivity", "Se recibio el archivo " + resultados.getString(i));
                        archivos.add(resultados.getString(i));
                    }
                    gridView = (GridView) findViewById(R.id.gridView);
                    if (archivos != null) {
                        gridView.setAdapter(new FileGridAdapter(getApplicationContext(), archivos.toArray(new String[archivos.size()])));
                    } else {
                        gridView.setAdapter(new FileGridAdapter(getApplicationContext(), null));
                    }
                    Log.d("NavigationActivity", "Se completo la busqueda exitosamente");
                } catch (JSONException e){}
            }
        });

        spinner = (Spinner)findViewById(R.id.spinner);
        ArrayAdapter<String>adapter = new ArrayAdapter<String>(NavigationActivity.this,
                android.R.layout.simple_spinner_item, criterios);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);

        mLocationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000,
                    50, mLocationListener);
        } catch (SecurityException e){}

    }

    /**
     * Al resumir la actividad se actualiza la lista de archivos. {@link #actualizarArchivos()}
     */
    @Override
    public void onResume(){
        Log.d("NavigationActivity", "Se resume la actividad");
        actualizarArchivos();
        super.onResume();
    }


    /**
     * Metodo que crea el menu cuando se presiona el boton superior derecho.
     * @param menu El menu a crear
     * @return Si salio bien
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.d("NavigationActivity", "Se abre el menu");
        getMenuInflater().inflate(R.menu.menu_navigation, menu);
        return true;
    }


    /**
     * Método que responde cuando se clickea un item en el menú.
     * Si se presionó el botón UP, se desconecta del sistema y vuelve a la actividad inicial. {@link #logOut()}
     * Si se presionó Profile Settings, se abre {@link ProfileSettingsActivity}.
     * Si se presionó Search User, se abre {@link UserProfileActivity}
     * Si se presionó Upload File, se abre una ventana de selección de archivo para ser subido.
     * Si se presionó Deleted Files, se abre {@link RecycleBinActivity}.
     * @param item El item presionado
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.profile_settings) {
            Log.d("NavigationActivity", "Se selecciono Profile Settings");
            Intent profileSettingsIntent = new Intent(getApplicationContext(), ProfileSettingsActivity.class);
            profileSettingsIntent.putExtra("token", token);
            profileSettingsIntent.putExtra("username", username);
            startActivityForResult(profileSettingsIntent, PROFILE_SETTINGS_RESULT_CODE);
            return true;
        }
        if(id == R.id.search_user_profile){
            Log.d("NavigationActivity", "Se selecciono Search User Profile");
            Intent searchUserProfileIntent = new Intent(getApplicationContext(), UserProfileActivity.class);
            searchUserProfileIntent.putExtra("token", token);
            searchUserProfileIntent.putExtra("username", username);
            startActivity(searchUserProfileIntent);
            return true;

        }
        if (id == android.R.id.home){ //Boton UP (flecha arriba a la izquierda)
            Log.d("NavigationActivity", "Se presiono el boton Up");
            this.logOut();
        }
        if (id == R.id.upload_file){
            Log.d("NavigationActivity", "Se selecciono Upload File");
            Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
            fileintent.addCategory(Intent.CATEGORY_OPENABLE);
            fileintent.setType("*/*"); //Este intent es un navegador de archivos
            try {
                startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
            } catch (ActivityNotFoundException e) {}
            return true;
        }
        if (id == R.id.deleted_files){
            Log.d("NavigationActivity", "Se selecciono Deleted Files");
            Intent recycleBinIntent = new Intent(getApplicationContext(), RecycleBinActivity.class);
            recycleBinIntent.putExtra("token", token);
            recycleBinIntent.putExtra("username", username);
            startActivity(recycleBinIntent);
        }
        return super.onOptionsItemSelected(item);
    }


    /**
     * Al presionar el boton Back, se desloggea del sistema.
     * Llama a {@link #logOut()}
     */
    @Override
    public void onBackPressed() { //Boton BACK (triangulo abajo a la izquierda)
        Log.d("NavigationActivity", "Se presiono el boton Back");
        new AlertDialog.Builder(this)
                .setMessage("Are you sure you want to log out?")
                .setNegativeButton(android.R.string.no, null)
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {

                    public void onClick(DialogInterface arg0, int arg1) {
                        logOut();
                        NavigationActivity.super.onBackPressed();
                    }
                }).create().show();
    }

    /**
     * Metodo que maneja la finalizacion de actividades.
     * Si se eligio un archivo para ser subido, se llama a {@link #uploadFile}.
     * Si se elimino el usuario en {@link ProfileSettingsActivity}, se vuelve a {@link MainActivity}.
     * @param requestCode Código de la actividad iniciada.
     * @param resultCode Código resultado de la actividad.
     * @param data Datos resultados de la actividad.
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    if(data != null) {
                        Log.d("NavigationActivity", "Se selecciono un archivo valido");
                        Uri FilePath = data.getData();
                        uploadFile(FilePath.getPath());
                    }
                }
                return ;
            case PROFILE_SETTINGS_RESULT_CODE:
                if (resultCode == -1){
                    Log.d("NavigationActivity", "Se elimino el usuario");
                    finish();
                }
        }
    }

    /**
     * Pide al server la lista de archivos a los que el usuario tiene acceso.
     * Tanto propios como compartidos.
     *
     * @return Lista de archivos a los que el usuario puede acceder.
     */
    public JSONArray listFiles(){
        Log.d("NavigationActivity", "Se pide la lista de archivos");
        Request request = new Request("GET", "/files/"+username+"/");
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        JSONArray availableFiles = new JSONArray();
        try {
            JSONArray myFiles = response.getJSONArray("my_file_tokens");
            JSONObject sharedFiles = response.getJSONObject("shared_file_tokens");
            for(int i = 0; i < myFiles.length(); i++){
                Log.d("NavigationActivity", "Se recibio "+myFiles.get(i));
                availableFiles.put(availableFiles.length(), myFiles.get(i));
            }
            Iterator<String> it = sharedFiles.keys();
            while(it.hasNext()){
                String next = sharedFiles.getString(it.next());
                Log.d("NavigationActivity", "Se recibio "+next);
                availableFiles.put(availableFiles.length(), next);
            }

        } catch (JSONException e){}
        return availableFiles;
    }

    /**
     * Se encodea en Base 64 el archivo en el path indicado y se pide al server que lo suba.
     * Se chequean los permisos necesarios. Se piden al usuario en caso de que falten. {@link Permissions}
     * Se actualizan {@link #ubicacionLoc} y {@link #ubicacion}
     * Llama a {@link #actualizarArchivos()}
     * @param path La ruta del archivo a subir
     */
    public void uploadFile(String path){
        Log.d("NavigationActivity", "Se inicia la subida de archivo");
        Log.d("NavigationActivity", "El path seleccionado es " + path);
        JSONObject data = new JSONObject();
        File file;
        String fname;
        FileInputStream fis;
        try {
            file = new File(path);
            fname = path;
            fis = new FileInputStream(file);
        } catch (Exception e){
            Log.d("NavigationActivity", "Estas en el emulador");
            try {
                file = new File(Environment.getExternalStorageDirectory().toString(), path.split(":")[1]);
                fis = new FileInputStream(file);
                fname = path.split(":")[1];
            } catch (Exception ex){
                ex.printStackTrace();
                file = null;
                fis = null;
                fname = "";
            }
        }
        try {
            int pos = fname.lastIndexOf("/");
            if (pos > 0) {
                fname = fname.substring(pos+1, fname.length());
            }
            data.put("username", username);
            data.put("filename", fname);
            Permissions.verifyStoragePermissions(this);
            byte[] arrayB = new byte[(int)file.length()];
            fis.read(arrayB);
            fis.close();
            data.put("content", new String(Base64.encode(arrayB, Base64.DEFAULT)));
            Permissions.verifyLocationPermissions(this);
            ubicacionLoc = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if (ubicacionLoc != null) {
                Log.d("NavigationActivity", "Se recibio ubicacion del usuario");
                ubicacion = String.valueOf(ubicacionLoc.getLatitude()) + " " + String.valueOf(ubicacionLoc.getLongitude());
            } else {
                Log.d("NavigationActivity", "No se recibio ubicacion del usuario");
            }
            data.put("ubicacion", ubicacion);

        } catch (JSONException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            Log.w("NavigationActivity", "Archivo invalido FNF");
            e.printStackTrace();
        } catch (IOException e) {
            Log.w("NavigationActivity", "Archivo invalido IOE");
            e.printStackTrace();
        } catch (SecurityException e){
            Log.w("NavigationActivity", "No se obtuvieron los permisos necesarios");
            e.printStackTrace();
        }
        Request request = new Request("POST", "/files/"+username+"/", data);
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        try {
            Log.d("NavigationActivity", "Se recibio status " + response.getString("status"));
            Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
        } catch (JSONException e) {
            Log.d("NavigationActivity", "La respuesta no contenia campo status ");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }

        Log.d("NavigationActivity", "Se subio el archivo exitosamente");
        actualizarArchivos();
    }

    /**
     * Se desconecta del sistema.
     */
    public void logOut(){
        Log.d("NavigationActivity", "Se desconecta del sistema (Log Out)");
        Request request = new Request("DELETE", "/sessions/"+username);
        request.setHeader("conn_token", token);
        request.send();
    }

    /**
     * Llama a {@link #listFiles()} y actualizan los archivos mostrados en la cuadricula.
     */
    public void actualizarArchivos(){
        Log.d("NavigationActivity", "Se actualiza la lista de archivos");
        archivos = new ArrayList();
        JSONArray files = listFiles();
        for (int i = 0; i < files.length() ;i++){
            try {
                String next = files.getString(i);
                Log.d("NavigationActivity", "Se recibio el archivo "+next);
                archivos.add(next);
            } catch(JSONException e){}
        }
        gridView = (GridView) findViewById(R.id.gridView);
        if (archivos != null) {
            gridView.setAdapter(new FileGridAdapter(this, archivos.toArray(new String[archivos.size()])));
        } else {
            gridView.setAdapter(new FileGridAdapter(this, null));
        }
    }


}
