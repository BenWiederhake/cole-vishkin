/* Copyright (c) 2015 Ben Wiederhake, https://github.com/BenWiederhake/cv/
 * CC0 1.0 Universal -- So this is essentially Public Domain.
 * Please see LICENSE or http://creativecommons.org/publicdomain/zero/1.0/
 *
 * Compile (minimal command):
 *   gcc -std=c++11 cv.cpp -o cv
 *
 * Compile (in-depth error checking):
 *   gcc -std=c++11 cv.cpp -o cv -O0 -g3 -Wall -Wextra -Werror -pedantic
 *
 * Compile (optimized executable):
 *   gcc -std=c++11 cv.cpp -o cv -O3 -finline-functions -DNDEBUG
 *
 * Execute:
 *   cv
 *
 * Execute (defaults):
 *   cv --cpus 4 --length 268435456 --init-pattern minstd --init-seed 0 \
 *   --file cv_out_<pattern>_<seed>.dat
 *
 * The length is chosen so that it uses 2 GiB on 64-bit machines
 * and 1 GiB on 32-bit machines.
 * For all options and their explanations, run it with --help.
 */

#include <cassert>
#include <cstdio>
#include <string>
#include <thread>
#include <vector>

/* I know, cstdio isn't really C++11-ish. However, it feels more appropriate. */


/* ===== Pseudo-header ===== */

/* If you ever want to use this as part of your
 * why-the-hell-would-you-need-to-emulate-cole-vishkin-implementation,
 * feel free to copy the following into a header file and access the
 * functionality through it: */

extern const std::string CV_about;
typedef void (*cv_fill_fn_t)(size_t*,size_t,size_t);
extern cv_fill_fn_t cv_default_fill;
class cv_opts {
public:
    size_t cpus = 4;
    std::string file_out_name = "";
    cv_fill_fn_t init_pattern_fn = cv_default_fill;
    size_t init_seed = 0;
    size_t length = 1024; // FIXME: Change to 268435456. I'm using something small for TESTING ONLY.
    size_t rounds = 4;
};
const char* cv_try_parse(cv_opts& into, const int argc, char** const argv);
void cv_start_and_join_workers(size_t* const begin, const size_t length,
                             const size_t cpus, const size_t rounds);
const char* cv_write_file(const size_t* const begin, const size_t length,
                          const std::string& file_out_name);
int cv_main(int argc, char **argv, bool print_errors = true);
/* Note: you can disable the 'main' symbol by compiling with -DCV_NO_MAIN. */


/* ===== Core algorithm ===== */

static inline void compute_cv(size_t* const which, const size_t* const with) {
    /* This feels like it's just two instructions!
     * I'm sure there's an even more efficient implementation
     * using SIMD or something. */
    size_t xored = *which ^ *with;
    assert(xored);
    size_t num = __builtin_ctz(xored);
    size_t orig_bit = 1 & (*which >> num);
    *which = orig_bit | (num << 1);
}

static void run_chunk(size_t* const begin, size_t* const end,
                      std::vector<size_t> following) {
    while (!following.empty()) {
        for (size_t* ptr = begin; ptr != end; ++ptr) {
            compute_cv(ptr, ptr + 1);
        }
        compute_cv(end, &following.front());
        for (size_t i = 1; i < following.size(); ++i) {
            compute_cv(&following[i - 1], &following[i]);
        }
        following.erase(--following.end());
    }
}


/* ===== Filling algorithm(s) ===== */

static void fill_rnd_minstd(size_t* const begin, const size_t length,
                            const size_t seed) {
    // FIXME
    abort();
}

/*
void fill_rnd_whatever(size_t* begin, size_t length, size_t seed) {
    // FIXME
    // TODO: If you add an algorithm, update try_parse!
    abort();
}
*/

cv_fill_fn_t cv_default_fill = fill_rnd_minstd;

/* ===== Commandline parsing ===== */

const std::string CV_about = ""
"CV, a Cole-Vishkin emulator in C++11.\n"
#ifndef NDEBUG
"Compiled without NDEBUG (so this is the slow version).\n"
#else
"Compiled with NDEBUG (so this is the fast version).\n"
#endif
"Default arguments: --cpus 4 --file-out cv_out_<pattern>_<seed>.dat \\\n"
"    --init-pattern minstd --init-seed 0 --length 268435456 --rounds 4\n"
"\n"
"Explanation of each argument:\n"
"--cpus <n>:\n"
"    How many worker threads should run on the data. This shouldn't exceed\n"
"    your physical resources too far.\n"
"--file-out <filename>:\n"
"    The file to which the result should be written. A bit pointless,\n"
"    since no-one reads it anyways. But without this, Cole-Vishkin would be\n"
"    utterly pointless.\n"
"--init-pattern <type>:\n"
"    Name of the initial pattern. Currently, only one type is supported,\n"
"    namely 'minstd', which uses std::minstd_rand to generate the colors.\n"
"    Note that, no matter the pattern used, duplicates will be skipped.\n"
"    Note that future versions may introduce a --init-max argument.\n"
"--init-seed <n>:\n"
"    The seed for the pattern (presumably a PRNG). This argument exists in\n"
"    order to provide reproducibility.\n"
"--length <n>:\n"
"    Length of the simulated list. Note that cv will need approximately this\n"
"    many words of memory. For the default amount (roughly 200 million), this\n"
"    equals 2 GiB on a 64-bit machine, or 1 GiB on a 32-bit one.\n"
"--rounds <n>:\n"
"    The number of rounds for which Cole-Vishkin should be executed.\n"
"    Here's a table about how long the initial color may be for each value:\n"
"    1: 3 bits or less\n"
"    2: 4 bits or less\n"
"    3: 8 bits or less\n"
"    4: 128 bits or less\n"
"    5: 2^64 bits or less\n"
"    6: YAGNI\n"
"\n"
"Go forth and haveth fun!\n";

/* Returns a human-readable string on error, nullptr otherwise. */
const char* cv_try_parse(cv_opts& into, const int argc, char** const argv) {
    // FIXME
    // TODO: Also do sanity checking (e.g. no more than 20 rounds,
    //       short filename, 1 <= cpus <= 256, etc.)
    abort();
}


/* ===== Control worker threads ===== */

void cv_start_and_join_workers(size_t* const begin, const size_t length,
                             const size_t cpus, const size_t rounds) {
    // FIXME
    abort();
}


/* ===== Write to file ===== */

const char* cv_write_file(const size_t* const begin, const size_t length,
                const std::string& file_out_name) {
    // FIXME
    abort();
}


/* ===== Holistic ===== */

int cv_main(int argc, char **argv, bool print_errors) {
    cv_opts opts;
    const char* err = cv_try_parse(opts, argc, argv);
    if (err) {
        if (print_errors) {
            printf(err);
        }
        return 1;
    }

    /* Use malloc since I don't want to use try/catch. */
    size_t* arr = static_cast<size_t*>(malloc(opts.length * sizeof(size_t)));
    if (!arr) {
        if (print_errors) {
            printf("malloc failed!");
        }
        return 2;
    }

    opts.init_pattern_fn(arr, opts.length, opts.init_seed);

    cv_start_and_join_workers(arr, opts.length, opts.cpus, opts.rounds);

    err = cv_write_file(arr, opts.length, opts.file_out_name);
    free(arr);
    if (err) {
        if (print_errors) {
            printf(err);
        }
        return 3;
    }
    return 0;
}

#ifndef CV_NO_MAIN
int main(int argc, char **argv) {
    return cv_main(argc, argv);
}
#endif
