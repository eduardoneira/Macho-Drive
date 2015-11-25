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

import java.net.HttpURLConnection;
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
        if(!actualizarUsers()){
            return;
        }
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
    protected boolean actualizarUsers() {

        try {
            Log.d("ShareFileActivity", "Se actualiza la lista de usuarios con acceso al archivo");
            Request getfile = new Request("GET", "/files/" + username + "/" + filename + "/metadata");
            getfile.setHeader("conn_token", token);
            JSONObject response = getfile.send();

            if(getfile.getStatusCode() != HttpURLConnection.HTTP_OK){
                Log.d("ShareFileActivity", "Algo esta corrupto, aparece el file en la lista pero no se puede encontrar");
                Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
                return false;
            }

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
        } catch (Exception e) {
            Log.d("ShareFileActivity", "Error en la request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
        return true;
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
            JSONObject response = request.send();

            Log.d("ShareFileActivity", "Se recibio status " + response.getString("status"));
            Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
            if(request.getStatusCode() == HttpURLConnection.HTTP_OK){
                actualizarUsers();
            }
        } catch (Exception e){
            Log.d("ShareFileActivity", "Error en la request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Pregunta al usuario si está seguro de querer descompartir el archivo {@link #filename}.
     * En caso afirmativo, pide al server que lo haga.
     * Llama a {@link #actualizarUsers()}
     * @param other_username
     */
    protected void unshare(final String other_username){
        Log.d("ShareFileActivity", "Se quiere descompartir el archivo");
        new AlertDialog.Builder(this)
                .setTitle("Unshare file")
                .setMessage("Are you sure you want to unshare this file with "+other_username+"?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            Log.d("ShareFileActivity", "El usuario esta seguro de querer descompartir");
                            JSONObject data = new JSONObject();
                            JSONArray userADescompartir = new JSONArray();
                            data.put("owner_username", username);
                            userADescompartir.put(other_username);
                            data.put("users_with_read_permission_remove", userADescompartir);
                            data.put("users_with_write_permission_remove", userADescompartir);
                            Request request = new Request("PUT", "/files/"+username+"/"+filename, data);
                            request.setHeader("conn_token", token);

                            JSONObject response = request.send();
                            Log.d("ShareFileActivity", "Se recibio status " + response.getString("status"));

                            if(request.getStatusCode() == HttpURLConnection.HTTP_OK){
                                actualizarUsers();
                            } else {
                                Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
                            }
                        } catch (Exception e){
                            Log.d("ShareFileActivity", "Error en la request");
                            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
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
