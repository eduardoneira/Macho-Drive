package taller2.fiuba.cliente.activity;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

public class ProfileSettingsActivity extends AppCompatActivity {

    private String username, token;
    private String name, email, ubicacion, picture;
    private static final int PICKFILE_RESULT_CODE = 101;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
        setContentView(R.layout.activity_profile_settings);
        setTheme(R.style.GreyText);
        username = getIntent().getStringExtra("username");
        token = getIntent().getStringExtra("token");
        ((TextView)findViewById(R.id.username)).setText(username);
        try {
            Request request = new Request("GET", "/users/" + username);
            request.setHeader("conn_token", token);
            JSONObject response = request.send();
            name = response.getString("name");
            email = response.getString("email");
            ubicacion = response.getString("ultima_ubicacion");
            picture = response.getString("picture");
            if(picture.isEmpty()){
                ((ImageView) findViewById(R.id.profilePicture)).setImageResource(R.drawable.machoke);
            } else{
                byte[] decodedString = Base64.decode(picture, Base64.DEFAULT);
                Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);
                ((ImageView) findViewById(R.id.profilePicture)).setImageBitmap(decodedByte);
            }

            ((EditText)findViewById(R.id.name)).setText(name);
            ((EditText)findViewById(R.id.email)).setText(email);
        } catch (JSONException e){
            System.out.println("Error en la solicitud de datos del usuario");
        }
    }

    public void saveChanges(View view){
        System.out.println("save changes");
        try {
            email = ((EditText)findViewById(R.id.email)).getText().toString();
            name = ((EditText)findViewById(R.id.name)).getText().toString();
            JSONObject data = new JSONObject();
            data.put("name", name);
            data.put("email", email);
            data.put("picture", picture);
            data.put("ultima_ubicacion", ubicacion);
            Request request = new Request("PUT", "/users/"+username, data);
            request.setHeader("conn_token", token);
            request.send();
        } catch (JSONException e){
            System.out.println("error al guardar cambios");
        }
    }

    public void changePicture(View view){
        System.out.println("Change picture");
        Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
        fileintent.addCategory(Intent.CATEGORY_OPENABLE);
        fileintent.setType("*/*"); //Este intent es un navegador de archivos
        try {
            startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
        } catch (ActivityNotFoundException e) {
        }
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

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (data == null)
            return;
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    Uri FilePath = data.getData();
                    System.out.println("picked file");
                    System.out.println(FilePath.toString());
                    System.out.println(FilePath.getPath());
                    File file = new File(Environment.getExternalStorageDirectory().toString(), (FilePath.getPath()).split(":")[1]);
                    try {
                        byte[] arrayB = new byte[(int) file.length()];
                        FileInputStream fis = new FileInputStream(file);
                        fis.read(arrayB);
                        fis.close();
                        picture = new String(Base64.encode(arrayB, Base64.DEFAULT));
                        Bitmap decodedByte = BitmapFactory.decodeByteArray(arrayB, 0, arrayB.length);
                        ((ImageView)findViewById(R.id.profilePicture)).setImageBitmap(decodedByte);
                    } catch (Exception e){

                    }
                }
                return;
        }
    }

}
