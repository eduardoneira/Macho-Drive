package taller2.fiuba.cliente.activity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Base64;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

/**
 * Actividad de busqueda de perfiles.
 */
public class UserProfileActivity extends AppCompatActivity {

    private String username, token;
    private String name, email, ubicacion, picture;

    /**
     * Constructor de la actividad de buscador de perfiles.
     * Inicializa las variables {@link #username} y {@link #token}.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_profile);
        setTheme(R.style.GreyText);

        username = getIntent().getStringExtra("username");
        token = getIntent().getStringExtra("token");

    }

    /**
     * Busca el usuario ingresado y muestra su perfil.
     * En caso de no existir, se informa al usuario por pantalla.
     * Setea las variables {@link #name}, {@link #email}, {@link #ubicacion} y {@link #picture}
     * @param view
     */
    public void Search(View view){
        try {
            Request request = new Request("GET", "/users/" + ((TextView)findViewById(R.id.username)).getText().toString()+"/profile/");
            request.setHeader("conn_token", token);
            JSONObject response = request.send();
            if (response.has("status")){
                Toast.makeText(getApplicationContext(), "User does not exist", Toast.LENGTH_SHORT).show();
            } else {
                name = response.getString("name");
                email = response.getString("email");
                ubicacion = response.getString("place");
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
        } catch (JSONException e){}
    }
}
