package taller2.fiuba.cliente.activity;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.text.Layout;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.RelativeLayout.LayoutParams;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.tagsGridAdapter;
import taller2.fiuba.cliente.model.Request;

public class ModifyFileActivity extends AppCompatActivity {

    private String token, filename, username;
    private List<String> tags;
    GridView tagsGrid;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_modify_file);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        ((EditText)findViewById(R.id.filename)).setText(filename);
        actualizarTags();
        //Si se clickea un archivo, se abre un dialogo
        tagsGrid.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                deleteTag(tags.get(position));

            }
        });


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_modify_file, menu);
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

    public void addTagButton(View view){
        addTag(((EditText)findViewById(R.id.tagToAdd)).getText().toString());
    }

    protected void addTag(String tag){
        try {
            JSONObject data = new JSONObject();
            JSONArray tagAagregar = new JSONArray();
            tagAagregar.put(tag);
            data.put("owner_username", username);
            data.put("tags_add", tagAagregar);
            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
            request.setHeader("conn_token", token);
            request.send();
            actualizarTags();
        } catch (JSONException e){
            System.out.println("Error al agregar tag");
        }
    }

    protected void actualizarTags(){
        Request getfile = new Request("GET", "/files/"+username+"/"+filename);
        getfile.setHeader("conn_token", token);
        JSONObject response = getfile.send();
        System.out.println("actualizar Tags enviado");
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
            System.out.println(tags.size());
            tagsGrid = (GridView) findViewById(R.id.tagsGrid);
            if (tags != null) {
                tagsGrid.setAdapter(new tagsGridAdapter(this, tags.toArray(new String[tags.size()])));
            } else {
                tagsGrid.setAdapter(new tagsGridAdapter(this, null));
            }

        } catch ( JSONException e){
            System.out.println("Response sin campo tags");
        }
    }

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
                            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
                            request.setHeader("conn_token", token);
                            request.send();
                            actualizarTags();

                        } catch (JSONException e){

                        }
                    }
                })
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();

    }

    public void changeFilename(View view){
        System.out.println("ChangeFilename clickeado");
        try {
            String newFilename = ((EditText) findViewById(R.id.filename)).getText().toString();
            JSONObject data = new JSONObject();
            data.put("owner_username", username);
            data.put("filename_change", newFilename);
            Request request = new Request("PUT", "/files/" + username + "/" + filename, data);
            request.setHeader("conn_token", token);
            request.send();
        } catch(JSONException e){
            System.out.println("Error al crear el JSON de changeFilename");
        }
    }
}
