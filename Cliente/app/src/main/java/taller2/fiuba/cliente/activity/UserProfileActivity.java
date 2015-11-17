package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.AlertDialog;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Base64;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

/**
 * Created by nicolas on 17/11/15.
 */
public class UserProfileActivity extends AppCompatActivity {

    private static final int PERMISSION_ACCESS_FINE_LOCATION = 106;
    private String username, token;
    private String name, email, ubicacion, picture;
    private static final int PICKFILE_RESULT_CODE = 101;



    /**
     * Constructor de la actividad de buscador de perfiles.
     * Inicializa las variables {@link #username} y {@link #token}.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
        setContentView(R.layout.activity_user_profile);
        setTheme(R.style.GreyText);

        username = getIntent().getStringExtra("username");
        token = getIntent().getStringExtra("token");

    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_profile_settings, menu);
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

    /**
     * Busca el usuario ingresado y muestra su perfil.
     * En caso de no existir, se informa al usuario por pantalla.
     * @param view
     */
    public void Search(View view){
        try {
            Request request = new Request("GET", "/users/" + ((TextView)findViewById(R.id.username)).getText().toString());
            request.setHeader("conn_token", token);
            JSONObject response = request.send();
            if (response.has("status")){
                Toast.makeText(getApplicationContext(), "User does not exist", Toast.LENGTH_SHORT).show();
            } else {
                name = response.getString("name");
                email = response.getString("email");
                ubicacion = response.getString("ultima_ubicacion");
                picture = response.getString("picture");
                if (picture.isEmpty()) {
                    ((ImageView) findViewById(R.id.profilePicture)).setImageResource(R.drawable.machoke);
                } else {
                    byte[] decodedString = Base64.decode(picture, Base64.DEFAULT);
                    Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);
                    ((ImageView) findViewById(R.id.profilePicture)).setImageBitmap(decodedByte);
                }

                ((TextView) findViewById(R.id.name)).setText(name);
                ((TextView) findViewById(R.id.email)).setText(email);
                ((TextView) findViewById(R.id.location)).setText(ubicacion);
            }
        } catch (JSONException e){
            System.out.println("Error en la solicitud de datos de usuario");
        }
    }





}
