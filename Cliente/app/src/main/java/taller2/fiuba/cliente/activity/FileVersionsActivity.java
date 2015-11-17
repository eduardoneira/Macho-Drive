package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
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
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.dialogoArchivos;
import taller2.fiuba.cliente.model.fileGridAdapter;

public class FileVersionsActivity extends AppCompatActivity {

    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    private static final int PICKFILE_RESULT_CODE = 101;
    private String token, username, filename;
    GridView gridView;
    static List<String> versiones = new ArrayList();
    static List<String> contenidoVersiones = new ArrayList();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        final Activity activity = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_versions);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        mostrarVersiones();

        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    final int position, long id) {
                new AlertDialog.Builder(activity)
                        .setTitle("Download version")
                        .setMessage("Are you sure you want to download this version?")
                        .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                try {
                                    System.out.println(versiones.get(position));
                                    byte[] content = contenidoVersiones.get(contenidoVersiones.size()-position-1).toString().getBytes();
                                    byte[] bytes = Base64.decode(content, Base64.DEFAULT);
                                    verifyStoragePermissions(activity);
                                    File file = new File("/mnt/sdcard/Download/" + filename);
                                    FileOutputStream fop = new FileOutputStream(file);
                                    fop.write(bytes);
                                    fop.flush();
                                    fop.close();
                                } catch (Exception e) {
                                    e.printStackTrace();
                                }
                            }})
                        .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                            }
                        })
                        .setIcon(android.R.drawable.ic_dialog_alert)
                        .show();

            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_file_versions, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.new_version) {
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
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    if (data != null) {
                        Uri FilePath = data.getData();
                        System.out.println("picked file");
                        System.out.println(FilePath.toString());
                        System.out.println(FilePath.getPath());
                        uploadVersion(FilePath.getPath());
                    }
                }
                return;
        }
    }

    public void mostrarVersiones(){
        versiones = new ArrayList();
        Request request = new Request("GET", "/files/"+username+"/"+filename);
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        System.out.println(response);
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
            gridView.setAdapter(new fileGridAdapter(this, versiones.toArray(new String[versiones.size()])));
        } else {
            gridView.setAdapter(new fileGridAdapter(this, null));
        }
    }

    public void uploadVersion(String path){
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
        System.out.println(data);
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
        // Check if we have write permission
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        System.out.println(permission == PackageManager.PERMISSION_GRANTED);
        ActivityCompat.checkSelfPermission(activity, Manifest.permission.READ_EXTERNAL_STORAGE);
        System.out.println(permission == PackageManager.PERMISSION_GRANTED);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
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
