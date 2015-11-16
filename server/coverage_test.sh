#!/bin/bash

./build.sh
cp cov.sh build/unit_tests/
cd build/unit_tests/
./cov.sh
#coveralls-lcov --repo-token 'yoK0wIjg3zeJ5fTVQdZr1fyG4fXltkZ7a' coverage.info
