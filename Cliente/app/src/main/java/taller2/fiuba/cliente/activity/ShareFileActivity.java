package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.GridView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.tagsGridAdapter;

public class ShareFileActivity extends AppCompatActivity {

    private String token, username, filename;
    private List<String> users;
    GridView usersGrid;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_share_file);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        actualizarUsers();
        //Si se clickea un archivo, se abre un dialogo
        usersGrid.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                unshare(users.get(position));

            }
        });

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_share_file, menu);
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

    protected void actualizarUsers(){
        Request getfile = new Request("GET", "/files/"+username+"/"+filename);
        getfile.setHeader("conn_token", token);
        JSONObject response = getfile.send();
        System.out.println("actualizar Users enviado");
        try {
            JSONArray users_with_read_permission = response.getJSONArray("users_with_read_permission");
            JSONArray users_with_write_permission = response.getJSONArray("users_with_write_permission");
            users = new ArrayList();
            for (int i = 0; i < users_with_read_permission.length() ;i++){
                try {
                    String next = users_with_read_permission.getString(i);
                    System.out.println(next); // Debug
                    users.add(next);
                } catch(JSONException e){}
            }
            System.out.println(users.size());
            usersGrid = (GridView) findViewById(R.id.usersGrid);
            if (users != null) {
                usersGrid.setAdapter(new tagsGridAdapter(this, users.toArray(new String[users.size()])));
            } else {
                usersGrid.setAdapter(new tagsGridAdapter(this, null));
            }

        } catch ( JSONException e){
            System.out.println("Response sin campo users_with_read_permission");
        }
    }

    public void shareButton(View view){
        System.out.println("Share button pressed");
        try {
            JSONObject data = new JSONObject();
            JSONArray userACompartir = new JSONArray();
            userACompartir.put(((EditText) findViewById(R.id.userToAdd)).getText().toString());
            data.put("owner_username", username);
            data.put("users_with_read_permission_add", userACompartir);
            data.put("users_with_write_permission_add", userACompartir);
            Request request = new Request("PUT", "/files/"+username+"/"+filename, data);
            request.setHeader("conn_token", token);
            request.send();
            actualizarUsers();
        } catch (JSONException e){
            System.out.println("Error al compartir");
        }
    }

    protected void unshare(final String username){
        System.out.println("unshare pressed");
        new AlertDialog.Builder(this)
                .setTitle("Unshare file")
                .setMessage("Are you sure you want to unshare this file with "+username+"?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            JSONObject data = new JSONObject();
                            JSONArray userADescompartir = new JSONArray();
                            data.put("owner_username", username);
                            userADescompartir.put(username);
                            data.put("users_with_read_permission_remove", userADescompartir);
                            data.put("users_with_write_permission_remove", userADescompartir);
                            Request request = new Request("PUT", "/files/"+username+"/"+filename, data);
                            request.setHeader("conn_token", token);
                            request.send();
                            actualizarUsers();
                        } catch (JSONException e){
                            System.out.println("Error al descompartir");
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

}
