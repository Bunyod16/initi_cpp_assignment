#ifndef FT_ITERATOR_HPP
# define FT_ITERATOR_HPP

#include <iostream>
#include "map"

namespace ft
{

template <typename Iterator>
class iterator_traits
{
	public:
	typedef typename    Iterator::difference_type difference_type ;
	typedef typename    Iterator::value_type value_type;
	typedef typename    Iterator::pointer pointer;
	typedef typename    Iterator::reference reference;
	typedef typename    Iterator::iterator_category iterator_category;
};

template <class T>
class iterator_traits<T*>
{
	public:
	typedef ptrdiff_t                                   difference_type;
	typedef T                                           value_type;
	typedef T*                                          pointer;
	typedef T&                                          reference;
	typedef typename  std::random_access_iterator_tag   iterator_category;
};

template <class T>
class iterator_traits<const T*>
{
	public:
	typedef ptrdiff_t										difference_type;
	typedef T												value_type;
	typedef const T*										pointer;
	typedef const T&										reference;
	typedef typename  std::random_access_iterator_tag		iterator_category;
};

template<bool B, class T = void>
struct enable_if
{

};

template<class T>
struct enable_if<true, T>
{
	typedef T type;
};

template <typename T>
class is_integral
{
	public:
		static const bool	value_type = std::numeric_limits<T>::is_integer;
};

template<class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                            	InputIt2 first2, InputIt2 last2)
{
    for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
    {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
 
    return (first1 == last1) && (first2 != last2);
}

template<class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
    							InputIt2 first2, InputIt2 last2, Compare comp)
{
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
    {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
 
    return (first1 == last1) && (first2 != last2);
}

template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator {
	typedef T         value_type;
	typedef Distance  difference_type;
	typedef Pointer   pointer;
	typedef Reference reference;
	typedef Category  iterator_category;
};

template< class T1, class T2>
class pair
{
	public:
	typedef T1 first_type;
	typedef T2 second_type;
	T1 first;
	T2 second;
	
	pair():first(),second() {};
	
	template<class U, class V>
	pair (const pair<U,V> &pr) : first(pr.first), second(pr.second) {}

	pair (const first_type &a, const second_type &b) : first(a), second(b) {};

	// pair & operator=(const pair & pr)
	// {
	// 	this->first = pr.first;
	// 	this->second = pr.second;
	// 	return (*this);
	// }
};

template <class A, class B>
bool operator==( const pair<A, B>& lhs, const pair<A, B>& rhs )
{
	return ((lhs.first == rhs.first) && (lhs.second == rhs.second));
}

template <class A, class B>
bool operator!=( const pair<A, B>& lhs, const pair<A, B>& rhs )
{
	return (lhs.first != rhs.first || lhs.second != rhs.second);
	// return (!(lhs == rhs));
}

template <class A, class B>
bool operator<( const pair<A, B>& lhs, const pair<A, B>& rhs )
{
	return (lhs.first < rhs.first || 
		(lhs.second < rhs.second));
}

template <class A, class B>
bool operator<=(const pair<A, B> & lhs, const pair<A, B> & rhs)
{
	return (!(rhs < lhs));
}

template <class A, class B>
bool operator>(const pair<A, B> & lhs, const pair<A, B> & rhs)
{
	return (rhs < lhs);
}

template <class A, class B>
bool operator>=(const pair<A, B> & lhs, const pair<A, B> & rhs)
{
	return (!(lhs < rhs));
}

template <class A, class B>
ft::pair<A, B> make_pair( A t, B u )
{
	return (pair<A, B>(t, u));
}

template <typename T>
class VecIterator : public std::iterator<std::random_access_iterator_tag, T>
{
	public:
		typedef typename std::random_access_iterator_tag iterator_category;
	    typedef T value_type;
	    typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T& const_reference;

	    // constructor
		pointer base() const
		{
			return _ptr;
		}

	    VecIterator() : _ptr(nullptr) {}
		VecIterator(const VecIterator<typename std::remove_const<value_type>::type> &other) : _ptr(other._ptr) {}
	    VecIterator(pointer p) : _ptr(p) {}



        VecIterator<value_type>	&operator=(VecIterator<typename std::remove_const<value_type>::type> const &other)
		{
			this->_ptr = other._ptr	;
			return (*this);
		}

		~VecIterator () {}
	    // increment/decrement operators
	    VecIterator& operator++() { ++_ptr; return *this; }
	    VecIterator& operator--() { --_ptr; return *this; }
	    VecIterator operator++(int) { VecIterator tmp(*this); operator++(); return tmp; }
	    VecIterator operator--(int) { VecIterator tmp(*this); operator--(); return tmp; }

	    // arithmetic operators
	    VecIterator operator+(difference_type n) const { return VecIterator(_ptr + n); }
	    VecIterator operator-(difference_type n) const { return VecIterator(_ptr - n); }
	    difference_type operator-(const VecIterator& rhs) const { return _ptr - rhs._ptr; }
	    VecIterator& operator+=(difference_type n) { _ptr += n; return *this; }
	    VecIterator& operator-=(difference_type n) { _ptr -= n; return *this; }

	    // dereference operator
	    T& operator*() const { return *_ptr; }
	    T* operator->() const { return _ptr; }
	    T& operator[](difference_type n) const { return _ptr[n]; }

	    // comparison operators
	    // bool operator==(const VecIterator& rhs) const { return _ptr == rhs._ptr; }
	    // bool operator!=(const VecIterator& rhs) const { return _ptr != rhs._ptr; }
	    // bool operator<(const VecIterator& rhs) const { return _ptr < rhs._ptr; }
	    // bool operator>(const VecIterator& rhs) const { return _ptr > rhs._ptr; }
	    // bool operator<=(const VecIterator& rhs) const { return _ptr <= rhs._ptr; }
		// bool operator>=(const VecIterator& rhs) const { return _ptr >= rhs._ptr; }

		T* _ptr;
};

// template <class T>
// VecIterator<T> operator+(typename VecIterator<T>::difference_type n, const VecIterator<T> &other)
// {
// 	return (other + n);
// }

// template <class T>
// VecIterator<T> operator-(typename VecIterator<T>::difference_type n, const VecIterator<T> &other)
// {
// 	return (other - n);
// }

template <class T>
VecIterator<T> operator+(typename VecIterator<T>::difference_type n,
								const VecIterator<T> &other)
{
	return (other + n);
}

template <class T, class U>
typename VecIterator<T>::difference_type
	operator-(const VecIterator<U> &lhs,
			const VecIterator<T> &rhs)
{
	return (lhs.base() - rhs.base());
}

template <class T, class U>
bool	operator==(const VecIterator<T> & lhs, const VecIterator<U> & rhs)
{
	return (lhs._ptr == rhs._ptr);
}

template <class T, class U>
bool	operator!=(const VecIterator<T> & lhs, const VecIterator<U> & rhs)
{
	return (lhs._ptr != rhs._ptr);
}

template <class T, class U>
bool	operator<(const VecIterator<T> & lhs, const VecIterator<U> & rhs)
{
	return (lhs._ptr < rhs._ptr);
}

template <class T, class U>
bool	operator<=(const VecIterator<T> & lhs, const VecIterator<U> & rhs)
{
	return (lhs._ptr <= rhs._ptr);
}

template <class T, class U>
bool	operator>(const VecIterator<T> & lhs, const VecIterator<U> & rhs)
{
	return (lhs._ptr > rhs._ptr);
}

template <class T, class U>
bool	operator>=(const VecIterator<T> & lhs, const VecIterator<U> & rhs)
{
	return (lhs._ptr >= rhs._ptr);
}

template <typename T> //this T is an iterator for const_reverse_iterator version? then it is T<const int>
class VecRevIterator : public std::iterator<std::random_access_iterator_tag, T>
{
	public:
		typedef T iterator_type;
		typedef typename ft::iterator_traits<T>::iterator_category iterator_category;
	    typedef typename ft::iterator_traits<T>::value_type value_type;
	    typedef typename ft::iterator_traits<T>::difference_type difference_type;
	    typedef typename ft::iterator_traits<T>::pointer pointer;
	    typedef typename ft::iterator_traits<T>::reference reference;

	    // constructor
		iterator_type _ptr;

		iterator_type base(void) const
		{
			return _ptr;
		}

	    VecRevIterator() : _ptr(nullptr) {}

		explicit VecRevIterator( iterator_type x) : _ptr(x) {
		}
		
		template<class U>
		VecRevIterator(const VecRevIterator<U> &other) : _ptr(other.base()) {
		}

		VecRevIterator	&operator=(const VecRevIterator &other)
		{
			_ptr = other._ptr;
			return (*this);
		}// it cannot match a constructor , that;s the issue VecRevIterator<VecIterator<const int> > || VecRevIterator<VecIterator<int> >

		~VecRevIterator() {}
	    // increment/decrement operators
	    VecRevIterator& operator++() { --_ptr; return *this; }
	    VecRevIterator& operator--() { ++_ptr; return *this; }
	    VecRevIterator operator++(int) { VecRevIterator tmp(*this); --_ptr; return tmp; }
	    VecRevIterator operator--(int) { VecRevIterator tmp(*this); ++_ptr; return tmp; }

	    // arithmetic operators
	    VecRevIterator operator+(difference_type n) const { return VecRevIterator(_ptr - n); }
	    // VecRevIterator operator+(int n) const
		// {
		// 	VecRevIterator result = *this;
        // 	result += n;
        // 	return result;
		// }
		// i think need a way for this to include
	    // VecRevIterator operator-(const VecRevIterator n) const { return VecRevIterator(_ptr + n); }
		// 
	    VecRevIterator operator-(difference_type n) const { return VecRevIterator(_ptr + n); } // difference_type is not rev vector, hence - not recoginx
 
		// template<class U>
	    // VecRevIterator<U> operator-(const VecRevIterator<U> n) { return VecRevIterator(_ptr - n); }

	    difference_type operator-(const VecRevIterator& rhs) const { return (_ptr + rhs._ptr); }
	    VecRevIterator& operator+=(difference_type n) {_ptr -= n; return *this; }
	    VecRevIterator& operator-=(difference_type n) { _ptr += n; return *this; }

	    // dereference operator
	    reference operator*() const
		{
			iterator_type temp = _ptr;

			return *(--temp);
		}

		// if this is const_reverse_iterator, the pointer should be const_pointer, not really right
		// since the underlying template is another iterator
		// this pointer value return can be change right?
	    pointer operator->() const
		{
			return &(operator*());
		}

	    reference operator[](difference_type n) const
		{
			iterator_type temp = _ptr;

			return *(--temp - n);
		}

	    // comparison operators
	    // bool operator==(const VecRevIterator& rhs) const { return _ptr == rhs._ptr; }
	    // bool operator!=(const VecRevIterator& rhs) const { return _ptr != rhs._ptr; }
	    // bool operator<(const VecRevIterator& rhs) const { return _ptr < rhs._ptr; }
	    // bool operator>(const VecRevIterator& rhs) const { return _ptr > rhs._ptr; }
	    // bool operator<=(const VecRevIterator& rhs) const { return _ptr <= rhs._ptr; }
		// bool operator>=(const VecRevIterator& rhs) const { return _ptr >= rhs._ptr; }

};

template <class Iter1, class Iter2>
bool operator!=(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (lhs.base() != rhs.base());
}

template <class Iter1, class Iter2>
bool operator==(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (lhs.base() == rhs.base());
}

template <class Iter1, class Iter2>
bool operator<(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (lhs.base() > rhs.base());
}

template <class Iter1, class Iter2>
bool operator<=(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (lhs.base() >= rhs.base());
}

template <class Iter1, class Iter2>
bool operator>(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (lhs.base() < rhs.base());
}

template <class Iter1, class Iter2>
bool operator>=(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (lhs.base() <= rhs.base());
}
// template <class T>
// VecRevIterator<T> operator+(typename VecRevIterator<T>::difference_type n, const VecRevIterator<T> &other)
// {
// 	return (other + n);
// }
// // i think it should be this overload right
// template <class T>
// VecRevIterator<T> operator-(typename VecRevIterator<T>::difference_type n, const VecRevIterator<T> &other)
// {
// 	return (other - n);
// }

template <class Iter>
	VecRevIterator<Iter>
		operator+(typename VecRevIterator<Iter>::difference_type n,
			const VecRevIterator<Iter> &other)
	{
		return (other + n);
	}

template <class Iter1, class Iter2> 
typename VecRevIterator<Iter1>::difference_type
	operator-(const VecRevIterator<Iter1> &lhs, const VecRevIterator<Iter2> &rhs)
{
	return (rhs.base() - lhs.base());
}

template <class T>
struct Node {
	typedef Node<T> *node_pointer;
	
	Node() : _data(nullptr),
			_parent(nullptr),
			_left(nullptr),
			_right(nullptr),
			_is_sentinal(false),
			_color(0),
			_count(0) {};

	Node(T* data, node_pointer parent = nullptr, node_pointer left = nullptr, node_pointer right = nullptr, bool is_sentinal = false, int color = 0, size_t count = 0) : 
		_data(data),
		_parent(parent),
		_left(left),
		_right(right),
		_color(color),
		_is_sentinal(is_sentinal),
		_count(count) {};

	Node(const Node &other) : 
		_data(other._data),
		_parent(other._parent),
		_left(other._left),
		_right(other._right),
		_color(other._color),
		_is_sentinal(other._is_sentinal),
		_count(other._count) {};

	~Node() {};

	public:
		T* _data; // holds the pair, access key with .first and value with .second
		node_pointer _parent; // pointer to the parent
		node_pointer _left; // pointer to left child
		node_pointer _right; // pointer to right child
		int _color; // 1 -> Red, 0 -> Black
		bool _is_sentinal; // 1 -> Red, 0 -> Black
		size_t _count;
};

template <class T>
class   map_iterator : public ft::iterator<std::bidirectional_iterator_tag, T>
{
	public:
		typedef T iterator_type;
		typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::iterator_category iterator_category;
	    typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::value_type value_type;
	    typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
	    typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::pointer pointer;
	    typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::reference reference;
		typedef Node<typename std::remove_const<value_type>::type>*	node_pointer;

		map_iterator() : _ptr(nullptr) {};
		map_iterator(const map_iterator<typename std::remove_const<value_type>::type> &copy) : _ptr(copy._ptr) {}
		map_iterator(node_pointer node) : _ptr(node) {}
		
		map_iterator	&operator=(const map_iterator &other)
		{
			this->_ptr = other._ptr;
			return (*this);
		}

		~map_iterator() {};
		
		map_iterator &operator++()
		{
			// if node has a right child
			if (_ptr == max()) {
				_ptr = _ptr->_right;
				return *this;
			}
			if (!_ptr->_right->_is_sentinal)
			{
				_ptr = _ptr->_right;

				while (!_ptr->_left->_is_sentinal) {
					_ptr = _ptr->_left;
				}

			}
			else
			{
				// set parent to one node above child
				node_pointer _parent = _ptr->_parent;

				// while a node is the _right child of a _parent
				while (!_parent->_is_sentinal && _ptr == _parent->_right)
				{
					// set _parent to node
					_ptr = _parent;
					_parent = _parent->_parent;
				}
				// set node to _parent
				_ptr = _parent;
			}
			return *this;
		}
	
	    map_iterator operator++(int)
		{
			map_iterator<value_type> tmp(*this);
			// if node has a _right child
			if (_ptr == max())
			{
				_ptr = _ptr->_right;
				return tmp;
			}
			if (!_ptr->_right->_is_sentinal)
			{
				_ptr = _ptr->_right;
				while (!_ptr->_left->_is_sentinal) {
					_ptr = _ptr->_left;
				}
			}
			else
			{
				// set _parent to one node above child
				node_pointer _parent = _ptr->_parent;


				// while a node is the _right child of a _parent
				while (!_parent->_is_sentinal && _ptr == _parent->_right)
				{
					// set _parent to node
					_ptr = _parent;
					_parent = _parent->_parent;
				}
				// set node to _parent
				_ptr = _parent;
			}
			return tmp;
		}

	    map_iterator& operator--()
		{
			// if node has a _left child
			if (_ptr->_is_sentinal)
			{
				_ptr = max();
				return *this;
			}
			if (!_ptr->_left->_is_sentinal)
			{
				_ptr = _ptr->_left;
				while (!_ptr->_right->_is_sentinal)
				{
					_ptr = _ptr->_right;
				}
			}
			else
			{
				// set parent to one node above child
				node_pointer _parent = _ptr->_parent;

				// while a node is the _left child of a _parent
				while (!_parent->_is_sentinal && _ptr == _parent->_left)
				{
					// set _parent to node
					_ptr = _parent;
					_parent = _parent->_parent;
				}
				// set node to _parent
				_ptr = _parent;
			}
			return *this;
  		}

	    map_iterator operator--(int)
		{
			map_iterator<value_type> tmp(*this);
			// if node has a _left child
			if (_ptr->_is_sentinal)
			{
				
				_ptr = max();
				return tmp;
			}
			if (!_ptr->_left->_is_sentinal)
			{
				_ptr = _ptr->_left;
				while (!_ptr->_right->_is_sentinal) {
					_ptr = _ptr->_right;
				}
			}
			else
			{
				// set parent to one node above child
				node_pointer _parent = _ptr->_parent;

				// while a node is the _left child of a _parent
				while (!_parent->_is_sentinal && _ptr == _parent->_left)
				{
					// set _parent to node
					_ptr = _parent;
					_parent = _parent->_parent;
				}
				// set node to _parent
				_ptr = _parent;
			}
			return tmp;
		}

		bool operator==(const map_iterator& rhs) const { return _ptr == rhs._ptr; }
	    bool operator!=(const map_iterator& rhs) const { return _ptr != rhs._ptr; }
	    bool operator<(const map_iterator& rhs) const { return _ptr < rhs._ptr; }
	    bool operator>(const map_iterator& rhs) const { return _ptr > rhs._ptr; }
	    bool operator<=(const map_iterator& rhs) const { return _ptr <= rhs._ptr; }
		bool operator>=(const map_iterator& rhs) const { return _ptr >= rhs._ptr; }
		reference	operator*(void) const { return (*this->_ptr->_data); }
		pointer		operator->(void) const { return (this->_ptr->_data); }
		node_pointer _ptr;
	    // arithmetic operators
		private:

			node_pointer max()
			{
				node_pointer temp = _ptr;
				
				if (temp->_is_sentinal)
				{
					if (!temp->_right->_is_sentinal)
						temp = temp->_right;
					else if (!temp->_left->_is_sentinal)
						temp = temp->_left;
				}

				while (temp->_parent->_is_sentinal == false)
					temp = temp->_parent;
				while (temp->_right->_is_sentinal == false)
					temp = temp->_right;
				return (temp);
			}
	
			node_pointer min()
			{
				node_pointer temp = _ptr;
	
				if (temp->_is_sentinal)
				{
					if (!temp->_right->_is_sentinal)
						temp = temp->_right;
					else if (!temp->_left->_is_sentinal)
						temp = temp->_left;
				}

				while (temp->_parent->_is_sentinal == false)
					temp = temp->_parent;
				while (temp->_left->_is_sentinal == false)
					temp = temp->_left;
				return (temp);
			}

};

}

#endif