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
                    URL url = new URL("http://10.0.2.2:8000"+path);
                    HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                    urlConnection.setDoOutput(true);
                    InputStream is = urlConnection.getErrorStream();
                    if (is != null) {
                        BufferedReader reader = new BufferedReader(new InputStreamReader(is));

                        String json, line;
                        StringBuilder builder = new StringBuilder();

                        while ((line = reader.readLine()) != null) {
                            builder.append(line);
                        }

                        json = builder.toString();
                        if (json.length() > 0) {
                            JSONTokener tokener = new JSONTokener(json);
                            response = new JSONObject(tokener);
                            response.put("status", "success!");


                        } else {
                            Map success = new HashMap();
                            success.put("status", "success");
                            response = new JSONObject(success);
                        }
                    } else {
                            Map success = new HashMap();
                            success.put("status", "success");
                            response = new JSONObject(success);
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
