// Implementing Red-Black Tree in C++

#include <iostream>
using namespace std;
#include <stack>

enum Color { BLACK = 0, RED = 1 };

struct Node {
  string data;
  Node *parent;
  Node *left;
  Node *right;
  enum Color color;
  size_t count;
};

typedef Node *NodePtr;

class RedBlackTree {
   private:
  NodePtr root;
  NodePtr TNULL;

  void initializeNULLNode(NodePtr node, NodePtr parent) {
    node->data = "";
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = BLACK;
  }

  // Preorder
  void preOrderHelper(NodePtr node) {
    if (node != TNULL) {
      cout << node->data << " ";
      preOrderHelper(node->left);
      preOrderHelper(node->right);
    }
  }

  // Inorder
  void inOrderHelper(NodePtr node) {
    if (node != TNULL) {
      inOrderHelper(node->left);
      cout << node->data << " ";
      inOrderHelper(node->right);
    }
  }

  // Post order
  void postOrderHelper(NodePtr node) {
    if (node != TNULL) {
      postOrderHelper(node->left);
      postOrderHelper(node->right);
      cout << node->data << " ";
    }
  }

  NodePtr searchTreeHelper(NodePtr node, string key) {
    if (node == TNULL || key == node->data) {
      return node;
    }

    if (key < node->data) {
      return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
  }

  // For balancing the tree after deletion
  void deleteFix(NodePtr x) {
    // std::cout << "deleteFix" << std::endl;
    NodePtr s;
    if (x == root)
      x->count = 1;
    while (x != root && x->color == BLACK) {
      if (x == x->parent->left) {
        s = x->parent->right;
        if (s->color == RED) {
          s->color = BLACK;
          x->parent->color = RED;
          // std::cout << "this one" << std::endl;
          leftRotate(x->parent);
          s = x->parent->right;
        }

        if (s->left->color == BLACK && s->right->color == BLACK) {
          s->color = RED;
          x = x->parent;
        } else {
          if (s->right->color == BLACK) {
            s->left->color = BLACK;
            s->color = RED;
            rightRotate(s);
            s = x->parent->right;
            // s->count = (s->right == TNULL ? 0 : s->right->count) + ()
          }

          s->color = x->parent->color;
          x->parent->color = BLACK;
          s->right->color = BLACK;
          leftRotate(x->parent);
          x = root;
        }
      } else {
        s = x->parent->left;
        if (s->color == RED) {
          s->color = BLACK;
          x->parent->color = RED;
          rightRotate(x->parent);
          s = x->parent->left;
        }

        if (s->right->color == BLACK && s->right->color == BLACK) {
          s->color = RED;
          x = x->parent;
        } else {
          if (s->left->color == BLACK) {
            s->right->color = BLACK;
            s->color = RED;
            leftRotate(s);
            s = x->parent->left;
          }

          s->color = x->parent->color;
          x->parent->color = BLACK;
          s->left->color = BLACK;
          rightRotate(x->parent);
          x = root;
        }
      }
    }
    x->color = BLACK;
  }

  //                        5          6
  void rbTransplant(NodePtr u, NodePtr v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
    v->count = (u->right == TNULL ? 0 : u->right->count) + (u->left == TNULL ? 0 : u->left->count) + 1;
  }

  void deleteNodeHelper(NodePtr node, string key) {
    NodePtr z = TNULL;
    NodePtr x, y;
    while (node != TNULL) {
      if (node->data == key) {
        z = node;
      }

      if (node->data <= key) {
        node = node->right;
      } else {
        node = node->left;
      }
    }

    if (z == TNULL) {
      cout << "Key not found in the tree" << endl;
      return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
      x = z->right;
      rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
      x = z->left;
      rbTransplant(z, z->left);
    } else {
      y = minimum(z->right);
      reduceCount(y);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z) {
        x->parent = y;
      } else {
        rbTransplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }

      rbTransplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    if (z == TNULL || z->parent == TNULL) {
      // no need to recount
    }
    delete z;
    if (y_original_color == BLACK) {
      deleteFix(x);
      // x->count = (x->right == TNULL ? 0 : x->right->count) + (x->left == TNULL ? 0 : x->left->count) + 1;
    }
      updateCount(x);
  }

  // For balancing the tree after insertion
  void insertFix(NodePtr k) {
    NodePtr u;
    while (k->parent->color == RED) {
      if (k->parent == k->parent->parent->right) {
        u = k->parent->parent->left;
        if (u->color == RED) {
          u->color = BLACK;
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          k = k->parent->parent;
        } else {
          if (k == k->parent->left) {
            k = k->parent;
            rightRotate(k);
          }
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          leftRotate(k->parent->parent);
        }
      } else {
        u = k->parent->parent->right;

        if (u->color == RED) {
          u->color = BLACK;
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          k = k->parent->parent;
        } else {
          if (k == k->parent->right) {
            k = k->parent;
            leftRotate(k);
          }
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          rightRotate(k->parent->parent);
        }
      }
      if (k == root) {
        break;
      }
    }
    root->color = BLACK;
  }

  void printHelper(NodePtr root, string indent, bool last) {
    if (root != TNULL) {
      cout << indent;
      if (last) {
        cout << "R----";
        indent += "   ";
      } else {
        cout << "L----";
        indent += "|  ";
      }

      string sColor = root->color ? "RED" : "BLACK";
      cout << root->data << "(" << sColor << ") " << root->count << endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

   public:
  RedBlackTree() {
    TNULL = new Node;
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
  }

  void preorder() {
    preOrderHelper(this->root);
  }

  void inorder() {
    inOrderHelper(this->root);
  }

  void postorder() {
    postOrderHelper(this->root);
  }

  NodePtr searchTree(string k) {
    return searchTreeHelper(this->root, k);
  }

  NodePtr minimum(NodePtr node) {
    while (node->left != TNULL) {
      node = node->left;
    }
    return node;
  }

  NodePtr maximum(NodePtr node) {
    while (node->right != TNULL) {
      node = node->right;
    }
    return node;
  }

  NodePtr successor(NodePtr x) {
    if (x->right != TNULL) {
      return minimum(x->right);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  NodePtr predecessor(NodePtr x) {
    if (x->left != TNULL) {
      return maximum(x->left);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->left) {
      x = y;
      y = y->parent;
    }

    return y;
  }

  void leftRotate(NodePtr x) {
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    x->count = (x->left == TNULL ? 0 : x->left->count) + (x->right == TNULL ? 0 : x->right->count) + 1;
    y->count = x->count + (y->right == TNULL ? 0 : y->right->count) + 1;
  }

  void rightRotate(NodePtr x) {
    NodePtr y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
      y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    x->count = (x->left == TNULL ? 0 : x->left->count) + (x->right == TNULL ? 0 : x->right->count) + 1;
    y->count = x->count + (y->left == TNULL ? 0 : y->left->count) + 1;
  }

  // Inserting a node
  void insert(string key) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->data = key;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;
    node->count = 1;

    NodePtr y = nullptr;
    NodePtr x = this->root;

    while (x != TNULL) {
      y = x;
      x->count++;
      if (node->data < x->data) {
        x = x->left;
      } else {
        x = x->right;
      }
    }

    node->parent = y;
    if (y == nullptr) {
      root = node;
    } else if (node->data < y->data) {
      y->left = node;
    } else {
      y->right = node;
    }

    if (node->parent == nullptr) {
      node->color = BLACK;
      return;
    }

    if (node->parent->parent == nullptr) {
      return;
    }

    insertFix(node);
  }

  NodePtr getRoot() {
    return this->root;
  }

  void deleteNode(string data) {
    deleteNodeHelper(this->root, data);
  }

  void reduceCount(NodePtr start) {
    while (1) {
        start->count--;
        if (start == getRoot())
            break;
        start = start->parent;
    }
  }

  void updateCount(NodePtr start) {
    if (start == TNULL || start == nullptr)
      return;
    while (1) {
        start->count = (start->right == TNULL ? 0 : start->right->count) + (start->left == TNULL ? 0 : start->left->count) + 1;
        if (start == getRoot())
            break;
        start = start->parent;
    }
  }

  void updatePathCount(stack<NodePtr> path) {
    NodePtr start;
    while (path.size() > 0) {
        start = path.top();
        path.pop();
        start->count = (start->right == TNULL ? 0 : start->right->count) + (start->left == TNULL ? 0 : start->left->count) + 1;
    }
  }

  NodePtr findByIndex(NodePtr node, size_t index, stack<NodePtr>& path, bool to_delete = false) {
		size_t leftCount = (node->left == TNULL) ? 0 : node->left->count;

    path.push(node);
		if (leftCount == index) {
			return node;
		}
		else if (index <= leftCount) {
			return findByIndex(node->left, index, path, to_delete);
		}
		else {
			return findByIndex(node->right, index - leftCount - 1, path, to_delete);
		}
	}

  NodePtr find(size_t index) {
    std::stack<NodePtr> path;

    return findByIndex(getRoot(), index, path, false);
  }

  void deleteByIndex(size_t index) {
    std::stack<NodePtr> path;

    NodePtr node = findByIndex(getRoot(), index, path, false);
    reduceCount(node);
    deleteNodeHelper(node, node->data);
    path.pop(); // to remove the deleted node
    updatePathCount(path);
  }

  void printTree() {
    // if (root) {
    //   printHelper(this->root, "", true);
    // }
  }
};

// int main() {
//   RedBlackTree bst;
//   bst.insert("0");
//   bst.insert("1");
//   bst.insert("2");
//   bst.insert("3");
//   bst.insert("4");
//   bst.insert("5");
//   bst.insert("6");
//   bst.insert("7");
//   bst.insert("8");
//   bst.insert("9");



//   bst.printTree();
//   cout << endl
//      << "After deleting" << endl;
//   bst.deleteByIndex(0);
//   bst.printTree();
//   bst.deleteByIndex(0);
//   bst.printTree();
//   bst.deleteByIndex(0);
//   bst.printTree();
//   bst.deleteByIndex(0);
//   bst.printTree();
//   bst.deleteByIndex(0);
//   bst.printTree();
//   bst.deleteByIndex(0);
//   bst.printTree();
//   bst.deleteByIndex(1);
//   bst.printTree();
// }