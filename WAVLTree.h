#ifndef WAVLTREE_H
#define WAVLTREE_H
#include <iostream>

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

    typedef node* Node;
    auto find_node(const KeyType& key)-> Node;
    void setValue(KeyType key, const ValType val);
    auto remove(WAVLTree<KeyType, ValType>::node* leaf, KeyType key)->Node;

	private:
		// define new private members
    int count = 0;
    void destroy_tree(node* leaf);
    void insert_node(node* leaf, node* newNode);
    auto findSmallestNode(node* leaf)->Node;
    void balance_insert(node* leaf);
    void balance_remove(node* leaf);
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
  node* leaf = find_node(key);
  if (leaf)
    return leaf->v;
  else
    return ValType();
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
  node* leaf = find_node(key);
  if (leaf)
    return leaf->rank;
  else
    return -999;
}

template<typename KeyType, typename ValType>
auto WAVLTree<KeyType, ValType>::find_node(const KeyType& key) -> Node
{
  node* leaf = root;
  
  while (leaf && leaf->k != key)
  {
    if (leaf->k > key)
      leaf = leaf->left;
    else
      leaf = leaf->right;
  }
  return leaf;
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::setValue(KeyType key, const ValType val)
{
  node* n = find_node(key);
  n->v = val;
}

template<typename KeyType, typename ValType>
auto WAVLTree<KeyType, ValType>::remove(WAVLTree<KeyType, ValType>::node* leaf, KeyType key) -> Node
{
  node* parent;
  if (leaf->k > key)
    parent = remove(leaf->left, key);
  else if (leaf->k < key)
    parent = remove(leaf->right, key);
  else
  {
    parent = leaf->parent;
    node* lnode = leaf->left;
    node* rnode = leaf->right;
    // if the dNode has only one or no child
    if (!lnode || !rnode)
    {
      node* child = lnode ? lnode : rnode;
      // if there's a child
      if (child)
      {
        // if dNode is not root
        if (parent)
        {
          child->parent = parent;
          if (parent->left == leaf)
            parent->left = child;
          else
            parent->right = child;
        }

        // if dNode is root
        else
          root = child;
      }
      else if(parent)
      {
        if (parent->left == leaf)
          parent->left = nullptr;
        else
          parent->right = nullptr;
      }
      delete leaf;
    }

    // if dNode has two children
    else
    {
      // 1) get the node with the smallest key in the right subtree
      node* nNode;
      if (leaf->left->rank == 1)
      {
        nNode = leaf->left;
        nNode->rank = leaf->rank;
        nNode->right = leaf->right;
        nNode->parent = leaf->parent;
        if(leaf->parent)
          leaf->parent->right = nNode;
        leaf->right->parent = nNode;
        parent = nNode;
      }
        
      else
      {
        node* temp;
        temp = findSmallestNode(rnode);
        parent = temp->parent; // node for balancing

        // 2) create a new node including temp info
        //    place it to the leaf spot and delete leaf
        nNode = new node;
        nNode->k = temp->k;
        nNode->v = temp->v;
        nNode->rank = leaf->rank;
        nNode->right = leaf->right;
        nNode->parent = leaf->parent;
        nNode->left = leaf->left;
        leaf->right->parent = nNode;
        leaf->left->parent = nNode;
        parent->left = nullptr;
        delete temp;
      }

      
      if (leaf == root)
      {
        root = nNode;
        while (parent)
        {
          balance_remove(parent);
          parent = parent->parent;
        }
      }
      delete leaf;     
    }
    this->count -= 1;
  }
  
  // balance tree after remove
  if (parent)
  {
    balance_remove(parent);
    return parent->parent;
  }

  if (this->count == 0)
    root = nullptr;

  return root;
  
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
  balance_insert(leaf);
}

template<typename KeyType, typename ValType>
auto WAVLTree<KeyType, ValType>::findSmallestNode(node* leaf) -> Node
{
  while (leaf->left)
    leaf = leaf->left;    
  
  return leaf;
}

template<typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::balance_insert(node* leaf)
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
void WAVLTree<KeyType, ValType>::balance_remove(node* leaf)
{
  node* lnode = leaf->left;
  node* rnode = leaf->right;
  int ldiff = 0, rdiff = 0;
  if (lnode)
    ldiff = leaf->rank - lnode->rank;
  else
    ldiff = leaf->rank;
  if (rnode)
    rdiff = leaf->rank - rnode->rank;
  else
    rdiff = leaf->rank;

  // 2,2 || 2,3 || 3,2
  if (ldiff > 1 && rdiff > 1)
  {
    leaf->rank -= 1;
    return;
  }

  // 1,3
  else if (ldiff == 1 && rdiff > 2)
  {
    int l_lrank = 0;
    if (lnode->left)
      l_lrank = lnode->left->rank;
    // [1] Single rotation (right)
    if (lnode->rank - l_lrank == 1)
      right_rotation(leaf);

    // [2] Double rotation (lr)
    else
      lr_rotation(leaf);

    balance_remove(leaf);
  }

  // 3,1
  else if (rdiff == 1 && ldiff > 2)
  {
    int r_rrank = 0;
    if (rnode->right)
      r_rrank = rnode->right->rank;
    // [1] Single rotation (left)
    if (rnode->rank - r_rrank == 1)
      left_rotation(leaf);

    // [2] Double rotation
    else
      rl_rotation(leaf);
    balance_remove(leaf);
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
