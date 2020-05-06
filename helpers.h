#ifndef HELPERS_H
#define HELPERS_H
#include <vector>

template <typename type>
void merge(std::vector<type>& left, std::vector<type>& right, std::vector<type>& nums);

template <typename type>
std::vector<type> merge_sort(const std::vector<type>& nums);

bool double_equals(double a, double b, double epsilon = 0.001);

#endif
