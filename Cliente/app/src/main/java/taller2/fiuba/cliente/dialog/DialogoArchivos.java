package taller2.fiuba.cliente.dialog;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.util.Base64;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.util.logging.Logger;

import taller2.fiuba.cliente.activity.FileVersionsActivity;
import taller2.fiuba.cliente.activity.ModifyFileActivity;
import taller2.fiuba.cliente.activity.ShareFileActivity;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.activity.NavigationActivity;
import taller2.fiuba.cliente.model.Permissions;

/**
 * Dialogo con cinco opciones: Download, Edit Details, Delete, Share y Versions
 */
public class DialogoArchivos extends DialogFragment {

    Activity activity;

    /**
     * Guarda una referencia a la {@link #activity} que lo abrió.
     * @param activity La actividad que abrió el diálogo
     */
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        this.activity=activity;
    }

    /**
     * Pide el archivo solicitado al server.
     * @param filename Archivo a pedir
     * @return Respuesta del server
     */
    public JSONObject getFile(String filename) {
        Request request = new Request("GET", "/files/" + getActivity().getIntent().getStringExtra("username") + "/" + filename);
        request.setHeader("conn_token", activity.getIntent().getStringExtra("token"));
        return request.send();
    }

    /**
     * Pregunta al usuario si está seguro de querer eliminar el archivo.
     * En caso afirmativo, pide al server que lo mande a la papelera de reciclaje.
     * @param filename Archivo a ser eliminado
     */
    public void deletefile(final String filename) {
        new AlertDialog.Builder(getActivity())
                .setTitle("Delete file")
                .setMessage("Are you sure you want to delete this file?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Request request = new Request("DELETE", "/files/" + activity.getIntent().getStringExtra("username") + "/" + filename);
                        request.setHeader("conn_token", activity.getIntent().getStringExtra("token"));
                        request.send();
                        ((NavigationActivity)activity).actualizarArchivos();
                    }
                })
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();

    }

    /**
     * Inicia {@link ModifyFileActivity}.
     * @param filename El nombre del archivo cuyos detalles se desea editar
     */
    public void modifyFile(String filename) {
        Intent modifyFileActivity = new Intent(getContext(), ModifyFileActivity.class);
        modifyFileActivity.putExtra("token", activity.getIntent().getStringExtra("token"));
        modifyFileActivity.putExtra("filename", filename);
        modifyFileActivity.putExtra("username", activity.getIntent().getStringExtra("username"));
        startActivity(modifyFileActivity);
    }

    /**
     * Inicia {@link ShareFileActivity}.
     * @param filename El nombre del archivo que se desea compartir/descompartir.
     */
    public void shareFile(String filename) {
        Intent shareFileActivity = new Intent(getContext(), ShareFileActivity.class);
        shareFileActivity.putExtra("token", activity.getIntent().getStringExtra("token"));
        shareFileActivity.putExtra("filename", filename);
        shareFileActivity.putExtra("username", activity.getIntent().getStringExtra("username"));
        startActivity(shareFileActivity);
    }

    /**
     * Inicia {@link FileVersionsActivity}
     * @param filename El nombre del archivo cuyas versiones se desean ver
     */
    public void fileVersions(String filename){
        Intent versionsActivity = new Intent(getContext(), FileVersionsActivity.class);
        versionsActivity.putExtra("token", activity.getIntent().getStringExtra("token"));
        versionsActivity.putExtra("filename", filename);
        versionsActivity.putExtra("username", activity.getIntent().getStringExtra("username"));
        startActivity(versionsActivity);
    }

    /**
     * Muestra las distintas opciones al clickear un archivo:
     * Download, Edit details, Delete, Share, Versions.
     * Si se presionó Download, se descarga el archivo en cuestión, pidiéndose permisos
     * en caso de ser necesario.
     * Si se presionó Edit details, se llama a {@link #modifyFile}.
     * Si se presionó Delete, se llama a {@link #deletefile}.
     * Si se presionó Share, se llama a {@link #shareFile}.
     * @param savedInstanceState
     * @return El diálogo en cuestión
     */
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        final CharSequence[] items = {"Download", "Edit details", "Delete", "Share", "Versions"};
        builder.setTitle("Choose an option")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        if (which == 0) {
                            JSONObject response = getFile((String) getArguments().get("filename"));
                            try {

                                byte[] content = ((JSONArray) response.get("content")).get(0).toString().getBytes();
                                byte[] bytes = Base64.decode(content, Base64.DEFAULT);
                                Permissions.verifyStoragePermissions(getActivity());
                                File file = new File("/mnt/sdcard/Download/" + getArguments().get("filename"));
                                FileOutputStream fop = new FileOutputStream(file);
                                fop.write(bytes);
                                fop.flush();
                                fop.close();
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        } else if (which == 1) {
                            modifyFile((String) getArguments().get("filename"));
                        } else if (which == 2) {
                            deletefile((String) getArguments().get("filename"));
                        } else if (which == 3){
                            shareFile((String) getArguments().get("filename"));
                        } else {
                            fileVersions((String) getArguments().get("filename"));
                        }

                    }
                });
        return builder.create();
    }
}
