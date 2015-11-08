#!/bin/bash

cd server/lib/RocksDB
unzip rocksdb-master.zip > /dev/null 2>&1
cd rocksdb-master
make static_lib
mv librocksdb.a ../
cd ../../../
