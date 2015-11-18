#!/bin/bash

cd unit_tests
for arch in $(ls "../src"); do
	nom_test=$(echo $arch | sed "s/^.*\/\([^\/]*\)$/\1/")
	nom_test=$(echo $nom_test | sed "s/\(.*\)\.cpp~*/\1/")
	nom_test=$(echo $nom_test | sed "s/\(.*\)\.h~*/\1/")

	if [ ! -f "$nom_test" ]; then
		echo "" >> "$nom_test"_unittest.cpp
	fi
done