#ifndef WAVLTREE_H
#define WAVLTREE_H


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
    void insert_key(node* leaf, KeyType key, ValType val);
    void balance_tree(node* leaf);
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
  insert_key(root, key, val);
  this->count++;
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
  if (leaf != nullptr)
  {
    destroy_tree(leaf->left);
    destroy_tree(leaf->right);
    delete leaf;
  }
}

template <typename KeyType, typename ValType>
void WAVLTree<KeyType, ValType>::insert_key(node* leaf, KeyType key, ValType val)
{
  // if it's the first element to be added into the tree
  if (root == nullptr)
  {
    node* newNode = new node;
    newNode->k = key;
    newNode->v = val;
    newNode->rank = 1;
    root = newNode;
    return;
  }

  // insert the new node at the external node
  if (leaf->k < key)
  {
    if (leaf->right == nullptr)
    {
      node* newNode = new node;
      newNode->k = key;
      newNode->v = val;
      newNode->rank = 1;
      leaf->right = newNode;
      newNode->parent = leaf;
    }
    else
      insert_key(leaf->right, key, val);
  }
  else
  {
    if (leaf->left == nullptr)
    {
      node* newNode = new node;
      newNode->k = key;
      newNode->v = val;
      newNode->rank = 1;
      leaf->left = newNode;
      newNode->parent = leaf;
    }
    else
      insert_key(leaf->left, key, val);
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

  if (rnode != nullptr && leaf->rank == rnode->rank)
  {
    int l_diff;
    if (lnode == nullptr)
      l_diff = leaf->rank;
    else
      l_diff = leaf->rank - lnode->rank;

    if (l_diff > 1)
    {
      int r_r_rank, r_l_rank;
      if (rnode->right != nullptr)
        r_r_rank = rnode->right->rank;
      else
        r_r_rank = 0;
      if (rnode->left != nullptr)
        r_l_rank = rnode->left->rank;
      else
        r_l_rank = 0;

      // [1] rnode == 2,1-node
      if (rnode->rank - r_l_rank > 1)
      {
        // left rotation
        leaf->right = rnode->left;
        leaf->rank -= 1;
        rnode->left = leaf;
        rnode->parent = leaf->parent;
        leaf->parent->left = rnode;
        leaf->parent = rnode;

        if (leaf == root)
          root = rnode;
      }

      // [2] rnode == 1,2-node
      else if (rnode->rank - r_r_rank > 1)
      {
        node* rlnode = rnode->left;
        // right-left
        rnode->left = rlnode->right;
        rnode->rank -= 1;
        rlnode->rank += 1;
        rlnode->parent = leaf;
        rlnode->right = rnode;
        leaf->right = rlnode;
        rnode->parent = rlnode;

        leaf->right = rlnode->left;
        leaf->rank -= 1;
        rlnode->left = leaf;
        rlnode->parent = leaf->parent;
        if (leaf != root)
          leaf->parent->left = rlnode;
        else
          root = rlnode;
        leaf->parent = rlnode;
          
      }

      // [3] rnode == 1,1-node
      else
      {
        leaf->right = rnode->left;
        rnode->left->parent = leaf;
        rnode->left = leaf;
        rnode->parent = leaf->parent;
        rnode->rank += 1;
        if (leaf == root)
          root = rnode;
        else
          leaf->parent->right = rnode;
        leaf->parent = rnode;
      }
    }

    else
      leaf->rank += 1;

  }

  if (lnode != nullptr && leaf->rank == lnode->rank)
  {
    int r_diff;
    if (rnode == nullptr)
      r_diff = leaf->rank;
    else
      r_diff = leaf->rank - rnode->rank;

    // if the current node has a child node with rank 2 difference
    if (r_diff > 1)
    {
      int l_r_rank, l_l_rank;
      if (lnode->right != nullptr)
        l_r_rank = lnode->right->rank;
      else
        l_r_rank = 0;
      if (lnode->left != nullptr)
        l_l_rank = lnode->left->rank;
      else
        l_l_rank = 0;

      // [1] lnode == 1,2-node
      if (lnode->rank - l_r_rank > 1)
      {
        leaf->left = lnode->right;
        leaf->rank -= 1;
        lnode->right = leaf;
        lnode->parent = leaf->parent;
        leaf->parent->right = lnode;
        leaf->parent = lnode;
        if (leaf == root)
          root = lnode;
      }

      // [2] lnode == 2,1-node
      else if (lnode->rank - l_l_rank > 1)
      {
        node* lrnode = lnode->right;
        // left-right
        lnode->right = lrnode->left;
        lnode->rank -= 1;
        lrnode->rank += 1;
        lrnode->parent = leaf;
        lrnode->left = lnode;
        leaf->left = lrnode;
        lnode->parent = lrnode;

        leaf->left = lrnode->right;
        leaf->rank -= 1;
        lrnode->right = leaf;
        lrnode->parent = leaf->parent;
        if (leaf != root)
          leaf->parent->right = lrnode;
        else
          root = lrnode;
        leaf->parent = lrnode;
      }

      // [3] lnode == 1,1-node
      else
      {
        leaf->left = lnode->right;
        lnode->right->parent = leaf;
        lnode->right = leaf;
        lnode->parent = leaf->parent;
        lnode->rank += 1;
        if (leaf == root)
          root = lnode;
        else
          leaf->parent->left = lnode;
        leaf->parent = lnode;
      }
    }
    else
      leaf->rank += 1;
  }
}

#endif /* WAVLTREE_H */
