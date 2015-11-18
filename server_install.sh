#!/bin/bash


echo "Comenzando instalación Macho-Drive"
cd /home/user/Macho-Drive/

if [[ $(ls server/lib/RocksDB/ | grep "librocksdb.a") == "" ]] 
then
	echo "No se encontro rocksdb, instalando rocksdb"
	./unpack_rocksdb.sh
	echo "Rocksdb se ha instalado"
fi

cd server/
echo "Comenzando compilacíon Macho-drive"
./build.sh

cd build
echo "Macho-Drive comenzando"
./Server
