#!/bin/bash

echo "Checkeando existencia de docker"
if ! which docker > /dev/null; then
	echo "Docker no se encuentra instalado. Se precisa de su instalacion para poder correr el script. Consultar http://docs.docker.com/engine/installation/ubuntulinux/ para poder instalarlo"
	exit
fi

echo "Docker se encuentra instalado. Procediendo a ejecutar server como daemon"
#sudo docker pull eduardoneira/taller_2_server
sudo docker run -d --add-host="localhost:10.0.0.2" eduardoneira/taller_2_server bash /home/user/Macho-Drive/server_install.sh

echo "ID proceso:"
sudo docker ps | grep "eduardoneira/taller_2_server"

echo "IP a conectarse: 172.17.0.2:8000"

echo "Para detener el server correr :~$ sudo docker stop <container-id>"
 
