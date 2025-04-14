#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <limits>
#include <stdexcept>
template< class T, class Cmp >
struct BiTree
{
  T data;
  Cmp cmp;
  BiTree< T, Cmp >* left;
  BiTree< T, Cmp >* right;
  BiTree< T, Cmp >* parent;
  BiTree(const T& value):
    data(value),
    left(nullptr),
    right(nullptr),
    parent(nullptr)
  {}
};
template< class T, class Cmp >
BiTree< T, Cmp >* insert(BiTree< T, Cmp >* root, const T& value)
{
  if (!root)
  {
    return new BiTree< T, Cmp >(value);
  }
  if (root->cmp(value, root->data))
  {
    root->left = insert(root->left, value);
    root->left->parent = root;
  }
  else
  {
    root->right = insert(root->right, value);
    root->right->parent = root;
  }
  return root;
}
template< class T, class Cmp >
BiTree< T, Cmp >* extract(BiTree< T, Cmp >* root, const T& value, BiTree< T, Cmp >** result)
{
  if (!root)
  {
    return nullptr;
  }
  if (root->data == value)
  {
    *result = root;
    if (!root->left && !root->right)
    {
      BiTree< T, Cmp >* temp = root;
      delete root;
      return temp;
    }
    if (!root->left)
    {
      BiTree< T, Cmp >* temp = root;
      root = root->right;
      delete temp;
      return temp;
    }
    if (!root->right)
    {
      BiTree< T, Cmp >* temp = root;
      root = root->left;
      delete temp;
      return temp;
    }
    BiTree< T, Cmp >* minNode = root->right;
    while (minNode && minNode->left)
    {
      minNode = minNode->left;
    }
    root->data = minNode->data;
    auto deletedNode = extract(root->right, minNode->data, result);
    return deletedNode;
  }
  if (root->cmp(value, root->data))
  {
    root->left = extract(root->left, value, result);
  }
  else
  {
    root->right = extract(root->right, value, result);
  }
  return nullptr;
}
void printInOrder(BiTree< int, std::less< int > >* node)
{
  if (!node)
  {
    return;
  }
  printInOrder(node->left);
  std::cout << node->data << " ";
  printInOrder(node->right);
}
void clearTree(BiTree< int, std::less< int > >* node)
{
  if (!node)
  {
    return;
  }
  clearTree(node->left);
  clearTree(node->right);
  delete node;
}
int main()
{
  int length = 0;
  if (!(std::cin >> length))
  {
    std::cerr << "ERROR: no length\n";
    return 1;
  }
  if (length < 0)
  {
    std::cerr << "ERROR: invalid length\n";
    return 1;
  }
  BiTree< int, std::less< int > >* treeRoot = nullptr;
  for (int i = 0; i < length; ++i)
  {
    int value = 0;
    if (!(std::cin >> value))
    {
      std::cerr << "ERROR: failed to read element\n";
      clearTree(treeRoot);
      return 1;
    }
    treeRoot = insert(treeRoot, value);
  }
  int deleteValue = 0;
  while (std::cin >> deleteValue)
  {
    BiTree< int, std::less< int > >* deletedNode = nullptr;
    treeRoot = extract(treeRoot, deleteValue, &deletedNode);
    if (!deletedNode)
    {
      std::cerr << "<INVALID NODE>\n";
      return 1;
    }
    else
    {
      delete deletedNode;
      deletedNode = nullptr;
    }
  }
  if (std::cin.fail() && !std::cin.eof())
  {
    std::cerr.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    printInOrder(treeRoot);
    std::cerr << "\n";
    clearTree(treeRoot);
    return 1;
  }
  clearTree(treeRoot);
  return 0;
}
