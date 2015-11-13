package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.util.Base64;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.util.logging.Logger;

import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.activity.NavigationActivity;

/**
 * Created by nicolas on 29/10/15.
 */
public class miDialogo extends DialogFragment {

    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

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
                            JSONObject response = getFile((String)getArguments().get("filename"));
                            try {

                                byte[] content = ((JSONArray)response.get("content")).get(0).toString().getBytes();
                                byte[] bytes = Base64.decode(content, Base64.DEFAULT);
                                verifyStoragePermissions(getActivity());
                                File file = new File("/mnt/sdcard/Download/"+getArguments().get("filename"));
                                FileOutputStream fop = new FileOutputStream(file);
                                fop.write(bytes);
                                fop.flush();
                                fop.close();
                            } catch (Exception e ){
                                e.printStackTrace();
                            }
                            System.out.println("get");
                            System.out.println(response);
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

    /**
     * Checks if the app has permission to write to device storage
     *
     * If the app does not has permission then the user will be prompted to grant permissions
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        // Check if we have write permission
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        System.out.println(permission == PackageManager.PERMISSION_GRANTED);
        ActivityCompat.checkSelfPermission(activity, Manifest.permission.READ_EXTERNAL_STORAGE);
        System.out.println(permission == PackageManager.PERMISSION_GRANTED);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{
                            Manifest.permission.READ_EXTERNAL_STORAGE,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE
                    }, PERMISSION_WRITE_EXTERNAL_STORAGE
            );
        }
    }
}
