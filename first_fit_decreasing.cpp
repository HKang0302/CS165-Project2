#include <vector>
#include "project2.h"
#include "WAVLTree.h"
#include "helpers.h"
#include <iostream>

void merge1(std::vector<double>& left, std::vector<double>& right, std::vector<double>& nums)
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

void merge_sort1(std::vector<double>& nums)
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

    merge_sort1(left);
    merge_sort1(right);
    merge1(left, right, nums);
  }
}
void first_fit_decreasing(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  std::vector<double> sorted_items = items;
  merge_sort1(sorted_items);
  first_fit(sorted_items, assignment, free_space);
}
