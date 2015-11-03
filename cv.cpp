/* Intended to be compiled as C++11 by gcc */

#include <thread>
#include <vector>

void compute_cv(size_t* which, const size_t* with) {
  abort(); // FIXME
}

void run_chunk(size_t* const begin, size_t* const end, std::vector<size_t> following) {
  while (!following.empty()) {
    for (size_t* ptr = begin; ptr != end; ++ptr) {
      compute_cv(ptr, ptr++);
    }
    compute_cv(end, &following.front());
    for (size_t i = 1; i < following.size(); ++i) {
      compute_cv(&following[i - 1], &following[i]);
    }
    following.erase(--following.end()):
  }
}

int main(/* uhh */) {
  /* uhh */
  return 0;
}

