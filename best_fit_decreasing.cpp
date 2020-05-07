#include <vector>
#include "project2.h"
#include "WAVLTree.h"

void merge2(std::vector<double>& left, std::vector<double>& right, std::vector<double>& nums)
{
  int lsize = left.size();
  int rsize = right.size();
  int l = 0, r = 0;
  nums.clear();

  while (l < lsize && r < rsize)
  {
    if (left[l] >= right[r])
    {
      nums.push_back(left[l]);
      l++;
    }

    else
    {
      nums.push_back(right[r]);
      r++;
    }
  }

  while (l < lsize)
  {
    nums.push_back(left[l]);
    l++;
  }

  while (r < rsize)
  {
    nums.push_back(right[r]);
    r++;
  }
}

void merge_sort2(std::vector<double>& nums)
{
  std::vector<double> temp = nums;
  int size = nums.size();
  std::vector<double> left, right, result;

  if (size > 1)
  {
    int lsize = size / 2;

    for (int i = 0; i < lsize; i++)
    {
      left.push_back(nums[i]);
    }
    for (int i = lsize; i < size; i++)
    {
      right.push_back(nums[i]);
    }

    merge_sort2(left);
    merge_sort2(right);
    merge2(left, right, nums);
  }
}

void best_fit_decreasing(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  std::vector<double> sorted_items = items;
  merge_sort2(sorted_items);
  best_fit(sorted_items, assignment, free_space);
  //// For Debug
  //std::cout << "================================================" << std::endl;
  //std::cout << "bin#: ";
  //for (int x : assignment)
  //{
  //  std::cout << x << " ";
  //}
  //std::cout << std::endl;
  //std::cout << "free: ";
  //for (double x : free_space)
  //{
  //  std::cout << x << " ";
  //}
  //std::cout << std::endl;
  //std::cout << "================================================" << std::endl;
}
