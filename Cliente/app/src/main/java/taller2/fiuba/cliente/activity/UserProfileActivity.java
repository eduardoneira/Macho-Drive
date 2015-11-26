package taller2.fiuba.cliente.activity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.AnimationDrawable;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

/**
 * Actividad de busqueda de perfiles.
 */
public class UserProfileActivity extends AppCompatActivity {

    private String token;
    private String name, email, ubicacion, picture;

    /**
     * Constructor de la actividad de buscador de perfiles.
     * Inicializa la variable {@link #token}.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("UserProfileActivity", "Se creo la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_profile);

        ImageView i = (ImageView)findViewById(R.id.machamp);
        i.setBackgroundResource(R.drawable.machamp);

        AnimationDrawable pro = (AnimationDrawable)i.getBackground();
        pro.start();

        (findViewById(R.id.nametextview)).setVisibility(View.INVISIBLE);
        (findViewById(R.id.locationtextview)).setVisibility(View.INVISIBLE);
        (findViewById(R.id.emailtextview)).setVisibility(View.INVISIBLE);
        setTheme(R.style.GreyText);
        token = getIntent().getStringExtra("token");
    }

    /**
     * Busca el usuario ingresado y muestra su perfil.
     * En caso de no existir, se informa al usuario por pantalla.
     * Setea las variables {@link #name}, {@link #email}, {@link #ubicacion} y {@link #picture}
     * @param view
     */
    public void Search(View view){
        Log.d("UserProfileActivity", "Se presiono el boton Search");
        try {
            Request request = new Request("GET", "/users/" + ((TextView)findViewById(R.id.username)).getText().toString()+"/profile/");
            request.setHeader("conn_token", token);
            JSONObject response = request.send();

            //Log.d("UserProfileActivity", "Se recibio status " + response.getString("status"));
            //Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();

            if(request.getStatusCode() == HttpURLConnection.HTTP_OK){
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
                (findViewById(R.id.nametextview)).setVisibility(View.VISIBLE);
                (findViewById(R.id.locationtextview)).setVisibility(View.VISIBLE);
                (findViewById(R.id.emailtextview)).setVisibility(View.VISIBLE);
                ((TextView) findViewById(R.id.name)).setText(name);
                ((TextView) findViewById(R.id.email)).setText(email);
                if (ubicacion.split(" ").length > 1) {
                    String latitud = ubicacion.split(" ")[0];
                    String longitud = ubicacion.split(" ")[1];
                    ((TextView) findViewById(R.id.location)).setText(latitud.substring(0, 5) + ", " + longitud.substring(0, 5));
                }
                Log.d("UserProfileActivity", "Se encontro y mostro el usuario");
                //Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
            } else {
                (findViewById(R.id.name)).setVisibility(View.INVISIBLE);
                (findViewById(R.id.location)).setVisibility(View.INVISIBLE);
                (findViewById(R.id.email)).setVisibility(View.INVISIBLE);
                (findViewById(R.id.profilePicture)).setVisibility(View.INVISIBLE);
                Log.d("UserProfileActivity", "No se encontro el usuario");
                Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
            }
        } catch (Exception e){
            Log.d("UserProfileActivity", "Error en la request");
            Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
        }
    }
}
