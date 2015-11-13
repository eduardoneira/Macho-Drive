package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;

import taller2.fiuba.cliente.model.Request;

/**
 * Created by nicolas on 29/10/15.
 */
public class miDialogo extends DialogFragment {

    public JSONObject getFile(String filename){
        Request request = new Request("GET", "/files/"+getActivity().getIntent().getStringExtra("username")+"/"+filename);
        request.setHeader("conn_token", getActivity().getIntent().getStringExtra("token"));
        return request.send();
    }

    public void deletefile(String filename){
        Request request = new Request("DELETE", "/files/"+getActivity().getIntent().getStringExtra("username")+"/"+filename);
        request.setHeader("conn_token", getActivity().getIntent().getStringExtra("token"));
        request.send();
    }

    public void modifyFile(String path) {
        File file = new File(path);
        //Request request = new Request("PUT", "/files/"+getIntent().getStringExtra("username"), );
        //request.send();
    }

    public void shareFile(String archivo){
        //Falta
        return;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        final CharSequence[] items = {"Get", "Modify", "Delete", "Share"};
        builder.setTitle("titulo")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        // The 'which' argument contains the index position
                        // of the selected item
                        if (which == 0){
                            getFile((String)getArguments().get("filename"));
                            System.out.println("get");
                        } else if (which == 1){
                            //modify archivo
                            System.out.println("modify");

                        } else if(which == 2) {
                            deletefile((String)getArguments().get("filename"));
                            System.out.println("delete");
                        } else {
                            shareFile((String)getArguments().get("filename"));
                            System.out.println("share");
                        }

                    }
                });
        return builder.create();
    }
}
