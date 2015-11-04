all: compile-debug

cv-debug: cv.cpp
	g++ -std=c++11 cv.cpp -o $@ -pthread -O0 -g3 -Wall -Wextra -Werror -pedantic

cv-fast: cv.cpp
	g++ -std=c++11 cv.cpp -o $@ -pthread -O3 -finline-functions -DNDEBUG

.PHONY: compile-debug run-debug compile-fast run-fast analyze

compile-debug: cv-debug

run-debug: cv-debug
	./cv-debug --cpus 1

compile-fast: cv-fast

run-fast: cv-fast
	./cv-fast

analyze: cv-fast
# Generated using ./analyze.sh
	@./cv-fast --init-pattern xorshift128plus --init-seed 1 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 2 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 3 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 4 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 5 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 6 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 7 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 8 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 9 --file-out /dev/null --cpus 1 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 10 --file-out /dev/null --cpus 1 --format tdl
	#
	@./cv-fast --init-pattern xorshift128plus --init-seed 1 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 2 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 3 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 4 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 5 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 6 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 7 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 8 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 9 --file-out /dev/null --cpus 2 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 10 --file-out /dev/null --cpus 2 --format tdl
	#
	@./cv-fast --init-pattern xorshift128plus --init-seed 1 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 2 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 3 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 4 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 5 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 6 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 7 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 8 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 9 --file-out /dev/null --cpus 3 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 10 --file-out /dev/null --cpus 3 --format tdl
	#
	@./cv-fast --init-pattern xorshift128plus --init-seed 1 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 2 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 3 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 4 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 5 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 6 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 7 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 8 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 9 --file-out /dev/null --cpus 4 --format tdl
	@./cv-fast --init-pattern xorshift128plus --init-seed 10 --file-out /dev/null --cpus 4 --format tdl
