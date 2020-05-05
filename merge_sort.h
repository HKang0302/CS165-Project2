#ifndef MERGE_SORT_H
#define MERGE_SORT_H
#include <vector>

void merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& nums)
{
  int lsize = left.size();
  int rsize = right.size();
  int l = 0, r = 0;
  nums.clear();

  while (l < lsize && r < rsize)
  {
    if (left[l] <= right[r])
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

void merge_sort(std::vector<int>& nums)
{
  int size = nums.size();
  std::vector<int> left, right, result;

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

    merge_sort(left);
    merge_sort(right);
    merge(left, right, nums);

  }
}
