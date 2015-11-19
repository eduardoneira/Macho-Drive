package taller2.fiuba.cliente.activity;

import android.content.Intent;
import android.content.res.Resources;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

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

    /**
     * Constructor. Deja invisibles los mensajes de error ("Solo se permiten letras o numeros".
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Request.server = getResources().getString(R.string.ip);
        (findViewById(R.id.invalidUsername)).setVisibility(View.INVISIBLE);
        (findViewById(R.id.invalidPassword)).setVisibility(View.INVISIBLE);
        Log.d("MainActivity", "Se creo la actividad");
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
            Request request = new Request("POST", "/sessions/", json);
            JSONObject response = request.send();
            Log.d("MainActivity", "Se envio la request de Log In al servidor");
            try {
                if (response.getString("status").equals("fail")) {
                    Log.i("MainActivity", "El usuario o la password ingresados son incorrectos");
                    Toast.makeText(getApplicationContext(), "Invalid username or password", Toast.LENGTH_SHORT).show();
                    return ;
                }
                Log.d("MainActivity","Se ingreso usuario y password correctos");
            } catch (JSONException e){}
            Intent navigationActivity = new Intent(this, NavigationActivity.class);
            try {
                String token = (String) response.get("conn_token");
                navigationActivity.putExtra("token", token);
                navigationActivity.putExtra("username", ((EditText) findViewById(R.id.usernameField)).getText().toString());
                startActivity(navigationActivity);
            } catch (JSONException e) {
                Log.w("MainActivity", "Error al crear JSONObject");
                e.printStackTrace();
            }
        }

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
            try {
                mapa.put("username", username);
                mapa.put("password", md5(password));
                JSONObject json = new JSONObject(mapa);
                Request request = new Request("POST", "/users/", json);
                JSONObject response = request.send();
                Log.d("MainActivity", "Se envio la request de Sign Up al servidor");
                if(response.getString("status").equals("fail")){
                    Log.i("MainActivity", "No se pudo registrar");
                    Toast.makeText(getApplicationContext(), "User already exists", Toast.LENGTH_SHORT).show();
                } else {
                    Log.d("MainActivity", "El usuario se registro exitosamente");
                    Toast.makeText(getApplicationContext(), "Successfully signed up", Toast.LENGTH_SHORT).show();
                }
            } catch (JSONException e){
                Log.w("MainActivity", "La response no tenia campo status");
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
}
