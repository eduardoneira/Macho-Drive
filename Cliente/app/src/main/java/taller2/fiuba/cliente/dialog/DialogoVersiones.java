package taller2.fiuba.cliente.dialog;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Base64;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.util.logging.Logger;

import taller2.fiuba.cliente.activity.FileVersionsActivity;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.Permissions;

/**
 * Dialogo de activitada de versiones.
 * Tiene dos elementos: Download y Delete.
 */
public class DialogoVersiones extends DialogFragment {

    private Activity activity;

    /**
     * Se crea el dialogo para {@link FileVersionsActivity}
     * Se llama a {@link #downloadVersion(String, String)} o a {@link #deleteVersion(String, int)}
     * @param savedInstanceState
     * @return
     */
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        final CharSequence[] items = {"Download", "Delete"};
        activity = getActivity();
        builder.setTitle("")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        if (which == 0) {
                            try {
                                downloadVersion(getArguments().getString("content"), getArguments().getString("filename"));
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                        if (which == 1){
                            try {
                                System.out.println(getArguments().getInt("version"));
                                deleteVersion(getArguments().getString("username"), getArguments().getInt("version"));
                            } catch (Exception e){
                                e.printStackTrace();
                            }
                        }
                    }
                });
        return builder.create();
    }

    /**
     * Se elimina la version seleccionada.
     * Primero se le pregunta al usuario si esta seguro.
     * Actualiza las versiones mostradas en la actividad madre.
     * @param username
     * @param version
     */
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

    /**
     * Se descarga la version seleccionada.
     * Primero se le pregunta al usuario si esta seguro.
     * @param contenido Contenido de la version que se desea descargar
     * @param filename Nombre del archivo a descargar
     */
    public void downloadVersion(final String contenido, final String filename){
        new AlertDialog.Builder(getActivity())
                .setTitle("Download version")
                .setMessage("Are you sure you want to download this version?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            byte[] content = contenido.getBytes();
                            byte[] bytes = Base64.decode(content, Base64.DEFAULT);
                            Permissions.verifyStoragePermissions(activity);
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
}
