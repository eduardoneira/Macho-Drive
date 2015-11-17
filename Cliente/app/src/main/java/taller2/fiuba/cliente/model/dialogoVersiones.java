package taller2.fiuba.cliente.model;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.util.Base64;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.util.logging.Logger;

import taller2.fiuba.cliente.activity.FileVersionsActivity;
import taller2.fiuba.cliente.activity.RecycleBinActivity;

/**
 * Created by nicolas on 17/11/15.
 */
public class dialogoVersiones extends DialogFragment {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);
    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    private Activity activity;

    /**
     * Dialogo de la actividad correspondiente a la papelera de reciclaje
     *
     * @param savedInstanceState
     * @return Dialogo con un único elemento, Restore
     */
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        final CharSequence[] items = {"Download", "Delete"};
        activity = getActivity();
        builder.setTitle("")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        // The 'which' argument contains the index position
                        // of the selected item
                        if (which == 0) {
                            try {

                                downloadVersion(getArguments().getString("content"), getArguments().getString("filename"));

                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                        if (which == 1){
                            try {
                                deleteVersion(getArguments().getString("username"), getArguments().getInt("version"));
                            } catch (Exception e){
                                e.printStackTrace();
                            }
                        }

                    }
                });
        return builder.create();
    }

    public void deleteVersion(final String username, final int version){
        new AlertDialog.Builder(getActivity())
                .setTitle("Delete version")
                .setMessage("Are you sure you want to delete this version?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            JSONObject data = new JSONObject();
                            data.put("owner_username", username);
                            JSONArray versionAEliminar = new JSONArray();
                            versionAEliminar.put(version);
                            data.put("delete_versions", versionAEliminar );
                            Request request = new Request("PUT", "/files/"+username+"/"+getArguments().getString("filename"), data);
                            request.setHeader("conn_token", getArguments().getString("token"));
                            request.send();
                            ((FileVersionsActivity)activity).mostrarVersiones();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }})
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
    }


    public void downloadVersion(final String contenido, final String filename){
        new AlertDialog.Builder(getActivity())
                .setTitle("Download version")
                .setMessage("Are you sure you want to download this version?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            byte[] content = contenido.getBytes();
                            byte[] bytes = Base64.decode(content, Base64.DEFAULT);
                            verifyStoragePermissions(activity);
                            File file = new File("/mnt/sdcard/Download/" + filename);
                            FileOutputStream fop = new FileOutputStream(file);
                            fop.write(bytes);
                            fop.flush();
                            fop.close();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }})
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
    }

    /**
     * Chequea si la aplicación tiene permiso para escribir el almacenamiento externo.
     *
     * Si la aplicación no tiene permiso, se le pedirá al usuario que lo conceda.
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        // Check if we have write permission
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION_WRITE_EXTERNAL_STORAGE
            );
        }
    }

}
