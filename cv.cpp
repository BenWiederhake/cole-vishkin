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
 *   --file cv_out.dat
 *
 * The length is chosen so that it uses 2 GiB on 64-bit machines
 * and 1 GiB on 32-bit machines.
 * For all options and their explanations, run it with --help.
 */

#include <cassert>
#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <thread>
#include <vector>

/* I know, cstdio isn't really C++11-ish. However, it feels more appropriate. */


/* ===== Pseudo-header ===== */

/* If you ever want to use this as part of your
 * why-the-hell-would-you-need-to-emulate-cole-vishkin-implementation,
 * feel free to copy the following into a header file and access the
 * functionality through it: */

extern const std::string cv_about;
typedef void (*cv_fill_fn_t)(size_t*,size_t,size_t);
extern cv_fill_fn_t cv_default_fill;
class cv_opts {
public:
    size_t cpus = 4;
    std::string file_out_name = "cv_out.dat";
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
    std::minstd_rand generator(seed);
    std::uniform_int_distribution<size_t> distrib;

    begin[0] = distrib(generator);
    for (size_t i = 1; i < length; ++i) {
        begin[i] = distrib(generator);
        if (begin[i] == begin[i - 1]) {
            --i;
        }
    }
    size_t* last = begin + (length - 1);
    while (*last == *begin) {
        *last = distrib(generator);
    }
}

/*
void fill_rnd_whatever(size_t* begin, size_t length, size_t seed) {
    // FIXME
    // If you add an algorithm, update try_parse!
    abort();
}
*/

cv_fill_fn_t cv_default_fill = fill_rnd_minstd;

/* ===== Commandline parsing ===== */

const std::string cv_about = ""
"CV, a Cole-Vishkin emulator in C++11.\n"
#ifndef NDEBUG
"Compiled without NDEBUG (so this is the slow version).\n"
#else
"Compiled with NDEBUG (so this is the fast version).\n"
#endif
"Default arguments: --cpus 4 --file-out cv_out.dat \\\n"
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
"--help:\n"
"    Prints this help text and quits.\n"
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
"Go forth and haveth fun!"; // No trailing newline!

static const char* advance(int& i, const int argc) {
    ++i;
    if (i < argc) {
        return nullptr;
    }
    return "Needs an argument.";
}

static const char* try_stos(const char* str, size_t& into) {
    try {
        into = std::stoll(str);
        return nullptr;
    } catch (std::invalid_argument) {
        return "Need a numeric argument.";
    } catch (std::out_of_range) {
        return "Expected numeric argument.";
    }
}

/* Returns a human-readable string on error, nullptr otherwise. */
const char* cv_try_parse(cv_opts& into, const int argc, char** const argv) {
    /* Ignore own name, so start at 1: */
    for (int i = 1; i < argc; ++i) {
        const char* err = nullptr;
        if (std::string("--cpus") == argv[i]) {
            if ((err = advance(i, argc))) {
                return err;
            }
            if ((err = try_stos(argv[i], into.cpus))) {
                return err;
            }
            ++i;
        } else if (std::string("--file-out") == argv[i]) {
            if ((err = advance(i, argc))) {
                return err;
            }
            into.file_out_name = argv[i];
            ++i;
        } else if (std::string("--help") == argv[i]) {
            printf("%s\n", cv_about.c_str());
            return "";
        } else if (std::string("--init-pattern") == argv[i]) {
            if ((err = advance(i, argc))) {
                return err;
            }
            if (std::string("minstd") == argv[i]) {
                into.init_pattern_fn = fill_rnd_minstd;
/*
            } else if (std::string("whatever") == argv[i]) {
                    into.init_pattern_fn = fill_rnd_whatever;
*/
            } else {
                return "Only 'minstd' is supported as --init-pattern, sorry.";
            }
        } else if (std::string("--init-seed") == argv[i]) {
            if ((err = advance(i, argc))) {
                return err;
            }
            if ((err = try_stos(argv[i], into.init_seed))) {
                return err;
            }
            ++i;
        } else if (std::string("--length") == argv[i]) {
            if ((err = advance(i, argc))) {
                return err;
            }
            if ((err = try_stos(argv[i], into.length))) {
                return err;
            }
            ++i;
        } else if (std::string("--rounds") == argv[i]) {
            if ((err = advance(i, argc))) {
                return err;
            }
            if ((err = try_stos(argv[i], into.rounds))) {
                return err;
            }
            ++i;
        } else {
            printf("At option %s\n", argv[i]);
            return "Unrecognized option";
        }
    }

    if (into.cpus < 1 || into.cpus > 256) {
        return "Invalid amount of cpus.";
    }
    if (into.length < into.cpus) {
        return "Must use at least #cpus many nodes in the list.";
    }
    if (into.length > (1ULL << 28)) {
        printf("Warning: More that 1<<28 nodes. This means you'll need >1GiB on"
                " 32-bit,\n and >2GiB on 64-bit platforms.");
    }
    if (into.length > (1ULL << 31)) {
        return "Error: More that 1<<31 nodes. This means you'll need >8GiB on"
                " 32-bit,\n and >16GiB on 64-bit platforms.";
    }
    if (into.rounds < 1) {
        return "Number of rounds must be positive.";
    }
    if (into.rounds < 4) {
        printf("Warning: with this few rounds, you may not end up with <= 6 colors.");
    }

    return nullptr;
}


/* ===== Control worker threads ===== */

void cv_start_and_join_workers(size_t* const begin, const size_t length,
                               const size_t cpus, const size_t rounds) {
    std::vector<size_t> border;
    border.emplace_back(0);
    for (size_t i = 1; i < 1 + cpus; ++i) {
        border.emplace_back((length * i) / cpus);
        if (border[i] < border[i - 1]) {
            assert("Dangit, jumped down!" || false);
            abort();
        }
    }

    std::vector<std::vector<size_t>> buf;
    for (size_t i = 0; i < cpus; ++i) {
        buf.emplace_back();
        if (border[i + 1] + rounds <= length) {
            buf.back().insert(buf.back().end(),
                    begin + border[i + 1], begin + border[i + 1]);
        } else {
            /* Should compute offsets and batch-insert.
             * But 'rounds' is small enough, so it doesn't matter. */
            for (size_t j = 0, pos = border[i + 1]; j < rounds; ++j) {
                if (pos >= length) {
                    pos -= length;
                }
                buf.back().push_back(begin[pos]);
            }
        }
    }

    /* This already starts the workers, not only allocates them! */
    std::vector<std::thread> threads;
    for (size_t i = 0; i < cpus; ++i) {
        /*run_chunk(size_t* const begin, size_t* const end,
                              std::vector<size_t> following)*/
        threads.emplace_back(run_chunk, begin + border[i],
                begin + border[i + 1], std::move(buf[i]));
    }

    for (std::thread& t : threads) {
        t.join();
    }
}


/* ===== Write to file ===== */

const char* cv_write_file(const size_t* const begin, const size_t length,
                const std::string& file_out_name) {
    FILE* fp = fopen64(file_out_name.c_str(), "wb");
    if (!fp) {
        return "fopen failed. (Bad filename? Write permissions?)";
    }

    if (1 != fwrite(begin, length * sizeof(size_t), 1, fp)) {
        return "fwrite failed. (Write permissions? enough space?)";
    }

    fclose(fp);
    return nullptr;
}


/* ===== Holistic ===== */

typedef std::chrono::high_resolution_clock my_clock_t;

// explain(clock_finish - clock_init, "<All>");
static void explain(const my_clock_t::duration dur, const std::string& reason) {
    printf("%s took %ld ms.\n", reason.c_str(),
           std::chrono::duration_cast<std::chrono::milliseconds>(dur).count());
}

int cv_main(int argc, char **argv, bool print_errors) {
    const my_clock_t::time_point clock_init = my_clock_t::now();

    cv_opts opts;
    const char* err = cv_try_parse(opts, argc, argv);
    if (err) {
        if (print_errors) {
            printf("%s\n", err);
        }
        return 1;
    }

    /* Use malloc since I don't want to use try/catch. */
    size_t* arr = static_cast<size_t*>(malloc(opts.length * sizeof(size_t)));
    if (!arr) {
        if (print_errors) {
            printf("malloc failed!\n");
        }
        return 2;
    }

    opts.init_pattern_fn(arr, opts.length, opts.init_seed);
    const my_clock_t::time_point clock_ready = my_clock_t::now();

    cv_start_and_join_workers(arr, opts.length, opts.cpus, opts.rounds);
    const my_clock_t::time_point clock_done = my_clock_t::now();

    err = cv_write_file(arr, opts.length, opts.file_out_name);
    free(arr);
    if (err) {
        if (print_errors) {
            printf("%s\n", err);
        }
        return 3;
    }
    const my_clock_t::time_point clock_finish = my_clock_t::now();

    /* Output statistics: */
    explain(clock_ready - clock_init, "Initialization");
    explain(clock_done - clock_ready, "Cole-Vishkin");
    explain(clock_finish - clock_done, "Cleanup");
    explain(clock_finish - clock_init, "<All>");

    return 0;
}

#ifndef CV_NO_MAIN
int main(int argc, char **argv) {
    return cv_main(argc, argv);
}
#endif
