#!/bin/bash

dpkg -x "rocksdb_3.3-1.deb" "rocksdb"
mv "rocksdb/usr/local/bin/librocksdb.a" "server/lib/RocksDB/"
