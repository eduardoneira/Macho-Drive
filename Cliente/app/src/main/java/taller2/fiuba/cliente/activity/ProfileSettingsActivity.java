package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;


import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Permissions;
import taller2.fiuba.cliente.model.Request;

/**
 * Actividad de modificacion de perfil.
 */
public class ProfileSettingsActivity extends AppCompatActivity {
    private String username, token;
    private String name, email, ubicacion, picture;
    private static final int PICKFILE_RESULT_CODE = 101;
    private Location ubicacionLoc;
    /**
     * Variable encargada de ir actualizando la posicion actual.
     * Actualiza {@link #ubicacionLoc} y {@link #ubicacion}
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
        }
    };

    /**
     * Variable encargada de proveer datos sobre la posicion actual.
     */
    private LocationManager mLocationManager;

    /**
     * Constructor de la actividad de modificacion de perfil.
     * Inicializa {@link #username}, {@link #token}., {@link #name}, {@link #email}, {@link #picture}, {@link #ubicacion}
     * Inicializa las variables {@link #mLocationListener} y {@link #mLocationManager}
     * Pide al servidor la informacion del usuario y la muestra para ser modificada.
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("ProfileSettingsActivity","Se crea la actividad");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_profile_settings);
        setTheme(R.style.GreyText);
        String quota;
        username = getIntent().getStringExtra("username");
        token = getIntent().getStringExtra("token");
        ((TextView)findViewById(R.id.username)).setText(username);
        mLocationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000,
                    50, mLocationListener);
        } catch (SecurityException e){}
        try {
            Log.d("ProfileSettingsActivity", "Se pide la informacion del usuario");
            Request request = new Request("GET", "/users/" + username);
            request.setHeader("conn_token", token);
            JSONObject response = request.send();
            name = response.getString("name");
            email = response.getString("email");
            ubicacion = response.getString("ultima_ubicacion");
            int cuotaActual = Integer.parseInt(response.getString("cuota_actual").split("\\.")[0]);
            cuotaActual /= 1000;
            int cuotaMax = Integer.parseInt(response.getString("cuota_max").split("\\.")[0]);
            cuotaMax /= 1000;
            quota = new Integer(cuotaActual).toString()+"/"+new Integer(cuotaMax).toString() + " kb";
            picture = response.getString("picture");
            if(picture.isEmpty()){
                Log.i("ProfileSettingsActivity", "El usuario no tiene imagen, se muestra la predeterminada");
                ((ImageView) findViewById(R.id.profilePicture)).setImageResource(R.drawable.machoke);
            } else{
                byte[] decodedString = Base64.decode(picture, Base64.DEFAULT);
                Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);
                ((ImageView) findViewById(R.id.profilePicture)).setImageBitmap(decodedByte);
            }
            ((EditText)findViewById(R.id.name)).setText(name);
            ((EditText)findViewById(R.id.email)).setText(email);
            ((TextView)findViewById(R.id.location)).setText(ubicacion);
            ((TextView)findViewById(R.id.quota)).setText(quota);
        } catch (JSONException e){}
    }

    /**
     * Lee la informacion ingresada por el usuario y la envía al server para que sea modificada.
     * Se actualiza la {@link #ubicacion}
     * @param view
     */
    public void saveChanges(View view){
        Log.d("ProfileSettingsActivity", "Se presiono Save Changes");
        try {
            email = ((EditText)findViewById(R.id.email)).getText().toString();
            name = ((EditText)findViewById(R.id.name)).getText().toString();
            Permissions.verifyLocationPermissions(this);
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
            Toast.makeText(getApplicationContext(), "Changes saved", Toast.LENGTH_SHORT).show();
        } catch (JSONException e){} catch (SecurityException e){}
    }

    /**
     * Cambia la imagen de perfil del usuario ({@link #picture})
     * No se comunica con el server, el cambio solo persiste si se pide guardar los cambios.
     * @param view
     */
    public void changePicture(View view){
        Log.d("ProfileSettingsActivity", "Se presiono la imagen de perfil");
        Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
        fileintent.addCategory(Intent.CATEGORY_OPENABLE);
        fileintent.setType("*/*");
        try {
            Log.d("ProfileSettingsActivity", "Se abre el navegador de archivos");
            startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
        } catch (ActivityNotFoundException e) {}
    }

    /**
     * Metodo encargado de manejar la finalizacion de actividades.
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
                Log.d("ProfileSettingsActivity", "Se salio del navegador de archivos");
                if (resultCode == RESULT_OK) {
                    Uri FilePath = data.getData();
                    File file = new File(Environment.getExternalStorageDirectory().toString(), (FilePath.getPath()).split(":")[1]);
                    try {
                        byte[] arrayB = new byte[(int) file.length()];
                        FileInputStream fis = new FileInputStream(file);
                        fis.read(arrayB);
                        fis.close();
                        Bitmap decodedByte = BitmapFactory.decodeByteArray(arrayB, 0, arrayB.length);
                        if (decodedByte == null){
                            Log.i("ProfileSettingsActivity", "Se selecciono un archivo invalido");
                            Toast.makeText(getApplicationContext(), "Invalid file", Toast.LENGTH_SHORT).show();
                        } else {
                            ((ImageView) findViewById(R.id.profilePicture)).setImageBitmap(decodedByte);
                            picture = new String(Base64.encode(arrayB, Base64.DEFAULT));
                            Log.d("ProfileSettingsActivity", "Se actualizo la imagen mostrada");
                        }
                    } catch (IOException e){}
                }
                return;
        }
    }

    /**
     * Pregunta al usuario si esta seguro de que quiere eliminar su cuenta.
     * En caso afirmativo, pide al server que la elimine y vuelve a {@link NavigationActivity}
     * con codigo -1.
     * @param view
     */
    public void deleteUser(View view){
        Log.d("ProfileSettingsActivity", "Se presiono Delete User");
        new AlertDialog.Builder(this)
                .setTitle("Delete user")
                .setMessage("Are you sure you want to delete your account?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Log.d("ProfileSettingsActivity", "El usuario esta seguro de querer eliminar su cuenta");
                        Request request = new Request("DELETE", "/users/" + username);
                        request.setHeader("conn_token", token);
                        request.send();
                        setResult(-1, null);
                        Log.d("ProfileSettingsActivity", "Se elimino la cuenta");
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
