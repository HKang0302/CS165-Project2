#include "helpers.h"
#include <cmath>

bool double_equals(double a, double b, double epsilon)
{
  return std::abs(a - b) < epsilon;
}
