#include <iostream>
#include "iterator.hpp"

namespace ft
{
template<class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
class RBTree {
	public:
		typedef Node<T> node_type;
        typedef T value_type;
		typedef typename T::first_type key_type;
		typedef typename T::second_type mapped_type;
        typedef Compare value_comp;
        typedef Allocator allocator_type;
		typedef typename Allocator::template rebind<Node<T> >::other node_allocator;
		typedef typename node_allocator::pointer node_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef ft::map_iterator<value_type> iterator;
        typedef ft::map_iterator<const value_type> const_iterator;
        typedef ft::VecRevIterator<iterator> reverse_iterator;
        typedef ft::VecRevIterator<const iterator> const_reverse_iterator;

private:

	node_pointer 		_root;
	node_pointer 		_sentinal;
	node_allocator _node_alloc;
	allocator_type _val_alloc;
	value_comp		_comp;
	size_t			_size;
 
public:

	size_t	size() const
	{
		return _size;
	}

	node_pointer get_root()
	{
		return _root;
	}

	node_pointer create_node(const value_type &val = value_type(), bool is_sentinal = false)
	{
		node_pointer node;
		pointer new_val;

		node = _node_alloc.allocate(1);
		new_val = _val_alloc.allocate(1);

		_val_alloc.construct(new_val, val);
		_node_alloc.construct(node, node_type(new_val, nullptr, _sentinal, _sentinal, is_sentinal, 1));

		return (node);
	}

	node_pointer copy_node(node_pointer to_copy)
	{
		node_pointer node;
		pointer new_val;

		node = _node_alloc.allocate(1);
		new_val = _val_alloc.allocate(1);

		_val_alloc.construct(new_val, *to_copy->_data);
		_node_alloc.construct(node, node_type(new_val, nullptr, _sentinal, _sentinal, false, to_copy->_color));

		return (node);
	}

	//constructor
	RBTree(const value_comp comp = value_comp(), const Allocator alloc = Allocator()) : _val_alloc(alloc),
																							_comp(comp),
																							_size(0)
	{
		_node_alloc = node_allocator();
		_sentinal = create_node(value_type()) ;
		_sentinal->_parent = _sentinal;
		_sentinal->_color = 0;
		_sentinal->_left = _sentinal;
		_sentinal->_right = _sentinal;
		_sentinal->_is_sentinal = true;
		_root = _sentinal;
	}

	RBTree(const RBTree &other) : _node_alloc(other._node_alloc),
									_val_alloc(other._val_alloc),
									_comp(other._comp)
	{
		_sentinal = create_node(value_type()) ;
		_sentinal->_parent = _sentinal;
		_sentinal->_color = 0;
		_sentinal->_left = _sentinal;
		_sentinal->_right = _sentinal;
		_sentinal->_is_sentinal = true;
		_root = _sentinal;
		if (other.size() == 0)
			return ;
		// _root = replicate(_root, other._root);
		const_iterator it = other.begin();
		while (it != other.end())
		{
			insert(*it);
			it++;
		}
		_size = other.size();
		// std::cout << "RAN " << _root->_is_sentinal << std::endl;
	}

	RBTree& operator=( const RBTree& other )
    {
		if (_size > 0)
			clear(get_root());
		_size = other._size;
		if (other._root == other._sentinal)
			return (*this);
		replicate(_root, other._root);
		return (*this);
		// _root = _sentinal;
		// _val_alloc = other._val_alloc;
		// _comp = other._comp;
		// _size = other._size;
        // _node_alloc = other._node_alloc;

		// if (other.size() == 0)
		// 	return (*this);
		// _root = replicate(_root, other._root);
		// _sentinal->_right = _root;
		// _sentinal->_left = _root;
		// prettyPrint();
		// std::cout << "ROOT SEN: " << _root->_is_sentinal << std::endl;
		// std::cout << "OTHER ROOT SEN: " << other._root->_is_sentinal << std::endl;
		// // other.prettyPrint();
    } 

	//destructor
	~RBTree()
	{
		if (_size > 0)
			clear();
		delete_node(_sentinal);
	}

	// find the node with the min key
	node_pointer min(node_pointer node) const
	{
		while (node->_left != _sentinal) {
			node = node->_left;
		}
		return node;
	}

	// find the node with the max key
	node_pointer max(node_pointer node) const
	{
		while (node->_right != _sentinal) {
			node = node->_right;
		}
		return node;
	}

	iterator begin()
	{
		return (iterator(min(_root)));
	}

	const_iterator begin() const
	{
		return (const_iterator(min(_root)));
	}

	iterator end()
	{
		return (iterator(get_sentinal()));
	}

	const_iterator end() const
	{
		return (const_iterator(get_sentinal()));
	}

	reverse_iterator rbegin()
	{
		return (reverse_iterator(max(_root)));
	}

	reverse_iterator rend()
	{
		return (reverse_iterator(get_sentinal()));
	}

	node_pointer	find_val(node_pointer node, const value_type &val) const
	{
		// std::cout << "IN" << std::endl;
		if (node->_is_sentinal)
			return node;
		if (_comp(val, *node->_data))
			return find_val(node->_left, val);
		if (_comp(*node->_data, val))
			return find_val(node->_right, val);
		return node;
	}

	void preOrderHelper(node_pointer node) {
		if (node != _sentinal) {
			std::cout<<node->_data<<" ";
			preOrderHelper(node->_left);
			preOrderHelper(node->_right);
		} 
	}

	void inOrderHelper(node_pointer node) {
		if (node != _sentinal) {
			inOrderHelper(node->_left);
			std::cout<<node->data<<" ";
			inOrderHelper(node->_right);
		} 
	}

	void postOrderHelper(node_pointer node) {
		if (node != _sentinal) {
			postOrderHelper(node->_left);
			postOrderHelper(node->_right);
			std::cout<<node->data<<" ";
		} 
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(node_pointer x) {
		node_pointer s;
		while (x != _root && x->_color == 0) {
			if (x == x->_parent->_left) {
				s = x->_parent->_right;
				if (s->_color == 1) {
					// case 3.1
					s->_color = 0;
					x->_parent->_color = 1;
					leftRotate(x->_parent);
					s = x->_parent->_right;
				}

				if (s->_left->_color == 0 && s->_right->_color == 0) {
					// case 3.2
					s->_color = 1;
					x = x->_parent;
				} else {
					if (s->_right->_color == 0) {
						// case 3.3
						s->_left->_color = 0;
						s->_color = 1;
						rightRotate(s);
						s = x->_parent->_right;
					} 

					// case 3.4
					s->_color = x->_parent->_color;
					x->_parent->_color = 0;
					s->_right->_color = 0;
					leftRotate(x->_parent);
					x = _root;
				}
			} else {
				s = x->_parent->_left;
				if (s->_color == 1) {
					// case 3.1
					s->_color = 0;
					x->_parent->_color = 1;
					rightRotate(x->_parent);
					s = x->_parent->_left;
				}

				if (s->_right->_color == 0 && s->_right->_color == 0) {
					// case 3.2
					s->_color = 1;
					x = x->_parent;
				} else {
					if (s->_left->_color == 0) {
						// case 3.3
						s->_right->_color = 0;
						s->_color = 1;
						leftRotate(s);
						s = x->_parent->_left;
					} 

					// case 3.4
					s->_color = x->_parent->_color;
					x->_parent->_color = 0;
					s->_left->_color = 0;
					rightRotate(x->_parent);
					x = _root;
				}
			} 
		}
		x->_color = 0;
	}


	void rbTransplant(node_pointer u, node_pointer v){
		if (u->_parent->_is_sentinal) {
			_root = v;
		} else if (u == u->_parent->_left){
			u->_parent->_left = v;
		} else {
			u->_parent->_right = v;
		}
		v->_parent = u->_parent;
	}
	
	// fix the red-black tree
	void fixInsert(node_pointer k){
		node_pointer u;
		while (k->_parent->_color == 1) {
			if (k->_parent == k->_parent->_parent->_right) {
				u = k->_parent->_parent->_left; // uncle
				if (u->_color == 1) {
					// case 3.1
					u->_color = 0;
					k->_parent->_color = 0;
					k->_parent->_parent->_color = 1;
					k = k->_parent->_parent;
				} else {
					if (k == k->_parent->_left) {
						// case 3.2.2
						k = k->_parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->_parent->_color = 0;
					k->_parent->_parent->_color = 1;
					leftRotate(k->_parent->_parent);
				}
			} else {
				u = k->_parent->_parent->_right; // uncle

				if (u->_color == 1) {
					// mirror case 3.1
					u->_color = 0;
					k->_parent->_color = 0;
					k->_parent->_parent->_color = 1;
					k = k->_parent->_parent;	
				} else {
					if (k == k->_parent->_right) {
						// mirror case 3.2.2
						k = k->_parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->_parent->_color = 0;
					k->_parent->_parent->_color = 1;
					rightRotate(k->_parent->_parent);
				}
			}
			if (k == _root) {
				break;
			}
		}
		_sentinal->_right = _root;
		_sentinal->_left = _root;
		_root->_color = 0;
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->_root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->_root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->_root);
	}

	// find the successor of a given node
	node_pointer successor(node_pointer x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != _sentinal) {
			return min(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		node_pointer y = x->parent;
		while (y != _sentinal && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	node_pointer predecessor(node_pointer x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != _sentinal) {
			return max(x->left);
		}

		node_pointer y = x->parent;
		while (y != _sentinal && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(node_pointer x) {
		node_pointer y = x->_right;
		x->_right = y->_left;
		if (y->_left != _sentinal) {
			y->_left->_parent = x;
		}
		y->_parent = x->_parent;
		if (x->_parent->_is_sentinal) {
			this->_root = y;
		} else if (x == x->_parent->_left) {
			x->_parent->_left = y;
		} else {
			x->_parent->_right = y;
		}
		y->_left = x;
		x->_parent = y;
	}

	// rotate _right at node x
	void rightRotate(node_pointer x) {
		node_pointer y = x->_left;
		x->_left = y->_right;
		if (y->_right != _sentinal) {
			y->_right->_parent = x;
		}
		y->_parent = x->_parent;
		if (x->_parent->_is_sentinal) {
			this->_root = y;
		} else if (x == x->_parent->_right) {
			x->_parent->_right = y;
		} else {
			x->_parent->_left = y;
		}
		y->_right = x;
		x->_parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	node_pointer insert(const value_type &val) {
		// Ordinary Binary Search Insertion
		node_pointer node = create_node(val);
		node->_parent = nullptr;
		node->_left = _sentinal;
		node->_right = _sentinal;
		node->_color = 1; // new node must be red
		node_pointer y = _sentinal;
		node_pointer x = _root;

		while (!x->_is_sentinal)
        {
			y = x;
			if (_comp(*node->_data, *x->_data))
            {
				x = x->_left;
			}
            else
            {
				x = x->_right;
			}
		}
		// y is parent of x
		node->_parent = y;
		if (y->_is_sentinal) {
			_root = node;
			_sentinal->_right = _root;
			_sentinal->_left = _root;
		} else if (_comp(*node->_data, *y->_data)) {
			y->_left = node;
		} else {
			y->_right = node;
		}

		// if new node is a _root node, simply return
		_size++;
		if (node->_parent->_is_sentinal){
			node->_color = 0;
			return (node);
		}

		// if the grandparent is null, simply return
		if (node->_parent->_parent->_is_sentinal) {
			return (node);
		}

		// Fix the tree
		fixInsert(node);
		return (node);
	}

	void delete_node(node_pointer node)
	{
		_val_alloc.destroy((node->_data));
		_val_alloc.deallocate((node->_data), 1);
		_node_alloc.destroy(node);
		_node_alloc.deallocate(node, 1);
	}

	void del_root()
	{
		node_pointer newroot;

		if (_size == 1)
		{

		}
		if (!_root->_left->_is_sentinal) 
		{
			newroot = _root->_left;
			_sentinal->_left = newroot;
			_sentinal->_right = newroot;
			newroot->_parent = _sentinal;
		}
		else if (!_root->_right->_is_sentinal) 
		{
			newroot = _root->_right;
			_sentinal->_left = newroot;
			_sentinal->_right = newroot;
			newroot->_parent = _sentinal;	
		}

		--_size;
	}

	void deleteNodeHelper(node_pointer to_delete) {

		node_pointer x, y;

		if (to_delete == _sentinal) {
			return;
		}

		y = to_delete;
		int y_original_color = y->_color;
		if (to_delete->_left == _sentinal) {
			x = to_delete->_right;
			rbTransplant(to_delete, to_delete->_right);
		} else if (to_delete->_right == _sentinal) {
			x = to_delete->_left;
			rbTransplant(to_delete, to_delete->_left);
		} else {
			y = min(to_delete->_right);
			y_original_color = y->_color;
			x = y->_right;
			if (y->_parent == to_delete) {
				x->_parent = y;
			} else {
				rbTransplant(y, y->_right);
				y->_right = to_delete->_right;
				y->_right->_parent = y;
			}

			rbTransplant(to_delete, y);
			y->_left = to_delete->_left;
			y->_left->_parent = y;
			y->_color = to_delete->_color;
		}
		delete_node(to_delete);
		_size--;
		if (y_original_color == 0)
			fixDelete(x);
		if (_size == 0)
		{
			_sentinal->_parent = _sentinal;
			_sentinal->_color = 0;
			_sentinal->_left = _sentinal;
			_sentinal->_right = _sentinal;
			_sentinal->_is_sentinal = true;
			_root = _sentinal;
		}
	}

	void deleteNode(key_type key)
	{
		node_pointer val = find_key(key);
		if (!val->_is_sentinal)
			deleteNodeHelper(val);
	}

	void printHelper(node_pointer node, std::string indent, bool last) const {
		// print the tree structure on the screen
	   	if (!node->_is_sentinal) {
		   std::cout<<indent;
		   if (last) {
		      std::cout << "R----";
		      indent += "     ";
		   } else {
		      std::cout << "L----";
		      indent += "|    ";
		   }
            
           std::string color = node->_color ? "RED" : "BLACK";
		   std::cout<<node->_data->first << ":" << node->_data->second << " " << "(" << color << ")" << std::endl;
		   printHelper(node->_left, indent, false);
		   printHelper(node->_right, indent, true);
		}
		// std::cout<<root->left->data<<std::endl;
	}

	void prettyPrint() const {
		
	    if (!_root->_is_sentinal) {
    		printHelper(_root, "", true);
	    }
	}

	node_pointer find_key(key_type key) const
	{
		node_pointer node;

		value_type val(key, mapped_type());

		node = find_val(_root, val);
		return (node);
	}

	node_pointer get_sentinal() const
	{
		return _sentinal;
	}

	void clear()
	{
		rec_del_node(_root);
		_sentinal->_color = 0;
		_sentinal->_is_sentinal = true;
		_sentinal->_parent = _sentinal;
		_sentinal->_left = _sentinal;
		_sentinal->_right = _sentinal;
		_root = _sentinal;
		_size = 0;
	}
	
	void rec_del_node(node_pointer node)
	{
		if (node->_is_sentinal)
			return;
		rec_del_node(node->_right);
		rec_del_node(node->_left);
		delete_node(node);
	}

	node_pointer replicate(node_pointer node, node_pointer src_node)
	{
		if (src_node->_is_sentinal)
			return node;
		std::cout << "ran" << std::endl;
		node = copy_node(src_node);
		// std::cout << "copied one node is sen: " << node->_is_sentinal << std::endl;
		if (!src_node->_left->_is_sentinal) {
			replicate(node->_left, src_node->_left);
			node->_left->_parent = node;
		}
		if (!src_node->_right->_is_sentinal) {
			replicate(node->_right, src_node->_right);
			node->_right->_parent = node;
		}
		return node;
	}

	void swap_tree(RBTree &other)
	{
		node_pointer temp_root(other._root);
		node_pointer temp_sentinal(other._sentinal);
		node_allocator temp_node_alloc(other._node_alloc);
		allocator_type temp_val_alloc(other._val_alloc);
		value_comp temp_comp(other._comp);
		size_t temp_size(other._size);

		other._root = _root;
		other._sentinal = _sentinal;
		other._node_alloc = _node_alloc;
		other._val_alloc = _val_alloc;
		other._comp = _comp;
		other._size = _size;

		_root = temp_root;
		_sentinal = temp_sentinal;
		_node_alloc = temp_node_alloc;
		_val_alloc = temp_val_alloc;
		_comp = temp_comp;
		_size = temp_size;
	}

	value_comp get_compare() const
	{
		return _comp;
	}
};
}

