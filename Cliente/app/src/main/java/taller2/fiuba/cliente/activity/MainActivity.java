package taller2.fiuba.cliente.activity;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

public class MainActivity extends AppCompatActivity {

    private EditText username, password;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
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

    public void logIn(View view){
        Map mapa = new HashMap();
        Pattern p = Pattern.compile("[^a-zA-Z0-9]");
        String username = ((EditText) findViewById(R.id.usernameField)).getText().toString();
        String password = ((EditText)findViewById(R.id.passwordField)).getText().toString();
        if(p.matcher(username).find() || p.matcher(password).find() || username.isEmpty() || password.isEmpty()){
            return ; //User o pass invalidas
        } else {
            mapa.put("username", username);
            mapa.put("password", password);
            JSONObject json = new JSONObject(mapa);
            System.out.print("jsonAEnviar: ");
            System.out.println(json.toString());
            Request request = new Request("POST", "/sessions/", json);
            JSONObject response = request.send();
            System.out.println(response.toString());
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


    public void signUp(View view){
        Map mapa = new HashMap();
        Pattern p = Pattern.compile("[^a-zA-Z0-9]");
        String username = ((EditText) findViewById(R.id.usernameField)).getText().toString();
        String password = ((EditText)findViewById(R.id.passwordField)).getText().toString();
        if(p.matcher(username).find() || p.matcher(password).find() || username.isEmpty() || password.isEmpty()){
            return ; //User o pass invalidas
        } else {
            mapa.put("username", username);
            mapa.put("password", password);

            JSONObject json = new JSONObject(mapa);
            Request request = new Request("POST", "/users/", json);
            JSONObject response = request.send();
            System.out.println(response.toString());
        }
    }
}
