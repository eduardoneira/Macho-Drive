----
rocksdb funcionando, hace falta instalar las librerias Bzip, Zlib, Snappy, Pthread y Gflags, y poner los ".a" en sus respectivas carpetas dentro de "lib/". Despues correr "./build.sh"

edit: hay que clonar el repo de RocksDB y correr make para armar el librocksdb.a, que pesa >100mb y no me deja subirlo. Iria en lib/RocksDB/librocksdb.a , lo habia puesto asi porque la idea era que todo lo que usamos (tanto libs como .hs) estuviera contenido en el proyecto, y no hubiera que andar con paths absolutos en /usr o lo que sea, pero si de todos modos no se puede subir todo podemos cambiarlo.
