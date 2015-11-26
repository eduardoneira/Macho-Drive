package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.AnimationDrawable;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;
import java.security.MessageDigest;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

/**
 * Actividad inicial.
 * Permite registrarse e ingresar al sistema.
 */
public class MainActivity extends AppCompatActivity {
    private static Context context;

    /**
     * Constructor. Deja invisibles los mensajes de error ("Solo se permiten letras o numeros".
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        MainActivity.context = getApplicationContext();
        setContentView(R.layout.activity_main);

        ImageView i = (ImageView)findViewById(R.id.machamp);
        i.setBackgroundResource(R.drawable.machamp);

        AnimationDrawable pro = (AnimationDrawable)i.getBackground();
        pro.start();

        PreferenceManager.setDefaultValues(this, R.xml.preferences_app_settings_default, false);
        (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
        (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);
        Log.d("MainActivity", "Se creo la actividad");
    }

    /**
     * Getter del contexto principal de la aplicacion
     * @return El contexto principal de la aplicacion
     */
    public static Context getAppContext(){
        return MainActivity.context;
    }

    /**
     * Metodo de conexion al sistema.
     * Toma el usuario y passwords que se ingresaron e intenta conectarse al sistema.
     * Se pide que los campos no sean nulos y sean alfanumericos.
     * En caso de ingresar un campo con caracteres inválidos, se informa al usuario de ello.
     * En caso de ingresar nombre de usuario o password incorrecta, se le informa al usuario.
     * Si todo sale bien, inicializa {@link NavigationActivity}
     *
     * @param view
     */
    public void logIn(View view){
        Log.d("MainActivity", "Se presiono Log In");
        Map mapa = new HashMap();
        String username = ((EditText) findViewById(R.id.usernameField)).getText().toString();
        String password = ((EditText)findViewById(R.id.passwordField)).getText().toString();
        if(!validarCampos(username, password)){
            return;
        } else {
            (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
            (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);
            mapa.put("username", username);
            mapa.put("password", md5(password));
            JSONObject json = new JSONObject(mapa);

            try {
                Request request = new Request("POST", "/sessions/", json);
                JSONObject response = request.send();
                Log.d("MainActivity", "Se envio la request de Log In al servidor");

                try {
                    Log.d("MainActivity", "Se recibio status " + response.getString("status"));
                    Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
                } catch (JSONException e) {
                    Log.d("MainActivity", "La respuesta no contenia campo status ");
                    Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
                }

                if (request.getStatusCode() == HttpURLConnection.HTTP_OK) {
                    Intent navigationActivity = new Intent(this, NavigationActivity.class);
                    try {
                        String token = (String) response.get("conn_token");
                        navigationActivity.putExtra("token", token);
                        navigationActivity.putExtra("username", ((EditText) findViewById(R.id.usernameField)).getText().toString());
                        startActivity(navigationActivity);
                    } catch (JSONException e) {
                        Log.w("MainActivity", "No se recibio conn token");
                        e.printStackTrace();
                    }
                }
            } catch (Exception e){
                Log.d("MainActivity", "Error en el request");
                Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
            }
        }

    }

    /**
     * Al presionar el boton Back, se cierra la aplicacion
     */
    @Override
    public void onBackPressed() { //Boton BACK (triangulo abajo a la izquierda)
        Log.d("NavigationActivity", "Se presiono el boton Back");
        new AlertDialog.Builder(this)
                .setMessage("Are you sure you want to exit?")
                .setNegativeButton(android.R.string.no, null)
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {

                    public void onClick(DialogInterface arg0, int arg1) {
                        MainActivity.super.onBackPressed();
                    }
                }).create().show();
    }

    /**
     * Metodo llamado al presionar el boton Sign Up.
     * Toma los campos ingresados por el usuario e intenta registrarse en el sistema.
     * Se pide que los campos no sean nulos y que sean alfanumericos.
     * En caso de ingresar caracteres invalidos, se informa al usuario.
     * En caso de que ya exista el nombre de usuario, se informa al usuario.
     * @param view
     */
    public void signUp(View view){
        Log.d("MainActivity", "El usuario presiono Sign Up");
        Map mapa = new HashMap();
        String username = ((EditText) findViewById(R.id.usernameField)).getText().toString();
        String password = ((EditText)findViewById(R.id.passwordField)).getText().toString();
        if(!validarCampos(username, password)){
            return;
        } else {
            (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
            (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);
            mapa.put("username", username);
            mapa.put("password", md5(password));
            JSONObject json = new JSONObject(mapa);

            try{
                Request request = new Request("POST", "/users/", json);
                JSONObject response = request.send();
                Log.d("MainActivity", "Se envio la request de Sign Up al servidor");
                // en lugar de ver el codigo o mensaje que me llego, imprimo el mensaje del servidor
                // (esto iria solo en los casos en los que hay un toast)
                try {
                    Log.d("MainActivity", "Se recibio status " + response.getString("status"));
                    Toast.makeText(getApplicationContext(), response.getString("status"), Toast.LENGTH_SHORT).show();
                } catch (JSONException e) {
                    Log.d("MainActivity", "La respuesta no contenia campo status ");
                    Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
                }
            } catch (Exception e){
                Log.d("MainActivity", "Error en el request");
                Toast.makeText(getApplicationContext(), "Unexpected error, please try again", Toast.LENGTH_SHORT).show();
            }
        }
    }

    /**
     * Hashea la password segun el algoritmo md5.
     * @param password
     * @return Hash md5 de password
     */
    public static String md5(String password) {
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            md.update(password.getBytes());
            byte byteData[] = md.digest();
            StringBuffer hexString = new StringBuffer();
            for (int i=0;i<byteData.length;i++) {
                String hex=Integer.toHexString(0xff & byteData[i]);
                if(hex.length()==1) hexString.append('0');
                hexString.append(hex);
            }
            return hexString.toString();
        }
        catch(java.security.NoSuchAlgorithmException missing) {
            Log.e("MainActivity", "No se encontro el algoritmo md5");
            return "Error.";
        }
    }

    /**
     * Valida que username y password esten compuestos por letras y numeros unicamente
     * @param username Usuario a validar
     * @param password Password a validar
     * @return Si son validos ambos campos
     */
    private boolean validarCampos(String username, String password){
        Pattern p = Pattern.compile("[^a-zA-Z0-9]");
        if(p.matcher(username).find() ||  username.isEmpty()){
            (findViewById(R.id.invalidUsername)).setVisibility(View.VISIBLE);
            Log.i("MainActivity", "Se ingreso un usuario invalido");
            return false;
        } else if (p.matcher(password).find() || password.isEmpty()) {
            (findViewById(R.id.invalidPassword)).setVisibility(View.VISIBLE);
            Log.i("MainActivity", "Se ingreso una password invalida");
            return false;
        }
        Log.d("MainActivity", "Tanto el usuario como la password son alfanumericas");
        return true;

    }

    /**
     * Metodo que crea el menu cuando se presiona el boton superior derecho.
     * @param menu El menu a crear
     * @return Si salio bien
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.d("MainActivity", "Se abre el menu");
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }


    /**
     * Método que responde cuando se clickea un item en el menú.
     * Si se presionó el botón UP, se desconecta del sistema y vuelve a la actividad inicial. {@link #logOut()}
     * Si se presionó Profile Settings, se abre {@link ProfileSettingsActivity}.
     * Si se presionó Search User, se abre {@link UserProfileActivity}
     * Si se presionó Upload File, se abre una ventana de selección de archivo para ser subido.
     * Si se presionó Deleted Files, se abre {@link RecycleBinActivity}.
     * @param item El item presionado
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.app_settings) {
            Log.d("MainActivity", "Se selecciono App Settings");
            Intent appSettingsIntent = new Intent(getApplicationContext(), AppSettingsActivity.class);
            startActivity(appSettingsIntent);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
