#!/bin/bash

upload=$1
lcov --directory . --zerocounters
lcov --no-external --capture --initial --directory . --output-file base.info --quiet
./run_tests.sh
lcov --no-external --directory . --capture --output-file tests.info --quiet
lcov --add-tracefile base.info --add-tracefile tests.info --output-file total.info --quiet
lcov --remove total.info '/unit_tests/*' '/include/*' '/src/main.cpp' '/src/HttpRequestMock.*' --output-file total.info --quiet
#lcov --list total.info
if [ "$upload" = "UPLOAD" ]; then
	coveralls-lcov --repo-token "E1b0z2jufLOPrtenbS8Lcwaj0M7SnxhfX" total.info
else
	#ordeno
	rm -rf coverage
	mkdir coverage
	mkdir coverage/html
	rm base.info
	rm tests.info
	cp total.info coverage/total.info
	rm total.info
	genhtml --ignore-errors source coverage/total.info --output-directory=coverage/html
fi