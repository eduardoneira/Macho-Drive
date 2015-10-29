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


    public Request(String method, String path, JSONObject data) {
        this.method = method;
        this.path = path;
        this.data = data;
    }

    public Request(String method, String path) {
        this.method = method;
        this.path = path;
        this.data = null;
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

    public JSONObject send() {
        Thread t = new Thread(new Runnable() {
            public void run() {
                try {
                    URL url = new URL("http://10.0.2.2:8000" + path);
                    HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();

                    urlConnection.setRequestMethod(method);
                    urlConnection.setRequestProperty("Connection", "close");
                    if (data != null) {
                        if (method == "PUT" || method == "POST") {
                            urlConnection.setDoOutput(true);
                        }
                        urlConnection.setRequestProperty("Content-Type", "application/json");
                        OutputStreamWriter wr = new OutputStreamWriter(urlConnection.getOutputStream());
                        wr.write(data.toString());
                        wr.flush();
                    }
                    if (urlConnection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                        response = new JSONObject();
                        System.out.print("content: ");
                        System.out.println(urlConnection.getContent());
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
