package model;

import org.apache.http.HttpEntity;
import org.apache.http.client.HttpClient;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.BufferedReader;
import java.io.InputStreamReader;


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
                    HttpResponse httpResponse;
                    HttpClient httpClient = HttpClientBuilder.create().build();
                    httpResponse = httpClient.execute(new HttpGet("http://localhost:8080"+path));
                    BufferedReader reader = new BufferedReader(new InputStreamReader(httpResponse.getEntity().getContent(), "UTF-8"));

                    String json, line;
                    StringBuilder builder = new StringBuilder();

                    while ((line = reader.readLine()) != null) {
                        builder.append(line);
                    }

                    json = builder.toString();
                    response = new JSONObject(new JSONTokener(json));

                } catch (Exception e) {}

            }
        });
        try {
            t.start();
            t.join();


        } catch (Exception e) {}
        return response;
    }

}
