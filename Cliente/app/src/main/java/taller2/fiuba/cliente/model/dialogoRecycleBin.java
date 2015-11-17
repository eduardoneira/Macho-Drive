package taller2.fiuba.cliente.model;

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
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.util.logging.Logger;

import taller2.fiuba.cliente.activity.ModifyFileActivity;
import taller2.fiuba.cliente.activity.RecycleBinActivity;
import taller2.fiuba.cliente.activity.ShareFileActivity;

/**
 * Created by nicolas on 16/11/15.
 */
public class dialogoRecycleBin extends DialogFragment {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

    /**
     * Dialogo de la actividad correspondiente a la papelera de reciclaje
     *
     * @param savedInstanceState
     * @return Dialogo con un único elemento, Restore
     */
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        final CharSequence[] items = {"Restore"};
        builder.setTitle("")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        // The 'which' argument contains the index position
                        // of the selected item
                        if (which == 0) {
                            try {
                                Request request = new Request("PUT", "/files/" + getActivity().getIntent().getStringExtra("username") + "/recycle_bin/" + (String) getArguments().get("filename"));
                                request.setHeader("conn_token", getActivity().getIntent().getStringExtra("token"));
                                request.send();
                                Toast.makeText(getActivity().getApplicationContext(), "File successfully restored", Toast.LENGTH_SHORT).show();

                                ((RecycleBinActivity)getActivity()).actualizarArchivos();
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }

                    }
                });
        return builder.create();
    }



}
