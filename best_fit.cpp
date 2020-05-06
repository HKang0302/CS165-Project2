#include <vector>
#include "project2.h"
#include "WAVLTree.h"

void best_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  WAVLTree<double, int> rcTree;  // key: remaining capacity, value: bin
  int size = items.size();

  for (int i = 0; i < size; i++)
  {
    double item = items[i];
    double rc = -1;
    if (!rcTree.root)
    {
      rc = 1 - item;
      rcTree.insert(rc, 1);
      assignment[0] = 1;
      free_space.push_back(rc);
    }
    else
    {

    }
  }
}
