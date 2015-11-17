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
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
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

public class ProfileSettingsActivity extends AppCompatActivity {

    private static final int PERMISSION_ACCESS_FINE_LOCATION = 106;
    private String username, token;
    private String name, email, ubicacion, picture;
    private static final int PICKFILE_RESULT_CODE = 101;
    private Location ubicacionLoc;
    /**
     * Variable encargada de ir actualizando la posición actual.
     */
    private final LocationListener mLocationListener = new LocationListener() {
        @Override
        public void onStatusChanged(String s, int i, Bundle b){}
        public void onProviderEnabled(String s){}
        public void onProviderDisabled(String s){}
        @Override
        public void onLocationChanged(final Location location) {
            ubicacionLoc = location;
            ubicacion = String.valueOf(location.getLatitude()) + " " + String.valueOf(location.getLongitude());
            System.out.println(ubicacion);

        }
    };
    /**
     * Variable encargada de proveer datos sobre la posición actual.
     */
    private LocationManager mLocationManager;

    /**
     * Constructor de la actividad de modificación de perfil.
     * Inicializa las variables username y token.
     * Inicializa las variables necesarias para obtener la ubicación actual.
     * Pide al servidor la información del usuario y la muestra para ser modificada.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
        setContentView(R.layout.activity_profile_settings);
        setTheme(R.style.GreyText);

        username = getIntent().getStringExtra("username");
        token = getIntent().getStringExtra("token");
        ((TextView)findViewById(R.id.username)).setText(username);
        mLocationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000,
                    50, mLocationListener);
        } catch (SecurityException e){}
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
            ((TextView)findViewById(R.id.location)).setText(ubicacion);
        } catch (JSONException e){
            System.out.println("Error en la solicitud de datos del usuario");
        }
    }

    /**
     * Lee la información ingresada por el usuario y la envía al server para que sea modificada.
     * @param view
     */
    public void saveChanges(View view){
        System.out.println("save changes");
        try {
            email = ((EditText)findViewById(R.id.email)).getText().toString();
            name = ((EditText)findViewById(R.id.name)).getText().toString();
            int permission = ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION);

            if (permission != PackageManager.PERMISSION_GRANTED) {
                // We don't have permission so prompt the user
                ActivityCompat.requestPermissions(
                        this,
                        new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, PERMISSION_ACCESS_FINE_LOCATION
                );
            }
            ubicacionLoc = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if (ubicacionLoc != null) {
                ubicacion = String.valueOf(ubicacionLoc.toString());
            }

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

    /**
     * Cambia la imagen de perfil del usuario.
     * No se comunica con el server, el cambio solo persiste si se pide guardar los cambios.
     * @param view
     */
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

    /**
     * Método encargado de manejar la finalización de actividades.
     * Solo se llama al seleccionar una nueva imagen de perfil.
     * Actualiza la imagen de perfil.
     * @param requestCode El código de la actividad que finalizó.
     * @param resultCode El código resultado.
     * @param data Los datos resultados.
     */
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
                        Bitmap decodedByte = BitmapFactory.decodeByteArray(arrayB, 0, arrayB.length);
                        if (decodedByte == null){
                            Toast.makeText(getApplicationContext(), "Invalid file", Toast.LENGTH_SHORT).show();
                        } else {
                            ((ImageView) findViewById(R.id.profilePicture)).setImageBitmap(decodedByte);
                            picture = new String(Base64.encode(arrayB, Base64.DEFAULT));
                        }
                    } catch (Exception e){


                    }
                }
                return;
        }
    }

    /**
     * Pregunta al usuario si está seguro de que quiere eliminar su cuenta.
     * En caso afirmativo, pide al server que la elimine y vuelve a NavigationActivity
     * con un código que indica lo que ocurrió.
     * @param view
     */
    public void deleteUser(View view){
        new AlertDialog.Builder(this)
                .setTitle("Delete user")
                .setMessage("Are you sure you want to delete your account?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Request request = new Request("DELETE", "/users/" + username);
                        request.setHeader("conn_token", token);
                        request.send();
                        setResult(-1, null);
                        finish();
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
