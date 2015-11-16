#!/bin/bash
rm -rf Coverage
mkdir Coverage

for entry in 'src'/*; do
	if [[ "$entry" != "src/main.cpp" ]] 
	then
		cp "$entry" "Coverage/" 
	fi

done

for entry in 'unit_tests'/*; do
	if [ -f "$entry" ] 
	then
		if [[ "$entry" != "unit_tests/CMakeLists.txt" ]]
		then
			cp "$entry" "Coverage/" 
		fi
	fi
done

cp "unit_tests/build_test/CMakeLists.txt" "Coverage"
mkdir Coverage/build
cd Coverage/build
cmake ../
make 
cp "../../cov.sh" .
./cov.sh
#coveralls-lcov --repo-token 'yoK0wIjg3zeJ5fTVQdZr1fyG4fXltkZ7a' coverage.info
