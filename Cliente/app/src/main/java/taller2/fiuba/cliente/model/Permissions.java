package taller2.fiuba.cliente.model;


import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;

public class Permissions {
    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    private static final int PERMISSION_ACCESS_FINE_LOCATION = 106;

    /**
     * Chequea si la aplicación tiene permiso para escribir el almacenamiento externo.
     *
     * Si la aplicación no tiene permiso, se le pide al usuario que lo conceda.
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION_WRITE_EXTERNAL_STORAGE
            );
        }
    }

    public static void verifyLocationPermissions(Activity activity){
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.ACCESS_FINE_LOCATION);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, PERMISSION_ACCESS_FINE_LOCATION
            );
        }
    }
}
