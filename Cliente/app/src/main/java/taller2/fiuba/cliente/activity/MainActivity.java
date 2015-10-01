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
        mapa.put("username", ((EditText)findViewById(R.id.usernameField)).getText().toString());
        mapa.put("password", ((EditText)findViewById(R.id.passwordField)).getText().toString());
        JSONObject json = new JSONObject(mapa);
        Request request = new Request("POST", "/sessions", json);
        JSONObject response = request.send();
        try{
            System.out.println(response.get("status"));
        } catch (JSONException e){}

    }


    public void signUp(View view){
        Intent signupActivity = new Intent(this, SignUpActivity.class);
        startActivity(signupActivity);
    }
}
