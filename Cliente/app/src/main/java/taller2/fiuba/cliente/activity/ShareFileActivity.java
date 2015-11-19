package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.adapter.TagsGridAdapter;

/**
 * Actividad para compartir y descompartir un archivo
 */
public class ShareFileActivity extends AppCompatActivity {

    private String token, username, filename;
    /**
     * Lista de usuarios con acceso al archivo.
     */
    private List<String> users;
    /**
     * Grilla de usuarios con acceso al archivo.
     */
    GridView usersGrid;

    /**
     * Constructor de la actividad encargada de compartir archivos.
     * Inicializa las variables {@link #token}, {@link #username} y {@link #filename}.
     * Inicializa el listener para cuando se quiere descompartir un archivo. {@link #unshare(String)}
     *
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("ShareFileActivity", "Se crea la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_share_file);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        filename = getIntent().getStringExtra("filename");
        actualizarUsers();
        usersGrid.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                unshare(users.get(position));
            }
        });

    }

    /**
     * Actualiza la lista de users con los que esta compartido el archivo.
     */
    protected void actualizarUsers() {
        Log.d("ShareFileActivity", "Se actualiza la lista de usuarios con acceso al archivo");
        Request getfile = new Request("GET", "/files/" + username + "/" + filename);
        getfile.setHeader("conn_token", token);
        JSONObject response = getfile.send();
        try {
            JSONArray users_with_read_permission = response.getJSONArray("users_with_read_permission");
            JSONArray users_with_write_permission = response.getJSONArray("users_with_write_permission");
            users = new ArrayList();
            for (int i = 0; i < users_with_read_permission.length(); i++) {
                try {
                    String next = users_with_read_permission.getString(i);
                    users.add(next);
                } catch (JSONException e) {}
            }
            Log.d("ShareFileActivity", "Se recibieron " + users.size() + " usuarios");
            usersGrid = (GridView) findViewById(R.id.usersGrid);
            if (users != null) {
                usersGrid.setAdapter(new TagsGridAdapter(this, users.toArray(new String[users.size()])));
            } else {
                usersGrid.setAdapter(new TagsGridAdapter(this, null));
            }
        } catch (JSONException e) {}
    }

    /**
     * Pide al server que comparta el archivo con el usuario indicado.
     * En caso de que no exista, se informa al usuario.
     * @param view
     */
    public void shareButton(View view){
        Log.d("ShareFileActivity", "Se presiono el boton Share");
        try {
            JSONObject data = new JSONObject();
            JSONArray userACompartir = new JSONArray();
            userACompartir.put(((EditText) findViewById(R.id.userToAdd)).getText().toString());
            Log.d("ShareFileActivity", "El usuario a compartir es " + userACompartir.getString(0));
            data.put("owner_username", username);
            data.put("users_with_read_permission_add", userACompartir);
            data.put("users_with_write_permission_add", userACompartir);
            Request request = new Request("PUT", "/files/"+username+"/"+filename, data);
            request.setHeader("conn_token", token);
            if(request.send().getString("status") == "fail"){
                Log.i("ShareFileActivity", "Usuario invalido");
                Toast.makeText(getApplicationContext(), "Invalid username", Toast.LENGTH_SHORT).show();
            } else {
                Log.d("ShareFileActivity", "Se compartio el archivo");
                actualizarUsers();
            }
        } catch (JSONException e){}
    }

    /**
     * Pregunta al usuario si está seguro de querer descompartir el archivo {@link #filename}.
     * En caso afirmativo, pide al server que lo haga.
     * Llama a {@link #actualizarUsers()}
     * @param username
     */
    protected void unshare(final String username){
        Log.d("ShareFileActivity", "Se quiere descompartir el archivo");
        new AlertDialog.Builder(this)
                .setTitle("Unshare file")
                .setMessage("Are you sure you want to unshare this file with "+username+"?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            Log.d("ShareFileActivity", "El usuario esta seguro de querer descompartir");
                            JSONObject data = new JSONObject();
                            JSONArray userADescompartir = new JSONArray();
                            data.put("owner_username", username);
                            userADescompartir.put(username);
                            data.put("users_with_read_permission_remove", userADescompartir);
                            data.put("users_with_write_permission_remove", userADescompartir);
                            Request request = new Request("PUT", "/files/"+username+"/"+filename, data);
                            request.setHeader("conn_token", token);
                            request.send();
                            Log.d("ShareFileActivity", "Se descompartio el archivo");
                            actualizarUsers();
                        } catch (JSONException e){}
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
