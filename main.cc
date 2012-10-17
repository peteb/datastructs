#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>

template<typename T>
class avl_set {
private:
  class node {
    node **select(const T &val) {
      return (val < value ? &left : &right);
    }

    void balance() {
      const int diff = (left ? left->height() : 0) - (right ? right->height() : 0);
      if (std::abs(diff) > 1) {
	std::cout << "need to re-balance" << std::endl;
      }

      if (parent)
	parent->balance();
    }
    
  public:
    node(const T &value, node *parent) : value(value), left(), right(), parent(parent) {}
    T value;
    node *left, *right, *parent;

    void insert(const T &val) {
      node **branch = select(val);
      assert(branch);
      if (!*branch) { // the branch doesn't exist
        *branch = new node(val, this);
        // TODO: check ancestors (parents) that their children heights differ by at most 1. otherwise we need to rotate.
	balance();
      }
      else {
        (*branch)->insert(val);
      }
    }

    void print(int level = 0) {
      std::string indent;
      for (int i = 0; i < level; ++i)
        indent += "  ";

      std::cout << indent << value << std::endl;
      if (left) {
        std::cout << indent << " left: " << std::endl;
        left->print(level + 1);
      }
      
      if (right) {
        std::cout << indent << " right: " << std::endl;
        right->print(level + 1);
      }
    }

    bool has(const T &val) {
      if (val == value) { // TODO: we don't want to need operator ==... especially not at every step
        return true;
      }
      else {
        if (node *branch = *select(val))  // this is safe
          return branch->has(val);
        else
          return false;
      }
    }

    std::size_t height() const {
      std::size_t h_l = 0;
      std::size_t h_r = 0;

      // TODO: cache height, invalidate on insert (some false-invalidations)
      //       if we don't care about duplicates, we can just keep running counters
      
      if (left)
        h_l = left->height();
      
      if (right)
        h_r = right->height();

      return std::max(h_l, h_r) + 1u;
    }
  };

  node *root;
public:
  typedef T value_type;

  avl_set() : root() {}
  
  void insert(const value_type &val) {
    if (!root) {
      root = new node(val, 0);
      return;
    }
    else {
      root->insert(val);
    }
  }

  void print() {
    if (root)
      root->print(0);
  }

  bool has(const value_type &val) {
    if (root)
      return root->has(val);
    else
      return false;
  }
};


int main() {
  avl_set<int> s;
  s.insert(1);
  s.insert(-2);
  s.insert(3);
  s.insert(-4);
  s.insert(5);
  s.insert(-6);
  s.print();

  assert(s.has(3));
  assert(!s.has(8));
  assert(s.has(1));
  assert(!s.has(0));
  assert(s.has(-6));
}
