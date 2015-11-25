package taller2.fiuba.cliente.model;


import android.content.SharedPreferences;
import android.content.res.Resources;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONObject;
import org.json.JSONTokener;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.activity.MainActivity;

/**
 * Clase que representa una request HTTP con formato REST.
 */
public class Request {

    /**
     * Ruta de la request.
     */
    private String path;
    /**
     * Metodo de la request.
     */
    private String method;
    /**
     * Datos de la request.
     */
    private JSONObject data;
    /**
     * Respuesta del servidor.
     */
    private JSONObject response;
    private HttpURLConnection urlConnection;
    private URL url;
    public static String server;

    /**
     * Constructor. Inicializa los parametros ingresados.
     * @param method Método de la request.
     * @param path Ruta de la request.
     * @param data Datos de la request.
     */
    public Request(String method, String path, JSONObject data){
        Log.d("Request", "Se crea la request");
        this.method = method;
        this.path = path;
        this.data = data;
        setServer();
        try {
            this.url = new URL(server + path);
            this.urlConnection = (HttpURLConnection) this.url.openConnection();
            this.urlConnection.setRequestMethod(method);
            this.urlConnection.setRequestProperty("Connection", "close");
            Log.d("Request", "La ip a la que se envian la request es " + server);
        } catch(IOException e){
            Log.d("Request", "Fallo la creacion del request");
        }
    }

    private void setServer(){
        SharedPreferences SP = PreferenceManager.getDefaultSharedPreferences(MainActivity.getAppContext());
        String ip = SP.getString("ip", "");
        String port = SP.getString("port", "");
        server = "http://" + ip + ":" + port;
    }

    /**
     * Constructor para requests que no envían datos.
     * @param method Método de la request.
     * @param path Ruta de la request.
     */
    public Request(String method, String path){
        this(method, path, null);
    }

    public JSONObject getData() {
        return data;
    }

    public int getStatusCode(){
        int status;
        try{
            setServer();
            status = urlConnection.getResponseCode();
        } catch (IOException e){
            status = -1;
        }
        return status;
    }

    /**
     * Ingresa el header solicitado a la request.
     * @param header Key del header
     * @param content Value del header
     */
    public void setHeader(String header, String content){
        setServer();
        urlConnection.setRequestProperty(header, content);
    }

    /**
     * Envía la request al servidor.
     * En caso de ser exitosa, devuelve la respuesta del servidor.
     * En caso de haber un error, devuelve un JSONObject con el valor "fail" en campo "status".
     *
     * @return La respuesta del servidor
     */
    public JSONObject send() {
        Log.d("Request", "Comienza el envio");
        Thread t = new Thread(new Runnable() {
            public void run() {
                try {
                    Log.d("Request", "Se envia " + method + " a " + path);
                    if ((method == "PUT" || method == "POST") && (data != null)) {
                        Log.d("Request", "Los datos son " + data.toString());
                        Log.d("Request", "Se quiere enviar datos");
                        setServer();
                        urlConnection.setDoOutput(true);
                        urlConnection.setRequestProperty("Content-Type", "application/json");
                        OutputStreamWriter wr = new OutputStreamWriter(urlConnection.getOutputStream());
                        Log.d("Request", "Se escriben los datos");
                        wr.write(data.toString());
                        wr.flush();
                        wr.close();
                    }
                    // como el servidor siempre manda un mensaje de status para el usuario, directo
                    // uso ese en el toast
                    Log.d("Request", "Se recibio codigo " + urlConnection.getResponseCode());
                    InputStream is;
                    if(urlConnection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                        is = urlConnection.getInputStream();
                    } else {
                        is = urlConnection.getErrorStream();
                    }
                    BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                    String json, line;
                    StringBuffer buffer = new StringBuffer();
                    while ((line = reader.readLine()) != null) {
                        Log.d("Request", "Se recibio la linea: " + line);
                        buffer.append(line);
                    }
                    if (buffer.length() != 0) {
                        json = buffer.toString();
                        JSONTokener tokener = new JSONTokener(json);
                        response = new JSONObject(tokener);
                    }
                } catch(ConnectException e){
                    Log.w("Request", "No se pudo conectar al servidor");
                    e.printStackTrace();
                    Map fail = new HashMap();
                    fail.put("status", "Can't connect to server, please check ip and port and try again");
                    response = new JSONObject(fail);
                } catch (Exception e) {
                    Log.w("Request", "Hubo un error no reconocido en el procesamiento de la request");
                    e.printStackTrace();
                    Map fail = new HashMap();
                    fail.put("status", "Unexpected error, please try again");
                    response = new JSONObject(fail);
                }
            }
        });
        try {
            t.start();
            t.join();
        } catch (Exception e) {}
        return response;
    }

}
