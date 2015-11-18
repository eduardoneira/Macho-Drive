package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.DialogoVersiones;
import taller2.fiuba.cliente.model.FileGridAdapter;

/**
 * Actividad de manejo de versiones de un archivo.
 */
public class FileVersionsActivity extends AppCompatActivity {

    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
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
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_versions);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        mostrarVersiones();

        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    final int position, long id) {
                String contenido = contenidoVersiones.get(contenidoVersiones.size()-position-1).toString();
                DialogoVersiones dial = new DialogoVersiones();
                Bundle bundle = new Bundle();
                bundle.putString("filename", filename);
                bundle.putString("content", contenido);
                bundle.putString("username", username);
                bundle.putString("token", token);
                bundle.putInt("version", contenidoVersiones.indexOf(contenido));
                dial.setArguments(bundle);
                dial.show(getFragmentManager(),"dialogo");
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
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_file_versions, menu);
        return true;
    }

    /**
     * Abre un navegador de archivos para elegir la nueva version a ser subida.
     *
     * @param item
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.new_version) {
            Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
            fileintent.addCategory(Intent.CATEGORY_OPENABLE);
            fileintent.setType("*/*");
            try {
                startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
            } catch (ActivityNotFoundException e) {}
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * Si se eligio un archivo para ser subido, se lo sube.
     * Llama a {@link #uploadVersion(String)}
     * @param requestCode Codigo de pedido
     * @param resultCode Codigo resultado
     * @param data Datos devueltos
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    if (data != null) {
                        Uri FilePath = data.getData();
                        uploadVersion(FilePath.getPath());
                    }
                }
                return;
        }
    }

    /**
     * Actualiza la lista de versiones que se muestra en pantalla.
     * Pide al server la lista y la muestra en una cuadricula {@link #gridView}
     */
    public void mostrarVersiones(){
        versiones = new ArrayList();
        Request request = new Request("GET", "/files/"+username+"/"+filename);
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        JSONArray versions = new JSONArray();
        try {
            JSONArray myFiles = response.getJSONArray("content");
            for(int i = 0; i < myFiles.length(); i++){
                versions.put(versions.length(), myFiles.get(i));
            }
        } catch (JSONException e){}
        for (int i = 0; i < versions.length() ;i++){
            try {
                String next = versions.getString(i);
                contenidoVersiones.add(contenidoVersiones.size(), next);
                versiones.add(filename + "_v" + new Integer(versions.length() - i).toString());
            } catch(JSONException e){}
        }
        //Se muestran las versiones en una cuadricula
        gridView = (GridView) findViewById(R.id.gridView);
        if (versiones != null) {
            gridView.setAdapter(new FileGridAdapter(this, versiones.toArray(new String[versiones.size()])));
        } else {
            gridView.setAdapter(new FileGridAdapter(this, null));
        }
    }

    /**
     * Sube al servidor una nueva version del archivo.
     * Llama a {@link #verifyStoragePermissions(Activity)}
     * Encodea el contenido del archivo en Base64.
     * Llama a {@link #mostrarVersiones()}
     * @param path Ruta del archivo que se desea subir
     */
    public void uploadVersion(String path){
        JSONObject data = new JSONObject();
        File file = new File(Environment.getExternalStorageDirectory().toString(), path.split(":")[1]);
        System.out.println(file.exists());

        try {
            data.put("owner_username", username);
            data.put("ubicacion", "");
            verifyStoragePermissions(this);
            byte[] arrayB = new byte[(int)file.length()];
            FileInputStream fis = new FileInputStream(file);
            fis.read(arrayB);
            fis.close();
            data.put("content_change", new String(Base64.encode(arrayB, Base64.DEFAULT)));
        } catch (JSONException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        Request request = new Request("PUT", "/files/"+username+"/"+filename, data);
        request.setHeader("conn_token", token);
        request.send();
        mostrarVersiones();
    }

    /**
     * Chequea si la aplicación tiene permiso para escribir en el almacenamiento externo.
     * <p/>
     * Si la aplicación no tiene permiso, se le pide al usuario que se lo dé.
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{
                            Manifest.permission.READ_EXTERNAL_STORAGE,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE
                    }, PERMISSION_WRITE_EXTERNAL_STORAGE
            );
        }
    }
}
