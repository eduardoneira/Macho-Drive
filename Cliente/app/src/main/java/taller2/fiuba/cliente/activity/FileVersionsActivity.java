package taller2.fiuba.cliente.activity;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.graphics.drawable.AnimationDrawable;
import android.net.Uri;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.dialog.DialogoVersiones;
import taller2.fiuba.cliente.adapter.FileGridAdapter;
import taller2.fiuba.cliente.model.Permissions;

/**
 * Actividad de manejo de versiones de un archivo.
 */
public class FileVersionsActivity extends FileChooserActivity {

    private static final int PICKFILE_RESULT_CODE = 101;
    private String token, username, filename;
    /**
     * Grilla de versiones
     */
    GridView gridView;
    /**
     * Lista de versiones
     */
    static List<String> versiones = new ArrayList();
    /**
     * Contenidos de las versiones
     */
    static List<String> contenidoVersiones = new ArrayList();

    /**
     * Constructor. Inicializa las variables de la actividad.
     * Inicializa {@link #username}, {@link #token} y {@link #filename}
     * Llama a {@link #mostrarVersiones()}
     * Establece el listener para cuando se clickea una version. {@link DialogoVersiones}
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("FileVersionsActivity", "Se creo la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_versions);

        ImageView i = (ImageView)findViewById(R.id.machamp);
        i.setBackgroundResource(R.drawable.machamp);

        AnimationDrawable pro = (AnimationDrawable)i.getBackground();
        pro.start();

        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        if(!mostrarVersiones()){
            return;
        }
        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    final int position, long id) {
                Log.d("FileVersionsActivity", "Se clickeo un item");
                String contenido = contenidoVersiones.get(contenidoVersiones.size() - position - 1).toString();
                DialogoVersiones dial = new DialogoVersiones();
                Bundle bundle = new Bundle();
                bundle.putString("filename", filename);
                bundle.putString("content", contenido);
                bundle.putString("username", username);
                bundle.putString("token", token);
                bundle.putInt("version", contenidoVersiones.indexOf(contenido));
                dial.setArguments(bundle);
                Log.d("FileVersionsActivity", "Se muestra el dialogo");
                dial.show(getFragmentManager(), "dialogo");
            }
        });
    }

    /**
     * Inicializador del menu de opciones
     * @param menu El menu a inicializar
     * @return Si salio bien
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.d("FileVersionsActivity", "Se creo el menu de opciones");
        getMenuInflater().inflate(R.menu.menu_file_versions, menu);
        return true;
    }

    /**
     * Abre un navegador de archivos para elegir la nueva version a ser subida.
     * @param item
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.new_version) {
            Log.d("FileVersionsActivity", "Se clickeo New Version");
            /*Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
            fileintent.addCategory(Intent.CATEGORY_OPENABLE);
            fileintent.setType("file/*");
            try {
                Log.d("FileVersionsActivity", "Se crea el navegador de archivos");
                startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
            } catch (ActivityNotFoundException e) {}*/
            showFileListDialog(Environment.getExternalStorageDirectory().toString(), FileVersionsActivity.this, false);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    protected void OnSelectFileAction(String file) {
        Log.d("FileVersionsActivity", "Se selecciono un archivo");
        uploadVersion(file);
    }

    /**
     * Si se eligio un archivo para ser subido, se lo sube.
     * Llama a {@link #uploadVersion(String)}
     * @param requestCode Codigo de pedido
     * @param resultCode Codigo resultado
     * @param data Datos devueltos
     */
    /*@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d("FileVersionsActivity", "Termino el navegador de archivos");
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    if (data != null) {
                        Log.d("FileVersionsActivity", "Se selecciono un archivo");
                        Uri FilePath = data.getData();
                        uploadVersion(FilePath.getPath());
                    }
                }
                return;
        }
    }*/

    /**
     * Actualiza la lista de versiones que se muestra en pantalla.
     * Pide al server la lista y la muestra en una cuadricula {@link #gridView}
     */
    public boolean mostrarVersiones(){
        Log.d("FileVersionsActivity", "Se actualiza la lista de versiones");
        versiones = new ArrayList();
        contenidoVersiones = new ArrayList();

        Request request;
        JSONObject response;
        JSONArray versions = new JSONArray();

        try {

            request = new Request("GET", "/files/"+username+"/"+filename);
            request.setHeader("conn_token", token);
            response = request.send();

            if(request.getStatusCode() != HttpURLConnection.HTTP_OK){
                Log.d("FileVersionsActivity", "Algo esta corrupto, el file aparece en la lista pero no se encuentra");
                Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
                return false;
            }

            JSONArray myFiles = response.getJSONArray("content");
            for(int i = 0; i < myFiles.length(); i++){
                versions.put(versions.length(), myFiles.get(i));
            }
        } catch (Exception e){
            return false;
        }
        Log.d("FileVersionsActivity", "Se recibieron "+ versions.length() + " versiones");
        for (int i = 0; i < versions.length() ;i++){
            try {
                String next = versions.getString(i);
                contenidoVersiones.add(contenidoVersiones.size(), next);
                versiones.add(filename + "_v" + new Integer(versions.length() - i).toString());
            } catch(JSONException e){
                return false;
            }
        }
        gridView = (GridView) findViewById(R.id.gridView);
        if (versiones != null) {
            gridView.setAdapter(new FileGridAdapter(this, versiones.toArray(new String[versiones.size()])));
        } else {
            gridView.setAdapter(new FileGridAdapter(this, null));
        }
        return true;
    }

    /**
     * Sube al servidor una nueva version del archivo.
     * Llama a {@link Permissions}
     * Encodea el contenido del archivo en Base64.
     * Llama a {@link #mostrarVersiones()}
     * @param path Ruta del archivo que se desea subir
     */
    public void uploadVersion(String path){
        Log.d("FileVersionsActivity", "Se inicia la subida de una nueva version");
        JSONObject data = new JSONObject();
        File file;
        FileInputStream fis;
        try {
            file = new File(path);
            fis = new FileInputStream(file);
        } catch (Exception e) {
            Log.d("FileVersionsActivity", "Estas en el emulador");
            try {
                file = new File(Environment.getExternalStorageDirectory().toString(), path.split(":")[1]);
                fis = new FileInputStream(file);
            } catch (Exception ex) {
                ex.printStackTrace();
                file = null;
                fis = null;
                Log.e("FileVersionsActivity", "No se pudo obtener el archivo");
            }
        }
        try {
            data.put("owner_username", username);
            data.put("ubicacion", "");
            Permissions.verifyStoragePermissions(this);
            byte[] arrayB = new byte[(int)file.length()];
            fis.read(arrayB);
            fis.close();
            data.put("content_change", new String(Base64.encode(arrayB, Base64.DEFAULT)));
        } catch (JSONException e) {
            Log.w("FileVersionsActivity", "Error en la creacion de JSONObject");
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            Log.w("FileVersionsActivity", "Archivo invalido");
            e.printStackTrace();
        } catch (IOException e) {
            Log.w("FileVersionsActivity", "Error de escritura");
            e.printStackTrace();
        }

        Request request;
        try {
            request = new Request("PUT", "/files/"+username+"/"+filename, data);
            request.setHeader("conn_token", token);
            JSONObject response = request.send();

            Log.d("FileVersionsActivity", "Se recibio status " + response.getString("status"));
            Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();

            if(request.getStatusCode() == HttpURLConnection.HTTP_OK){
                mostrarVersiones();
            }

        } catch (Exception e) {
            Log.d("FileVersionsActivity", "La respuesta no contenia campo status ");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
    }
}
