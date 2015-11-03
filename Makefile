all: compile-debug

cv-debug: cv.cpp
	g++ -std=c++11 cv.cpp -o $@ -pthread -O0 -g3 -Wall -Wextra -Werror -pedantic

cv-fast: cv.cpp
	g++ -std=c++11 cv.cpp -o $@ -pthread -O3 -finline-functions -DNDEBUG

.PHONY: compile-debug run-debug compile-fast run-fast

compile-debug: cv-debug

run-debug: cv-debug
	./cv-debug

compile-fast: cv-fast

run-fast: cv-fast
	./cv-fast
