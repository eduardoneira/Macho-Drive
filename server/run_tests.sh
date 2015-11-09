#!/bin/bash

for entry in build/unit_tests/*; do
  if [ -x "$entry" ]; then #si tiene permiso de ejecucion (archivos de texto tmb tienen, queria hacer que solo agarre ejecutables pero no se como. igual, en esta carpeta solo van a haber ejecutables asi que hasta se podria borrar el if
    ./$entry
  fi
done
