#include <vector>
#include <algorithm>
#include "project2.h"
#include "WAVLTree.h"
#include "helpers.h"
#include <functional>

void printTree(WAVLTree<double, std::vector<int>>::node* node)
{
  if (node)
  {
    std::cout << "Key: " << node->k << ", Val: ";
    for (int x : node->v)
      std::cout << x << " ";
    std::cout << ", Rank: " << node->rank;
    std::cout << std::endl;
  }
  else return;
  printTree(node->right);
  printTree(node->left);
}

double find_key(WAVLTree<double, std::vector<int>>::node* node, double item)
{
  // if there's no node with enough rc, return -1
  // otherwise return rc of the node

  // if the key is bigger than or equal to item, traverse to left node
  // else if the key is smaller than item, traverse to right
  // if node is nullptr, return -1
  double result = -1;
  if (!node)
    return result;

  // if key == item and left node's key != item
  if (double_equals(node->k, item) && (!node->left || (node->left && !double_equals(node->left->k, item))))
    return node->k;

  if (node->k > item || double_equals(node->k, item))
  {
    result = find_key(node->left, item);
    if (result == -1)
      return node->k;
  }

  if (node->k < item)
  {
    result = find_key(node->right, item);
  }
  return result;
}

void insertBin(std::vector<int>& bins, int bin)
{
  bins.push_back(bin);
  std::sort(bins.begin(), bins.end(), std::greater<int>());
}

void best_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
  WAVLTree<double, std::vector<int>> rcTree;  // key: remaining capacity, value: vector of bins
  int size = items.size();

  for (int i = 0; i < size; i++)
  {
    double item = items[i];
    double rc = -1;
    int bin = -1;
    std::vector<int> bins;
    //std::cout << "----------<" << item << ">-------------" << std::endl;
    if (rcTree.root)
      rc = find_key(rcTree.root, item);
    if (rc == -1)
    {
      rc = 1 - item;
      bin = rcTree.getSize() + 1;
      insertBin(bins, bin);
      rcTree.insert(rc, bins);
      free_space.push_back(rc);
    }

    else
    {
      // 1) check if the value of the node has more than one bins
      //    -- if so, do not delete the node (delete the lowest bin only)
      //    -- if one bin, delete the node
      bins = rcTree.find(rc);
      bin = bins.back(); // lowest bin
      if (bins.size() > 1)
      {
        bins.pop_back();  // remove the bin from the vector
        rcTree.setValue(rc, bins);
      }
      else
      {
        rcTree.remove(rcTree.root, rc);
      }

      
      //std::cout << "===============AFTER REMOVE============" << std::endl;
      //printTree(rcTree.root);

      rc -= item;
      WAVLTree<double, std::vector<int>>::node* eNode = rcTree.find_node(rc); // existing node

      if (eNode)
      {
        std::vector<int> newBins = eNode->v;
        insertBin(newBins, bin);
        rcTree.setValue(rc, newBins);
        rcTree.count++;
      }
      else
      {
        std::vector<int> newBins;
        insertBin(newBins, bin);
        rcTree.insert(rc, newBins);
      }

      free_space[bin - 1] -= item;
      // 2) get new rc (key of (1) - item)
      // 3) if new rc exists in the tree, add the bin# into the val vector
      //    -- if not, insert a new bin
    }

    //std::cout << "===============AFTER insertion============" << std::endl;
    //printTree(rcTree.root);

    assignment[i] = bin;
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
