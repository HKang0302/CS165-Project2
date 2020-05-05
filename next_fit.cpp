#include <iostream>
#include "project2.h"
#include <vector>
void next_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  int size = items.size();
  int current_bin = 0;
  for (int i = 0; i < size; i++)
  {
    double item = items[i];
    if (free_space.size() == 0)
    {
      free_space.push_back(1 - item);
      current_bin = 1;
    }
    else
    {
      if (free_space[current_bin - 1] >= item)
      {
        free_space[current_bin - 1] -= item;
      }
      else
      {
        free_space.push_back(1 - item);
        current_bin += 1;
      }
    }
    assignment[i] = current_bin;
  }
}
