package taller2.fiuba.cliente.model;


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

    /**
     * Constructor. Inicializa los parametros ingresados.
     * @param method Método de la request.
     * @param path Ruta de la request.
     * @param data Datos de la request.
     */
    public Request(String method, String path, JSONObject data) {
        this.method = method;
        this.path = path;
        this.data = data;
        try {
            this.url = new URL("http://10.0.2.2:8000" + path);
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
        Thread t = new Thread(new Runnable() {
            public void run() {
                try {
                    if ((method == "PUT" || method == "POST") && (data != null)) {
                        urlConnection.setDoOutput(true);
                        urlConnection.setRequestProperty("Content-Type", "application/json");
                        OutputStreamWriter wr = new OutputStreamWriter(urlConnection.getOutputStream());
                        wr.write(data.toString());
                        wr.flush();
                        wr.close();
                    }
                    if (urlConnection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                        response = new JSONObject();
                        InputStream is = urlConnection.getInputStream();
                        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                        String json, line;
                        StringBuffer buffer = new StringBuffer();
                        while ((line = reader.readLine()) != null) {
                            buffer.append(line);
                        }
                        if (buffer.length() != 0) {

                            json = buffer.toString();
                            JSONTokener tokener = new JSONTokener(json);
                            response = new JSONObject(tokener);
                        }
                    } else {
                        Map fail = new HashMap();
                        fail.put("status", "fail");
                        response = new JSONObject(fail);
                    }
                } catch (Exception e) {
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
