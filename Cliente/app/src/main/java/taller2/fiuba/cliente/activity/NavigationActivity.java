package taller2.fiuba.cliente.activity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.KeyEvent;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.GridView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
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
import taller2.fiuba.cliente.model.MyAdapter;

public class NavigationActivity extends AppCompatActivity {

    private static final int PICKFILE_RESULT_CODE = 101;
    private static final int ADVANCED_SEARCH_CODE = 102;
    GridView gridView;
    static List<String> archivos = new ArrayList();

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_navigation);
        //Se pide una lista de los archivos del usuario al server
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
            gridView.setAdapter(new MyAdapter(this, archivos.toArray(new String[archivos.size()])));
        } else {
            gridView.setAdapter(new MyAdapter(this, null));
        }

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
            }
        });

        TextView advancedSearch = (TextView) findViewById(R.id.advancedSearch);
        advancedSearch.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                System.out.println("advanced search clickeado");
                Intent advancedSearchIntent = new Intent(v.getContext(), AdvancedSearchActivity.class);
                startActivityForResult(advancedSearchIntent, ADVANCED_SEARCH_CODE);

            }
        });
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
        if (id == R.id.action_settings) {
            return true;
        }
        if (id == android.R.id.home){ //Boton UP (flecha arriba a la izquierda)
            this.logOut();
        }
        if (id == R.id.upload_file){
            Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
            fileintent.setType("gagt/sdf"); //Este intent es un navegador de archivos
            try {
                startActivityForResult(fileintent, PICKFILE_RESULT_CODE);
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


    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (data == null)
            return;
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    String FilePath = data.getData().getPath();
                    uploadFile(FilePath);
                }
            case ADVANCED_SEARCH_CODE:
                if (resultCode == RESULT_OK){
                    System.out.println("fuera de advanced search");
                } else {
                    System.out.println("not cool");
                }

        }
    }

    public JSONArray listFiles(){
        System.out.println("/files/" + getIntent().getStringExtra("username") + "/");

        Request request = new Request("GET", "/files/"+getIntent().getStringExtra("username")+"/");
        request.setHeader("conn_token", getIntent().getStringExtra("token"));
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

        File file = new File(path);
        try {
            String fname = path;
            int pos = fname.lastIndexOf("/");
            if (pos > 0) {
                fname = fname.substring(0, pos);
            }
            data.put("conn_token", getIntent().getStringExtra("token"));
            data.put("username", getIntent().getStringExtra("username"));
            data.put("filename", fname);
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
        Request request = new Request("POST", "/files/"+getIntent().getStringExtra("username"), data);
        request.send();
    }

    public void logOut(){
        Request request = new Request("DELETE", "/sessions/"+getIntent().getStringExtra("username"));
        request.setHeader("conn_token", getIntent().getStringExtra("token"));
        request.send();
    }


}
