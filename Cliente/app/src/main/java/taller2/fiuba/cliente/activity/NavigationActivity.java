package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
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
import java.util.logging.Logger;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.dialogoArchivos;
import taller2.fiuba.cliente.model.fileGridAdapter;
import android.widget.AdapterView.OnItemSelectedListener;

public class NavigationActivity extends AppCompatActivity implements OnItemSelectedListener {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);
    private static final int PICKFILE_RESULT_CODE = 101;
    private static final int ADVANCED_SEARCH_CODE = 102;
    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    private static final int PROFILE_SETTINGS_RESULT_CODE = 105;
    private static final int PERMISSION_ACCESS_FINE_LOCATION = 106;
    private String token, username;
    GridView gridView;
    static List<String> archivos = new ArrayList();
    private Spinner spinner;
    private static final String[]paths = {"Name", "Owner", "Tag", "Extension"};
    private String ubicacion;
    private Location ubicacionLoc;
    /**
     * Variable encargada de ir actualizando la posición actual.
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
            System.out.println(ubicacion);

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
     * Inicializa el listener para que los archivos puedan ser clickeados.
     * Inicializa el buscador.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_navigation);
        System.out.println("NavigationActivity creada");
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        //Se pide una lista de los archivos del usuario al server
        actualizarArchivos();

        //Si se clickea un archivo, se abre un dialogo
        gridView.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                dialogoArchivos diag = new dialogoArchivos();
                Bundle filename = new Bundle();
                //El usuario selecciona una opcion
                filename.putString("filename", archivos.get(position));
                diag.setArguments(filename);
                diag.show(getFragmentManager(), "ss"); //Hay que sacar esto, era para debuggear

            }
        });


        ImageView lupa = (ImageView) findViewById(R.id.lupa);
        lupa.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                System.out.println("lupa clickeada");
                actualizarArchivos();
                try {
                    String busqueda = ((EditText) findViewById(R.id.searchBar)).getText().toString();
                    String key = ((String) spinner.getSelectedItem()).toUpperCase();
                    System.out.println(busqueda);
                    Request request = new Request("GET", "/files/" + username + "/search/" + key + "/" + busqueda);
                    request.setHeader("conn_token", token);
                    JSONObject response = request.send();
                    JSONArray resultados = response.getJSONArray("search_result");
                    archivos = new ArrayList<String>();
                    for(int i = 0; i < resultados.length() ; i++){
                        archivos.add(resultados.getString(i));
                    }
                    gridView = (GridView) findViewById(R.id.gridView);
                    if (archivos != null) {
                        gridView.setAdapter(new fileGridAdapter(getApplicationContext(), archivos.toArray(new String[archivos.size()])));
                    } else {
                        gridView.setAdapter(new fileGridAdapter(getApplicationContext(), null));
                    }
                } catch (JSONException e){
                    System.out.println("Error en la busqueda");
                }
            }
        });

        spinner = (Spinner)findViewById(R.id.spinner);
        ArrayAdapter<String>adapter = new ArrayAdapter<String>(NavigationActivity.this,
                android.R.layout.simple_spinner_item,paths);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        spinner.setOnItemSelectedListener(this);

        mLocationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000,
                    50, mLocationListener);
        } catch (SecurityException e){}

    }

    public void onNothingSelected(AdapterView<?> parent){}

    public void onItemSelected(AdapterView<?> parent, View v, int position, long id) {}

    /**
     * Al resumir la actividad se actualiza la lista de archivos.
     */
    @Override
    public void onResume(){
        System.out.println("resume");
        actualizarArchivos();
        super.onResume();
    }

    @Override
    public void onStop(){
        System.out.println("stop");
        super.onStop();
    }

    @Override
    public void onDestroy(){
        System.out.println("destroy");
        super.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_navigation, menu);
        return true;
    }


    /**
     * Método que responde cuando se clickea un item en el menú.
     * Si se presionó el botón UP, se desconecta del sistema y vuelve a la actividad inicial.
     * Si se presionó Profile Settings, se abre {@link ProfileSettingsActivity}.
     * Si se presionó Upload File, se abre una ventana de selección de archivo para ser subido.
     * Si se presionó Deleted Files, se abre {@link RecycleBinActivity}.
     * @param item El item presionado
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();


        //noinspection SimplifiableIfStatement
        if (id == R.id.profile_settings) {
            Intent profileSettingsIntent = new Intent(getApplicationContext(), ProfileSettingsActivity.class);
            profileSettingsIntent.putExtra("token", token);
            profileSettingsIntent.putExtra("username", username);
            startActivityForResult(profileSettingsIntent, PROFILE_SETTINGS_RESULT_CODE);
            return true;
        }
        if(id == R.id.search_user_profile){
            Intent searchUserProfileIntent = new Intent(getApplicationContext(), UserProfileActivity.class);
            searchUserProfileIntent.putExtra("token", token);
            searchUserProfileIntent.putExtra("username", username);
            startActivity(searchUserProfileIntent);
            return true;

        }
        if (id == android.R.id.home){ //Boton UP (flecha arriba a la izquierda)
            this.logOut();
        }
        if (id == R.id.upload_file){
            Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
            fileintent.addCategory(Intent.CATEGORY_OPENABLE);
            fileintent.setType("*/*"); //Este intent es un navegador de archivos
            try {
                startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
            } catch (ActivityNotFoundException e) {
            }
            return true;
        }
        if (id == R.id.deleted_files){
            Intent recycleBinIntent = new Intent(getApplicationContext(), RecycleBinActivity.class);
            recycleBinIntent.putExtra("token", token);
            recycleBinIntent.putExtra("username", username);
            startActivity(recycleBinIntent);
        }


        return super.onOptionsItemSelected(item);
    }


    /**
     * Al presionar el botón Back, se desloggea del sistema.
     */
    @Override
    public void onBackPressed() { //Boton BACK (triangulo abajo a la izquierda)
        this.logOut();
        super.onBackPressed();
    }

    /**
     * Metodo que maneja la finalización de actividades.
     * Si se eligió un archivo para ser subido, se llama a {@link #uploadFile}.
     * Si se eliminó el usuario en {@link ProfileSettingsActivity}, se vuelve a {@link MainActivity}.
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
                        Uri FilePath = data.getData();
                        System.out.println("picked file");
                        System.out.println(FilePath.toString());
                        System.out.println(FilePath.getPath());
                        uploadFile(FilePath.getPath());
                    }
                }
                return ;
            case ADVANCED_SEARCH_CODE:
                if (resultCode == RESULT_OK){
                    System.out.println("advanced search ok");
                } else {
                    System.out.println("error en advanced search");
                }
            case PROFILE_SETTINGS_RESULT_CODE:
                if (resultCode == -1){
                    finish();
                }
                return ;
            default :
                System.out.println("default");
        }
    }

    /**
     * Pide al server la lista de archivos a los que el usuario tiene acceso,
     * tanto propios como compartidos.
     *
     * @return Lista de archivos a los que el usuario puede acceder.
     */
    public JSONArray listFiles(){
        System.out.println("/files/" + username + "/");

        Request request = new Request("GET", "/files/"+username+"/");
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        JSONArray availableFiles = new JSONArray();
        try {
            JSONArray myFiles = response.getJSONArray("my_file_tokens");
            JSONObject sharedFiles = response.getJSONObject("shared_file_tokens");
            for(int i = 0; i < myFiles.length(); i++){
                availableFiles.put(availableFiles.length(), myFiles.get(i));
            }
            Iterator<String> it = sharedFiles.keys();
            while(it.hasNext()){
                availableFiles.put(availableFiles.length(), sharedFiles.getString(it.next()));
            }

        } catch (JSONException e){}
        return availableFiles;
    }

    /**
     * Se encodea en Base 64 el archivo en el path indicado
     * y se pide al server que lo suba.
     *
     * @param path La ruta del archivo a subir
     */
    public void uploadFile(String path){
        JSONObject data = new JSONObject();
        System.out.println(path);
        System.out.println(path.split(":")[1]);
        File file = new File(Environment.getExternalStorageDirectory().toString(), path.split(":")[1]);
        System.out.println(file.exists());

        try {
            String fname = path.split(":")[1];
            int pos = fname.lastIndexOf("/");
            if (pos > 0) {
                fname = fname.substring(pos+1, fname.length());
            }

            data.put("username", username);
            data.put("filename", fname);
            verifyStoragePermissions(this);
            byte[] arrayB = new byte[(int)file.length()];
            FileInputStream fis = new FileInputStream(file);
            fis.read(arrayB);
            fis.close();
            data.put("content", new String(Base64.encode(arrayB, Base64.DEFAULT)));

            int permission = ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION);

            if (permission != PackageManager.PERMISSION_GRANTED) {
                // We don't have permission so prompt the user
                ActivityCompat.requestPermissions(
                        this,
                        new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, PERMISSION_ACCESS_FINE_LOCATION
                );
            }
            ubicacionLoc = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if (ubicacionLoc != null) {
                ubicacion = String.valueOf(ubicacionLoc.toString());
            }
            data.put("ubicacion", ubicacion);

        } catch (JSONException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        Request request = new Request("POST", "/files/"+username+"/", data);
        request.setHeader("conn_token", token);
        System.out.println(data);
        request.send();
        actualizarArchivos();

    }

    /**
     * Se desconecta del sistema.
     */
    public void logOut(){
        Request request = new Request("DELETE", "/sessions/"+username);
        request.setHeader("conn_token", token);
        request.send();
    }

    /**
     * Chequea si la aplicación tiene permiso para escribir el almacenamiento externo.
     *
     * Si la aplicación no tiene permiso, se le pedirá al usuario que lo conceda.
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        // Check if we have write permission
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION_WRITE_EXTERNAL_STORAGE
            );
        }
    }

    /**
     * Llama a {@link #listFiles()} y actualiza los archivos mostrados.
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
