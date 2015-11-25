package taller2.fiuba.cliente.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;

import java.io.File;
import java.io.FilenameFilter;

import taller2.fiuba.cliente.R;


/*
 * ESTO NO SE SI DEBERIA SER UNA ACTIVITY O SIMPLEMENTE UNA CLASE ABSTRACTA DE JAVA, PQ NO ME IMPORTA
 * NI EL LAYOUT NI NADA MAS QUE HEREDAR ESTAS FUNCIONALIDADES
 *
 * Credito de aca: http://stackoverflow.com/questions/18522784/android-configurable-directory/18523047
 */
public abstract class FileChooserActivity extends AppCompatActivity {

    //global variables
    static private File[] fileList;
    static private String[] filenameList;

    private File[] loadFileList(String directory, final boolean onlyImages) {
        File path = new File(directory);

        if(path.exists()) {
            FilenameFilter filter = new FilenameFilter() {
                public boolean accept(File dir, String filename) {
                    //add some filters here, for now return true to see all files
                    //File file = new File(dir, filename);
                    //return filename.contains(".txt") || file.isDirectory();
                    File file = new File(dir, filename);
                    if(onlyImages){
                        return filename.contains(".jpg") ||
                                filename.contains(".png") ||
                                filename.contains(".jpeg") ||
                                file.isDirectory();
                    }
                    return true;
                }
            };

            //if null return an empty array instead
            File[] list = path.listFiles(filter);
            return list == null ? new File[0] : list;
        } else {
            return new File[0];
        }
    }

    protected void showFileListDialog(final String directory, final Context context, final boolean onlyImages) {
        Dialog dialog = null;
        AlertDialog.Builder builder = new AlertDialog.Builder(context);

        File[] tempFileList = loadFileList(directory, onlyImages);

        //if directory is root, no need to up one directory
        if(directory.equals("/")) {
            fileList = new File[tempFileList.length];
            filenameList = new String[tempFileList.length];

            //iterate over tempFileList
            for(int i = 0; i < tempFileList.length; i++) {
                fileList[i] = tempFileList[i];
                filenameList[i] = tempFileList[i].getName();
            }
        } else {
            fileList = new File[tempFileList.length+1];
            filenameList = new String[tempFileList.length+1];

            //add an "up" option as first item
            fileList[0] = new File(upOneDirectory(directory));
            filenameList[0] = "..";

            //iterate over tempFileList
            for(int i = 0; i < tempFileList.length; i++) {
                fileList[i+1] = tempFileList[i];
                filenameList[i+1] = tempFileList[i].getName();
            }
        }

        builder.setTitle("Choose your file: " + directory);

        builder.setItems(filenameList, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                File chosenFile = fileList[which];
                if (chosenFile.isDirectory()) {
                    showFileListDialog(chosenFile.getAbsolutePath(), context, onlyImages);
                } else {
                    Log.d("NavigationActivity", "Se selecciono un archivo valido");
                    OnSelectFileAction(chosenFile.getAbsolutePath());
                }
            }
        });

        dialog = builder.create();
        dialog.show();
    }

    private String upOneDirectory(String directory) {
        String[] dirs = directory.split(File.separator);
        StringBuilder stringBuilder = new StringBuilder("");

        for(int i = 0; i < dirs.length-1; i++) {
            stringBuilder.append(dirs[i]).append(File.separator);
        }

        return stringBuilder.toString();
    }

    protected abstract void OnSelectFileAction(String file);

}
