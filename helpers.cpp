#include "helpers.h"
#include <cmath>

template <typename type>
void merge(std::vector<type>& left, std::vector<type>& right, std::vector<type>& nums)
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

template <typename type>
std::vector<type> merge_sort(const std::vector<type>& nums)
{
  std::vector<type> temp = nums;
  int size = nums.size();
  std::vector<type> left, right, result;

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

bool double_equals(double a, double b, double epsilon)
{
  return std::abs(a - b) < epsilon;
}
