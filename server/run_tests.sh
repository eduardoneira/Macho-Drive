#!/bin/bash

cd build/unit_tests
echo "///////////////////////////////////////////////////"
for entry in *; do
  if [ -x "$entry" ] && [ $(file "$entry" | grep -wc "executable") = 1 ]; then
	./"$entry"
	echo "///////////////////////////////////////////////////"
  fi
done
