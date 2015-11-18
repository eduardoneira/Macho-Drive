package taller2.fiuba.cliente.model;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.widget.Toast;

import java.util.logging.Logger;

import taller2.fiuba.cliente.activity.RecycleBinActivity;

/**
 * Created by nicolas on 16/11/15.
 */
public class DialogoPapeleraDeReciclaje extends DialogFragment {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

    /**
     * Dialogo de la actividad correspondiente a la papelera de reciclaje
     *
     * @param savedInstanceState
     * @return Dialogo con un unico elemento, Restore
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

                                ((RecycleBinActivity)getActivity()).actualizarArchivosEnPapelera();
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }

                    }
                });
        return builder.create();
    }



}
