#ifndef WAVLTREE_H
#define WAVLTREE_H
//#include <iostream>

// explanations for public member functions are provided in project2.h
// each file that uses a WAVL tree should #include this file 
template <typename KeyType, typename ValType>
class WAVLTree {
	public:
		// DO NOT MODIFY PUBLIC MEMBERS BELOW THIS LINE
		WAVLTree();
		~WAVLTree();
		void insert(KeyType key, ValType val);
		ValType find(const KeyType& key);
		int getSize();
		int getHeight();
		int getRank(const KeyType& key);
		// DO NOT MODIFY PUBLIC MEMBERS ABOVE THIS LINE

		// define new public members
    struct node
    {
      KeyType k;
      ValType v;
      int rank = 0;
      node* left = nullptr;
      node* right = nullptr;
      node* parent = nullptr;
    };
    node* root;

	private:
		// define new private members
    int count = 0;
    void destroy_tree(node* leaf);
    void insert_node(node* leaf, node* newNode);
    void balance_tree(node* leaf);
    void left_rotation(node* leaf);
    void right_rotation(node* leaf);
    void rl_rotation(node* leaf);
    void lr_rotation(node* leaf);
};

// fill in the definitions for each public member function and for any additional public/private members you define

template <typename KeyType, typename ValType>
WAVLTree<KeyType, ValType>::WAVLTree()
{
  root = nullptr;
}


template <typename KeyType, typename ValType>
WAVLTree<KeyType, ValType>::~WAVLTree()
{
   destroy_tree(root);
   count = 0;
}

template <typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::insert(KeyType key, ValType val)
{
  // create the new node to insert
  node* newNode = new node;
  newNode->k = key;
  newNode->v = val;
  newNode->rank = 1;
  this->count++;

  if (!root)
    root = newNode;
  else
    insert_node(root, newNode);
}

template <typename KeyType, typename ValType>
ValType WAVLTree<KeyType, ValType>::find(const KeyType& key)
{
  node* leaf = root;
  while (leaf->k != key)
  {
    if (leaf->k > key)
      leaf = leaf->left;
    else
      leaf = leaf->right;

    if (leaf == nullptr)
      return -999;
  }
	return leaf->v;
}
template <typename KeyType, typename ValType>
int WAVLTree<KeyType, ValType>::getSize()
{
  return count;
}

template <typename KeyType, typename ValType>
int WAVLTree<KeyType, ValType>::getHeight()
{
  return root->rank;
}

template <typename KeyType, typename ValType>
int WAVLTree<KeyType, ValType>::getRank(const KeyType& key)
{
  node* leaf = root;
  while (leaf->k != key)
  {
    if (leaf->k > key)
      leaf = leaf->left;
    else
      leaf = leaf->right;

    if (leaf == nullptr)
      return -999;
  }
  return leaf->rank;
}

// add definitions for any public/private members if needed
template <typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::destroy_tree(node* leaf)
{
  if (leaf)
  {
    destroy_tree(leaf->left);
    destroy_tree(leaf->right);
    delete leaf;
  }
}

template <typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::insert_node(node* leaf, node* newNode)
{
  if (leaf->k < newNode->k)
  {
    if (!leaf->right)
    {
      leaf->right = newNode;
      newNode->parent = leaf;
    }
    else
      insert_node(leaf->right, newNode);
  }
  else
  {
    if (!leaf->left)
    {
      leaf->left = newNode;
      newNode->parent = leaf;
    }
    else
      insert_node(leaf->left, newNode);
  }
  balance_tree(leaf);
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::balance_tree(node* leaf)
{
  // balance tree (if current node has a child node with the same rank)
  //  (1) 0,1 || 1,0 node => simply add 1 to the current node's rank
  //  (2) 0,2 || 2,0 node => do rotation
  node* lnode = leaf->left;
  node* rnode = leaf->right;

  if (rnode && leaf->rank == rnode->rank)
  {
    int l_diff;
    if (!lnode)
      l_diff = leaf->rank;
    else
      l_diff = leaf->rank - lnode->rank;

    if (l_diff > 1)
    {
      int r_r_rank, r_l_rank;
      if (rnode->right)
        r_r_rank = rnode->right->rank;
      else
        r_r_rank = 0;
      if (rnode->left)
        r_l_rank = rnode->left->rank;
      else
        r_l_rank = 0;

      // [1] single rotation (2,1-node)
      if (rnode->rank - r_l_rank > 1)
      {
        //std::cout << "LEFT" << std::endl;
        left_rotation(leaf);
      }
        
      // [2] double rotation (1,2-node)
      else {
        //std::cout << "RL" << std::endl;
        rl_rotation(leaf);
      }
        
    }
    else
    {
      //std::cout << "ADD RANK" << std::endl;
      leaf->rank += 1;
    }
      
  }

  if (lnode && leaf->rank == lnode->rank)
  {
    int r_diff;
    if (!rnode)
      r_diff = leaf->rank;
    else
      r_diff = leaf->rank - rnode->rank;

    // if the current node has a child node with rank 2 difference
    if (r_diff > 1)
    {
      int l_r_rank, l_l_rank;
      if (lnode->right)
        l_r_rank = lnode->right->rank;
      else
        l_r_rank = 0;
      if (lnode->left)
        l_l_rank = lnode->left->rank;
      else
        l_l_rank = 0;

      // [1] single rotation (1,2-node)
      if (lnode->rank - l_r_rank > 1)
      {
        //std::cout << "RIGHT" << std::endl;
        right_rotation(leaf);
      }
        
      // [2] double rotation (2,1-node)
      else
      {
        //std::cout << "LR" << std::endl;
        lr_rotation(leaf);
      }
        
    }
    else
    {
      //std::cout << "ADD RANK" << std::endl;
      leaf->rank += 1;
    }
  }
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::left_rotation(node* leaf)
{
  //std::cout << leaf->k <<"\t" << leaf->right->k << std::endl;
  node* rnode = leaf->right;
  if (leaf->parent)
  {
    //std::cout << leaf->k <<"'s parent: " << leaf->parent->k << std::endl;
    if (leaf->parent->left == leaf)
      leaf->parent->left = rnode;
    else
      leaf->parent->right = rnode;
    rnode->parent = leaf->parent;
  }

  leaf->right = rnode->left;
  if (rnode->left) rnode->left->parent = leaf;
  leaf->rank -= 1;
  leaf->parent = rnode;
  rnode->left = leaf;
  if (leaf == root)
  {
    root = rnode;
    root->parent = nullptr;
  }
    
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::right_rotation(node* leaf)
{
  node* lnode = leaf->left;
  if (leaf->parent)
  {
    //std::cout << leaf->k << "'s parent: " << leaf->parent->k << std::endl;
    if (leaf->parent->right == leaf)
      leaf->parent->right = lnode;
    else
      leaf->parent->left = lnode;
    lnode->parent = leaf->parent;
  }

  leaf->left = lnode->right;
  if (lnode->right) lnode->right->parent = leaf;
  leaf->rank -= 1;
  leaf->parent = lnode;
  lnode->right = leaf;
  if (leaf == root)
  {
    root = lnode;
    root->parent = nullptr;
  }
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::rl_rotation(node* leaf)
{
  node* cnode = leaf->right;
  node* bnode = cnode->left;
  cnode->rank -= 1;
  bnode->rank += 1;
  cnode->left = bnode->right;
  if (bnode->right) bnode->right->parent = cnode;
  bnode->right = cnode;
  cnode->parent = bnode;
  bnode->parent = leaf;
  leaf->right = bnode;
  left_rotation(leaf);
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::lr_rotation(node* leaf)
{
  node* cnode = leaf->left;
  node* bnode = cnode->right;
  cnode->rank -= 1;
  bnode->rank += 1;
  cnode->right = bnode->left;
  if (bnode->left) bnode->left->parent = cnode;
  bnode->left = cnode;
  cnode->parent = bnode;
  bnode->parent = leaf;
  leaf->left = bnode;
  right_rotation(leaf);
}


#endif /* WAVLTREE_H */
