package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
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

    public JSONObject getFile(String filename) {
        Request request = new Request("GET", "/files/" + getActivity().getIntent().getStringExtra("username") + "/" + filename);
        request.setHeader("conn_token", getActivity().getIntent().getStringExtra("token"));
        return request.send();
    }

    public void deletefile(final String filename) {
        new AlertDialog.Builder(getActivity())
                .setTitle("Delete file")
                .setMessage("Are you sure you want to delete this file?")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Request request = new Request("DELETE", "/files/" + getActivity().getIntent().getStringExtra("username") + "/" + filename);
                        request.setHeader("conn_token", getActivity().getIntent().getStringExtra("token"));
                        request.send();
                    }
                })
                .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                    }
                })
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();

    }

    public void modifyFile(String filename) {
        Intent modifyFileActivity = new Intent(getContext(), ModifyFileActivity.class);
        modifyFileActivity.putExtra("token", getActivity().getIntent().getStringExtra("token"));
        modifyFileActivity.putExtra("filename", filename);
        modifyFileActivity.putExtra("username", getActivity().getIntent().getStringExtra("username"));
        startActivity(modifyFileActivity);
    }

    public void shareFile(String filename) {
        Intent shareFileActivity = new Intent(getContext(), ShareFileActivity.class);
        shareFileActivity.putExtra("token", getActivity().getIntent().getStringExtra("token"));
        shareFileActivity.putExtra("filename", filename);
        shareFileActivity.putExtra("username", getActivity().getIntent().getStringExtra("username"));
        startActivity(shareFileActivity);
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        final CharSequence[] items = {"Download", "Edit details", "Delete", "Share", "Versions"};
        builder.setTitle("Choose an option")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        // The 'which' argument contains the index position
                        // of the selected item
                        if (which == 0) {
                            JSONObject response = getFile((String) getArguments().get("filename"));
                            try {

                                byte[] content = ((JSONArray) response.get("content")).get(0).toString().getBytes();
                                byte[] bytes = Base64.decode(content, Base64.DEFAULT);
                                verifyStoragePermissions(getActivity());
                                File file = new File("/mnt/sdcard/Download/" + getArguments().get("filename"));
                                FileOutputStream fop = new FileOutputStream(file);
                                fop.write(bytes);
                                fop.flush();
                                fop.close();
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                            System.out.println("get");
                            System.out.println(response);
                        } else if (which == 1) {
                            modifyFile((String) getArguments().get("filename"));
                            System.out.println("modify");

                        } else if (which == 2) {
                            deletefile((String) getArguments().get("filename"));
                            System.out.println("delete");
                        } else {
                            shareFile((String) getArguments().get("filename"));
                            System.out.println("share");
                        }

                    }
                });
        return builder.create();
    }

    /**
     * Checks if the app has permission to write to device storage
     * <p/>
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
