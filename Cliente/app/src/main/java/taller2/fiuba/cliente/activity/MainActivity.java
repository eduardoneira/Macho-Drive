package taller2.fiuba.cliente.activity;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;
import java.util.regex.Pattern;
import java.security.MessageDigest;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

/**
 * Actividad inicial.
 * Permite registrarse e ingresar al sistema.
 */
public class MainActivity extends AppCompatActivity {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

    /**
     * Constructor.
     * Deja invisibles los mensajes de error ("Solo se permiten letras o numeros")
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
        (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
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
     * Metodo Log In
     * Toma el usuario y contraseñas que se ingresaron e intenta conectarse al sistema.
     * Se pide que los campos no sean nulos y sean alfanuméricos.
     * En caso de ingresar un campo con caracteres inválidos, se informa al usuario de ello.
     * En caso de ingresar nombre de usuario o contraseña incorrecta, se le informa al usuario.
     * Si todo sale bien, inicializa {@link NavigationActivity}
     *
     * @param view
     */
    public void logIn(View view){
        Map mapa = new HashMap();
        Pattern p = Pattern.compile("[^a-zA-Z0-9]");
        String username = ((EditText) findViewById(R.id.usernameField)).getText().toString();
        String password = ((EditText)findViewById(R.id.passwordField)).getText().toString();
        if(p.matcher(username).find() ||  username.isEmpty()){
            (findViewById(R.id.invalidUsername)).setVisibility(View.VISIBLE);
            return ;
        } else if (p.matcher(password).find() || password.isEmpty()) {
            (findViewById(R.id.invalidPassword)).setVisibility(View.VISIBLE);
            return ;
        } else {
            (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
            (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);
            mapa.put("username", username);
            System.out.println(md5(password)); //Debug
            mapa.put("password", md5(password));
            JSONObject json = new JSONObject(mapa);
            Request request = new Request("POST", "/sessions/", json);
            JSONObject response = request.send();
            try {
                if (response.getString("status").equals("fail")) {
                    Toast.makeText(getApplicationContext(), "Invalid username or password", Toast.LENGTH_SHORT).show();
                    return ;
                }
            } catch (JSONException e){}
            System.out.println(response.toString()); //Debug
            Intent navigationActivity = new Intent(this, NavigationActivity.class);
            try {
                String token = (String) response.get("conn_token");
                navigationActivity.putExtra("token", token);
                navigationActivity.putExtra("username", ((EditText) findViewById(R.id.usernameField)).getText().toString());
                startActivity(navigationActivity);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }

    }

    /**
     * Método Sign Up
     * Toma los campos ingresados por el usuario e intenta registrarse en el sistema.
     * Se piden que los campos no sean nulos y que sean alfanuméricos.
     * En caso de ingresar caracteres invalidos, se informa al usuario.
     * En caso de que ya exista el nombre de usuario, se informa al usuario.
     * @param view
     */
    public void signUp(View view){
        LOGGER.info("Usuario solicito registrarse.");
        Map mapa = new HashMap();
        Pattern p = Pattern.compile("[^a-zA-Z0-9]");
        String username = ((EditText) findViewById(R.id.usernameField)).getText().toString();
        String password = ((EditText)findViewById(R.id.passwordField)).getText().toString();
        if(p.matcher(username).find() ||  username.isEmpty()){
            (findViewById(R.id.invalidUsername)).setVisibility(View.VISIBLE);
            return ;
        } else if (p.matcher(password).find() || password.isEmpty()) {
            (findViewById(R.id.invalidPassword)).setVisibility(View.VISIBLE);
            return ;
        } else {
            (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
            (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);
            try {
                mapa.put("username", username);
                mapa.put("password", md5(password));

                JSONObject json = new JSONObject(mapa);
                Request request = new Request("POST", "/users/", json);
                JSONObject response = request.send();
                if(response.getString("status").equals("fail")){
                    Toast.makeText(getApplicationContext(), "User already exists", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(getApplicationContext(), "Successfully signed up", Toast.LENGTH_SHORT).show();
                }
            } catch (Exception e){}
        }
    }

    /**
     * Hashea la contraseña según el algoritmo md5.
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
            return "Error.";
        }
    }
}
