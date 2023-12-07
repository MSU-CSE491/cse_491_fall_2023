#include <emscripten/emscripten.h>
#include <iostream>
#include <cmath>

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  double power(double base, double exponent) {
    return std::pow(base, exponent);
  }
}
