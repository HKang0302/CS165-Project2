#include <vector>
#include <algorithm>
#include "project2.h"
#include "WAVLTree.h"
#include <iostream>

double get_largest_rc(WAVLTree<int, std::pair<double, double>>& binTree, const int bin, const double rc)
{
  double largest_rc=0;
  double num1 = 0, num2 = 0;
  if (binTree.find_node(bin)->left)
  {
    num1 = binTree.find_node(bin)->left->v.second;
  }

  if (binTree.find_node(bin)->right)
  {
    num2 = binTree.find_node(bin)->right->v.second;
  }

  if (num1 > num2)
    largest_rc = num1;
  else
    largest_rc = num2;
  if (largest_rc < rc)
    largest_rc = rc;
  return largest_rc;
}

int get_bin_number(WAVLTree<int, std::pair<double, double>>::node* node, double item)
{
  int bin = -1;
  if (!node)
    return -1;

  if (node->v.second >= item)
  {
    bin = get_bin_number(node->left, item);
    if (bin == -1 && node->v.first >= item)
      return node->k;
    else if (bin == -1 && node->v.first < item)
      bin = get_bin_number(node->right, item);
    
  }
  return bin;
}

void updateLargestRCs(WAVLTree<int, std::pair<double, double>>::node* n)
{
  while (n->parent)
  {
    if (n->parent->v.second > n->v.second)
    {
      if (n->parent->v.second == n->parent->left->v.second
        || n->parent->v.second == n->parent->right->v.second)
        return;
      else
      {
        if (n->parent->v.first > n->v.second)
          n->parent->v.second = n->parent->v.first;
        else
          n->parent->v.second = n->v.second;
        n = n->parent;
      }
    }
    else if (n->parent->v.second < n->v.second)
    {
      n->parent->v.second = n->v.second;
      n = n->parent;
    }
    else
      return;
  }
}

void first_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  WAVLTree<int, std::pair<double, double>> binTree; // value = <remaining capacity, largest rc in the subtree rooted at the node>
  int size = items.size();

  for (int i = 0; i < size; i++)
  {
    double item = items[i];
    double rc;
    int bin=-1;
    if (binTree.root)
      bin = get_bin_number(binTree.root, item);

    double largest_rc = binTree.find(bin).second;

    if (bin==-1)
    {
      bin = binTree.getSize() + 1;
      rc = 1 - item;
      free_space.push_back(rc);
      binTree.insert(bin, { rc, rc });
    }
      
    else
    {
      free_space[bin-1] -= item;
      rc = free_space[bin-1];

      // if rc and lrc are same,
      // compare node->left and right, and get the largest_rc
      // then update both rc and largest_rc of the bin
      if (binTree.find(bin).first == largest_rc)
        largest_rc = get_largest_rc(binTree, bin, rc);
      
      binTree.setValue(bin, { rc, largest_rc });
    }
    // update values of all parent nodes
    updateLargestRCs(binTree.find_node(bin));
    assignment[i] = bin;
  }
}
