.. java:import:: org.json JSONObject

.. java:import:: org.json JSONTokener

.. java:import:: java.io BufferedReader

.. java:import:: java.io InputStream

.. java:import:: java.io InputStreamReader

.. java:import:: java.io OutputStreamWriter

.. java:import:: java.net HttpURLConnection

.. java:import:: java.net URL

.. java:import:: java.util HashMap

.. java:import:: java.util Map

Request
=======

.. java:package:: taller2.fiuba.cliente.model
   :noindex:

.. java:type:: public class Request

   Clase que representa una request HTTP con formato REST.

Constructors
------------
Request
^^^^^^^

.. java:constructor:: public Request(String method, String path, JSONObject data)
   :outertype: Request

   Constructor. Inicializa los parametros ingresados.

   :param method: Método de la request.
   :param path: Ruta de la request.
   :param data: Datos de la request.

Request
^^^^^^^

.. java:constructor:: public Request(String method, String path)
   :outertype: Request

   Constructor para requests que no envían datos.

   :param method: Método de la request.
   :param path: Ruta de la request.

Methods
-------
getData
^^^^^^^

.. java:method:: public JSONObject getData()
   :outertype: Request

send
^^^^

.. java:method:: public JSONObject send()
   :outertype: Request

   Envía la request al servidor. En caso de ser exitosa, devuelve la respuesta del servidor. En caso de haber un error, devuelve un JSONObject con el valor "fail" en campo "status".

   :return: La respuesta del servidor

setHeader
^^^^^^^^^

.. java:method:: public void setHeader(String header, String content)
   :outertype: Request

   Ingresa el header solicitado a la request.

   :param header: Key del header
   :param content: Value del header

