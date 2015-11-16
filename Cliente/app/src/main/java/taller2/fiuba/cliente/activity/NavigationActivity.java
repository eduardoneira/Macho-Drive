package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.KeyEvent;
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
import java.util.logging.Logger;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.fileGridAdapter;
import android.widget.AdapterView.OnItemSelectedListener;

public class NavigationActivity extends AppCompatActivity implements OnItemSelectedListener {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);
    private static final int PICKFILE_RESULT_CODE = 101;
    private static final int ADVANCED_SEARCH_CODE = 102;
    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    private static final int PROFILE_SETTINGS_RESULT_CODE = 105;
    private String token, username;
    GridView gridView;
    static List<String> archivos = new ArrayList();
    private Spinner spinner;
    private static final String[]paths = {"Name", "Owner", "Tag", "Extension"};


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
                miDialogo diag = new miDialogo();
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

    }

    public void onNothingSelected(AdapterView<?> parent){}

    public void onItemSelected(AdapterView<?> parent, View v, int position, long id) {

        switch (position) {
            case 0:
                // Whatever you want to happen when the first item gets selected
                break;
            case 1:
                // Whatever you want to happen when the second item gets selected
                break;
            case 2:
                // Whatever you want to happen when the thrid item gets selected
                break;
            case 3:
                break;

        }
    }

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


        return super.onOptionsItemSelected(item);
    }



    @Override
    public void onBackPressed() { //Boton BACK (triangulo abajo a la izquierda)
        this.logOut();
        super.onBackPressed();
    }

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

    public void logOut(){
        Request request = new Request("DELETE", "/sessions/"+username);
        request.setHeader("conn_token", token);
        request.send();
    }

    /**
     * Checks if the app has permission to write to device storage
     *
     * If the app does not has permission then the user will be prompted to grant permissions
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
