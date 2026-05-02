#include "circled_operations.hpp"

void course::circle_increment(size_t &value, size_t lower_bound, size_t upper_bound)
{
  if (value >= upper_bound)
  {
    value = lower_bound;
  }
  else
  {
    value++;
  }
}

void course::circle_decrement(size_t &value, size_t lower_bound, size_t upper_bound)
{
  if (value <= lower_bound)
  {
    value = upper_bound;
  }
  else
  {
    value--;
  }
}
