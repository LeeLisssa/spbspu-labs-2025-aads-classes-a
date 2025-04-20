#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

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
  if (root->cmp(value, root->data))
  {
    root->left = extract(root->left, value, result);
  }
  else if (root->cmp(root->data, value))
  {
    root->right = extract(root->right, value, result);
  }
  else
  {
    *result = root;
    if (!root->left)
    {
      return root->right;
    }
    else if (!root->right)
    {
      return root->left;
    }
    BiTree< T,Cmp >* minNode = root->right;
    while (minNode && minNode->left)
    {
      minNode = minNode->left;
    }
    root->data = minNode->data;
    root->right = extract(root->right, minNode->data, result);
  }
  return root;
}
void clearTree(BiTree< int,std::less< int > >* node)
{
  if (!node)
  {
    return;
  }
  clearTree(node->left);
  clearTree(node->right);
  delete node;
}
void printInOrder(BiTree< int,std::less< int > >* node)
{
  if (!node)
  {
    return;
  }
  printInOrder(node->left);
  std::cout << node->data << " ";
  printInOrder(node->right);
}
int main()
{
  int n = 0;
  if (!(std::cin >> n))
  {
    std::cerr << "ERROR: bad data\n";
    return 1;
  }
  BiTree< int,std::less< int > >* treeRoot = nullptr;
  for (int i = 0; i < n; ++i)
  {
    int value = 0;
    if (!(std::cin >> value))
    {
      std::cerr << "ERROR: bad data\n";
      clearTree(treeRoot);
      return 1;
    }
    treeRoot = insert(treeRoot, value);
  }
  std::string line;
  while (std::getline(std::cin >> std::ws , line))
  {
    int value = 0;
    try
    {
      value = std::stoi(line);
      BiTree< int,std::less< int > >* removedNode = nullptr;
      treeRoot = extract(treeRoot, value, &removedNode);
      if (!removedNode)
      {
        std::cerr << "<INVALID NODE>\n";
        break;
      }
      delete removedNode;
    }
    catch (const std::invalid_argument&)
    {
      std::cerr << "ERROR: invalid input\n";
      break;
    }
    catch (const std::out_of_range&)
    {
      std::cerr << "ERROR: value out of range\n";
      break;
    }
  }
  if (treeRoot != nullptr)
  {
    printInOrder(treeRoot);
    std::cout << std::endl;
  }
  clearTree(treeRoot);
  return 0;
}
