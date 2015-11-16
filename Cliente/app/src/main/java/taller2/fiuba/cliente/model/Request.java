package taller2.fiuba.cliente.model;

import org.apache.http.HttpEntity;
import org.apache.http.client.HttpClient;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;


public class Request {
    private String path;
    private String method;
    private JSONObject data;
    private JSONObject response;
    private HttpURLConnection urlConnection;
    private URL url;


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

    public Request(String method, String path) {
        this(method, path, null);
    }

    public String getMethod() {
        return method;
    }

    public String getPath() {
        return path;
    }

    public JSONObject getData() {
        return data;
    }

    public void setHeader(String header, String content){
        urlConnection.setRequestProperty(header, content);
    }

    public JSONObject send() {
        Thread t = new Thread(new Runnable() {
            public void run() {
                try {
                    System.out.println("json a enviar:");
                    System.out.println(data);
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
                        System.out.print("data: ");
                        System.out.println(data);
                        InputStream is = urlConnection.getInputStream();
                        BufferedReader reader = new BufferedReader(new InputStreamReader(is));

                        String json, line;
                        StringBuffer buffer = new StringBuffer();

                        while ((line = reader.readLine()) != null) {
                            buffer.append(line);
                            System.out.print("linea: ");
                            System.out.println(line);
                        }
                        if (buffer.length() != 0) {

                            json = buffer.toString();
                            System.out.print("json: ");
                            System.out.println(json);
                            JSONTokener tokener = new JSONTokener(json);
                            response = new JSONObject(tokener);
                        }



                    } else {
                        System.out.println("Status no ok");
                        System.out.println(urlConnection.getResponseCode());
                        System.out.println(method);
                        System.out.println(path);
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
