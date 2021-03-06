==================================================================================
Gestión
==================================================================================

Como sistema de control de versiones empleamos la herramienta Git. En particular, GitHub. Dentro de este sistema, aprovechamos el sistema de ticketing para mantener un desarrollo controlado y ordenado.


Workflow
==================================================================================

Control de versiones
-----------------------------------------------------------------------------

Dentro de la herramienta Git, se empleó el sistema de branches para poder mantener el desarrollo del proyecto separado y poder mantener una independencia entre las distintas partes. Para esto se crearon, además de la branch 'master', otras dos branches más llamadas 'cliente-dev' y 'server-dev'. Sobre cliente-dev se iban agregando las distintas funcionalidades del cliente, mientras que en server-dev ocurría lo mismo para el servidor.


Ticketing
-----------------------------------------------------------------------------

También se implementó el sistema de ticketing para poder marcar claramente las tareas que se debían a realizar y a su vez se evidenciaba qué tareas ya estaban concluídas.


Integración continua
-----------------------------------------------------------------------------

A su vez, para mantener un entorno de integración continua se empleó Travis, conectado al repositorio en github, lo cual permitía continuamente correr todas las unittests que ibamos colocando.


