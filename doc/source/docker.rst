===========================================================
Ejecución con Docker
===========================================================

El server se puede correr tambien con docker. Para esto es necesario tener docker instalado. De no ser el caso consultar http://docs.docker.com/engine/installation/ubuntulinux/ para instalarlo. Una vez instalado se debe bajar la imagen del server de https://hub.docker.com/r/eduardoneira/taller_2_server/. Esto se puede hacer con el siguiente comando:

.. code-block:: bash

	$ sudo docker pull eduardoneira/taller_2_server:release

Ya con la imagen bajada, se puede correr el script server_run_docker para ejecutar el server como demonio. El script especifica contra que ip conectarse y como pararlo. Este script se encuentra en Macho-Drive/.

.. code-block:: bash

	$ ./server_run_docker.sh

Si se quiere ver la imagen utilizada pero a través de una consola de bash se puede utilizar el siguiente comando:

.. code-block:: bash

	$ sudo docker run -i -t eduardoneira/taller_2_server:release /bin/bash