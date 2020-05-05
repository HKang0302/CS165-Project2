#include <vector>
#include "project2.h"
#include "WAVLTree.h"

void first_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  WAVLTree<int, double> binTree;
  int size = items.size();
  int total = 0;
  for (int i = 0; i < size; i++)
  {
    double remaining_capacity = 1;
    double item = items[i];
    int bin_num = 0;
    if (binTree.root == nullptr)
      bin_num = 1;
    else
    {

    }

    binTree.insert(bin_num, item);
    assignment[i] = bin_num;
    free_space.push_back(1 - item);
    total++;
  }
}
