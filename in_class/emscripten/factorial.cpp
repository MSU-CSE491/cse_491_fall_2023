#include <emscripten/emscripten.h>
#include <iostream>

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  int factorial(int n) {
    if (n == 0) return 1;
    return n * factorial(n - 1);
  }
}
