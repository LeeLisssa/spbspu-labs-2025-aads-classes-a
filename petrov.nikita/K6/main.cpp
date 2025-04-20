#include <iostream>
#include <string>

template< class T >
struct BiTree
{
  T data;
  BiTree< T > * left, * right, * parent;
};

template< class T >
BiTree< T > * rotate_right(BiTree< T > * root)
{
  if (!root->left)
  {
    return nullptr;
  }
  auto son = root->left;
  auto temp = root;
  root->left = nullptr;
  root = son;
  root->right = temp;
  temp->parent = root;
  root->parent = nullptr;
  return root;
}

template< class T >
BiTree< T > * rotate_left(BiTree< T > * root)
{
  if (!root->right)
  {
    return nullptr;
  }
  auto son = root->right;
  auto temp = root;
  root->right = nullptr;
  root = son;
  root->left = temp;
  temp->parent = root;
  root->parent = nullptr;
  return root;
}

template< class T, class Cmp >
BiTree< T > * find(BiTree< T > * root, const T & value, Cmp cmp)
{
  if (!root)
  {
    return root;
  }
  while (root->data != value)
  {
    if (cmp(value, root->data) && root->left)
    {
      root = root->left;
    }
    else if (!cmp(value, root->data) && root->right)
    {
      root = root->right;
    }
    else
    {
      root = nullptr;
      break;
    }
  }
  return root;
}

template< class T, class Cmp >
BiTree< T > * convert(const T * array, size_t size, Cmp cmp)
{
  if (!array)
  {
    return nullptr;
  }
  BiTree< T > * root = new BiTree< T >{ array[0], nullptr, nullptr, nullptr };
  try
  {
    for (size_t i = 1; i < size; i++)
    {
      auto temp = root;
      while (temp->left || temp->right)
      {
        if (cmp(array[i], temp->data) && temp->left)
        {
          temp = temp->left;
        }
        else if (!cmp(array[i], temp->data) && temp->right)
        {
          temp = temp->right;
        }
        else
        {
          break;
        }
      }
      if (cmp(array[i], temp->data))
      {
        temp->left = new BiTree< T >{ array[i], nullptr, nullptr, temp };
      }
      else
      {
        temp->right = new BiTree< T >{ array[i], nullptr, nullptr, temp };
      }
    }
  }
  catch(const std::bad_alloc & e)
  {
    clearBiTree(root);
    throw;
  }
  return root;
}

template< class T >
void clearBiTree(BiTree< T > * root)
{
  if (!root)
  {
    return;
  }
  while (root->left)
  {
    root = root->left;
  }
  while (root->parent)
  {
    auto todelete = root;
    root = root->parent;
    delete todelete;
  }
  while (root->right)
  {
    auto todelete = root;
    root = root->right;
    delete todelete;
  }
  delete root;
}

int main()
{
  size_t sequence_length = 0;
  if (!(std::cin >> sequence_length))
  {
    std::cerr << "ERROR: Invalid argument" << "\n";
    return 1;
  }
  else if (std::cin.eof())
  {
    std::cerr << "End of file" << "\n";
    return 2;
  }
  int * elements_array = nullptr;
  BiTree< int > * root = nullptr;
  try
  {
    if (sequence_length)
    {
      elements_array = new int[sequence_length];
    }
    else
    {
      elements_array = nullptr;
    }
    size_t i = 0;
    while(i != sequence_length && std::cin && !std::cin.eof())
    {
      std::cin >> elements_array[i++];
    }
    if (!std::cin)
    {
      throw std::invalid_argument("ERROR: Invalid argument");
    }
    else if (std::cin.eof())
    {
      throw std::logic_error("End of file");
    }
    root = convert(elements_array, sequence_length, std::less< int >());
  }
  catch (const std::invalid_argument & e)
  {
    clearBiTree(root);
    delete[] elements_array;
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (const std::logic_error & e)
  {
    clearBiTree(root);
    delete[] elements_array;
    std::cerr << e.what() << "\n";
    return 2;
  }
  catch (const std::bad_alloc & e)
  {
    clearBiTree(root);
    delete[] elements_array;
    std::cerr << "ERROR: Out of memory" << "\n";
    return 3;
  }
  std::string type_of_command;
  int number = 0;
  while (!std::cin.eof())
  {
    std::cin >> type_of_command;
    std::cin >> number;
    if (std::cin.eof())
    {
      break;
    }
    else if (!std::cin)
    {
      clearBiTree(root);
      delete[] elements_array;
      std::cerr << "<INVALID COMMAND>";
      std::cerr << "\n";
      return 4;
    }
    BiTree< int > * subroot = find(root, number, std::less< int >());
    if (type_of_command == "left" && subroot)
    {
      auto new_subroot = rotate_left(subroot);
      if (new_subroot)
      {
        std::cout << new_subroot->data;
        std::cout << "\n";
        while (root->parent)
        {
          root = root->parent;
        }
      }
      else
      {
        std::cout << "<INVALID ROTATE>";
        std::cout << "\n";
      }
    }
    else if (type_of_command == "right" && subroot)
    {
      auto new_subroot = rotate_right(subroot);
      if (new_subroot)
      {
        std::cout << new_subroot->data;
        std::cout << "\n";
        while (root->parent)
        {
          root = root->parent;
        }
      }
      else
      {
        std::cout << "<INVALID ROTATE>";
        std::cout << "\n";
      }
    }
    else
    {
      std::cout << "<INVALID ROTATE>";
      std::cout << "\n";
    }
  }
  clearBiTree(root);
  delete[] elements_array;
}
