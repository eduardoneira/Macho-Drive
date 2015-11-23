===========================================================
Ejecución con Docker
===========================================================

Ya con la imagen bajada, se puede correr el script server_run_docker para ejecutar el server como demonio. El script especifica contra que ip conectarse y como pararlo. Este script se encuentra en Macho-Drive/.

.. code-block:: bash

	$ ./server_run_docker.sh

Si se quiere ver la imagen utilizada pero a través de una consola de bash se puede utilizar el siguiente comando:

.. code-block:: bash

	$ sudo docker run -i -t eduardoneira/taller_2_server:release /bin/bash
