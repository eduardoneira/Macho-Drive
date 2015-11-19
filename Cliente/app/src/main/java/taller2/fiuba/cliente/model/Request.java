package taller2.fiuba.cliente.model;


import android.content.res.Resources;
import android.util.Log;

import org.json.JSONObject;
import org.json.JSONTokener;


import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import taller2.fiuba.cliente.R;

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
    public Request(String method, String path, JSONObject data) {
        Log.d("Request", "Se crea la request");
        this.method = method;
        this.path = path;
        this.data = data;
        try {

            if (server == "") {
                server = "http://10.0.2.2:8000";
            }
            Log.d("Request", "La ip a la que se envian la request es "+server);
            this.url = new URL(server + path);
            this.urlConnection = (HttpURLConnection) this.url.openConnection();
            this.urlConnection.setRequestMethod(method);
            this.urlConnection.setRequestProperty("Connection", "close");
        } catch (Exception e){}
    }

    /**
     * Constructor para requests que no envían datos.
     * @param method Método de la request.
     * @param path Ruta de la request.
     */
    public Request(String method, String path) {
        this(method, path, null);
    }

    public JSONObject getData() {
        return data;
    }

    /**
     * Ingresa el header solicitado a la request.
     * @param header Key del header
     * @param content Value del header
     */
    public void setHeader(String header, String content){
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
                    Log.d("Request", "Se envia " + method + " a " + "path");
                    Log.d("Request", "Con datos " + data.toString());
                    if ((method == "PUT" || method == "POST") && (data != null)) {
                        Log.d("Request", "Se quiere enviar datos");
                        urlConnection.setDoOutput(true);
                        urlConnection.setRequestProperty("Content-Type", "application/json");
                        OutputStreamWriter wr = new OutputStreamWriter(urlConnection.getOutputStream());
                        Log.d("Request", "Se escriben los datos");
                        wr.write(data.toString());
                        wr.flush();
                        wr.close();
                    }
                    if (urlConnection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                        Log.d("Request", "Se recibio codigo OK");
                        response = new JSONObject();
                        InputStream is = urlConnection.getInputStream();
                        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                        String json, line;
                        StringBuffer buffer = new StringBuffer();
                        while ((line = reader.readLine()) != null) {
                            Log.d("Request", "Se recibio la linea: "+line);
                            buffer.append(line);
                        }
                        if (buffer.length() != 0) {
                            json = buffer.toString();
                            JSONTokener tokener = new JSONTokener(json);
                            response = new JSONObject(tokener);
                        }
                    } else {
                        Log.d("Request", "No se recibio codigo OK");
                        Map fail = new HashMap();
                        fail.put("status", "fail");
                        response = new JSONObject(fail);
                    }
                } catch (Exception e) {
                    Log.w("Request", "Hubo un error en el procesamiento de la request");
                    e.printStackTrace();
                    Map fail = new HashMap();
                    fail.put("status", "fail");
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
