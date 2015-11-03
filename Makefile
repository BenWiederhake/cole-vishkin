all: compile-debug

cv-debug: cv.cpp
	gcc -std=c++11 cv.cpp -o $@ -O0 -g3 -Wall -Wextra -Werror -pedantic

cv-fast: cv.cpp
	gcc -std=c++11 cv.cpp -o $@ -O3 -finline-functions -DNDEBUG

.PHONY: compile-debug run-debug compile-fast run-fast

compile-debug: cv-debug

run-debug: cv-debug
	./cv-debug

compile-fast: cv-fast

run-fast: cv-fast
	./cv-fast
