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

CV_ANALYZE_OPTS=--init-pattern xorshift128plus --file-out /dev/null --format tdl --length 536870912 --length-force

analyze: cv-fast
# Generated using ./analyze.sh
	@./cv-fast --init-seed 1 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 2 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 3 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 4 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 5 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 6 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 7 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 8 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 9 --cpus 1 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 10 --cpus 1 ${CV_ANALYZE_OPTS}
	#
	@./cv-fast --init-seed 1 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 2 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 3 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 4 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 5 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 6 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 7 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 8 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 9 --cpus 2 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 10 --cpus 2 ${CV_ANALYZE_OPTS}
	#
	@./cv-fast --init-seed 1 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 2 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 3 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 4 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 5 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 6 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 7 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 8 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 9 --cpus 3 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 10 --cpus 3 ${CV_ANALYZE_OPTS}
	#
	@./cv-fast --init-seed 1 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 2 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 3 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 4 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 5 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 6 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 7 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 8 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 9 --cpus 4 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 10 --cpus 4 ${CV_ANALYZE_OPTS}
	#
	@./cv-fast --init-seed 1 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 2 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 3 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 4 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 5 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 6 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 7 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 8 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 9 --cpus 5 ${CV_ANALYZE_OPTS}
	@./cv-fast --init-seed 10 --cpus 5 ${CV_ANALYZE_OPTS}
