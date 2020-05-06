#include <vector>
#include "project2.h"
#include "WAVLTree.h"
#include "helpers.h"
//#include <iostream>

//// FOR DEBUG
//void print_children(WAVLTree<int, std::pair<double, double>>::node* n)
//{
//  if (n->left)
//  {
//    std::cout << n->k << "(" << n->rank << ")'s left: " << n->left->k << "(" << n->left->rank << ")\t" <<
//      "first: " << n->left->v.first << "\tsecond: " << n->left->v.second << std::endl;
//    print_children(n->left);
//  }
//  if (n->right)
//  {
//    std::cout << n->k << "(" << n->rank << ")'s right: " << n->right->k << "(" << n->right->rank << ")\t" <<
//      "first: " << n->right->v.first << "\tsecond: " << n->right->v.second << std::endl;
//    print_children(n->right);
//  }
//}

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

int get_bin_number(WAVLTree<int, std::pair<double, double>>::node* node, const double item)
{
  int bin = -1;
  if (!node)
    return -1;
  if (node->v.second > item || double_equals(node->v.second, item))
  {
    bin = get_bin_number(node->left, item);
    if (bin == -1 && (node->v.first > item || double_equals(node->v.first, item)))
    {
      return node->k;
    }
      
    else if (bin == -1 && node->v.first < item)
      bin = get_bin_number(node->right, item);
    
  }
  // else
  return bin;
}

double getLargestChildRC(WAVLTree<int, std::pair<double, double>>::node* n)
{
  double left = 0, right = 0;
  if(n->left)
    left = n->left->v.second;
  if(n->right)
    right = n->right->v.second;
  if (left > right || double_equals(left, right))
    return left;
  else
    return right;
}

void updateLargestRCs(WAVLTree<int, std::pair<double, double>>::node* n)
{
  while (n->parent)
  {
    // 1) Matching LRCs after rotation
    // After left rotation (left node's lrc needs to be updated)
    if (n->parent->left && double_equals(n->parent->left->v.second, n->parent->v.first))
    {
      double temp = getLargestChildRC(n->parent->left);
      if (n->parent->v.first > temp || double_equals(n->parent->v.first, temp))
        temp = n->parent->v.first;
      n->parent->left->v.second = temp;
    }

    // After right rotation (right node's lrc needs to be updated)
    if (n->parent->right && double_equals(n->parent->right->v.second, n->parent->v.first))
    {
      double temp = getLargestChildRC(n->parent->right);
      if (n->parent->v.first > temp || double_equals(n->parent->v.first, temp))
        temp = n->parent->v.first;
      n->parent->right->v.second = temp;
    }

    // 2) update parent's LRC
    double temp = getLargestChildRC(n->parent);
    if (n->parent->v.first > temp || double_equals(n->parent->v.first, temp))
      temp = n->parent->v.first;
    n->parent->v.second = temp;
    n = n->parent;
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

    if (bin == -1)
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
      if (double_equals(binTree.find(bin).first, largest_rc))
        largest_rc = get_largest_rc(binTree, bin, rc);
      
      binTree.setValue(bin, { rc, largest_rc });
    }
    // update values of all parent nodes
    updateLargestRCs(binTree.find_node(bin));
    assignment[i] = bin;

    //std::cout << binTree.root->k << "(" << binTree.root->rank << ")\t" <<
    //  "first: " << binTree.root->v.first << "\tsecond: " << binTree.root->v.second << std::endl;
    //print_children(binTree.root);
  }

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
