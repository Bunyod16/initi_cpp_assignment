#ifndef FT_MAP_HPP
# define FT_MAP_HPP

#include <iostream>
#include "iterator.hpp"
#include "rbt.hpp"

namespace ft
{

template<class Key, class T, class Compare = std::less<Key>,class Allocator = std::allocator<ft::pair<const Key, T> > >
class map
{
	public:
		typedef Key key_type;
		typedef T mapped_type;
		typedef ft::pair<const key_type, mapped_type> value_type;
		typedef Compare key_compare;
		typedef Allocator allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef ft::map_iterator<value_type> iterator;
		typedef ft::map_iterator<const value_type> const_iterator;
		typedef ft::VecRevIterator<iterator> reverse_iterator;
		typedef ft::VecRevIterator<const_iterator> const_reverse_iterator;
		typedef size_t size_type;

		class value_compare : public std::binary_function<value_type, value_type, bool>
		{
			protected:
				Compare comp;

			public:
				value_compare(Compare c) : comp(c) {};

				bool operator()( const value_type& lhs, const value_type& rhs ) const
				{
					return (comp(lhs.first, rhs.first));
				}

		};

		typedef ft::RBTree<ft::pair<const key_type, mapped_type>, value_compare, allocator_type> tree_type;
		typedef typename tree_type::node_pointer node_pointer;
		typedef typename tree_type::iterator	tree_iterator;
	
		private:
			tree_type		_rbt;
			allocator_type	_alloc;
			key_compare		_key_compare;

		public:
			explicit map(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :
					_rbt(comp, alloc),
					_alloc(alloc),
					_key_compare(comp) {}

			template< class InputIt >
			map( InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) :
					_rbt(comp, alloc),
					_alloc(alloc),
					_key_compare(comp)
			{
				while (first != last)
				{
					if (_rbt.find_val(_rbt.get_root(), *first) == _rbt.get_sentinal())
						_rbt.insert(*first);
					first++;
				}
			}

			map( const map& other ) : _rbt(other._rbt),
                                        _alloc(other._alloc),
                                        _key_compare(other._key_compare) {}

			~map() {};

			map& operator=( const map& other )
            {
                _alloc = other._alloc;
                _key_compare = other._key_compare;
                map &non_const_other = const_cast<map&>(other);
                clear();
                for (map::iterator it = non_const_other.begin(); it != non_const_other.end(); it++)
                {
                    insert(*it);
                }
                return (*this);
            } 

			allocator_type get_allocator() const
			{
				return _alloc;
			}

			// Element access
			T& at( const Key& key )
			{
				node_pointer node;

				node = _rbt.find_key(key);
				if (node->_data->first != key)
					throw std::out_of_range("No such key exists");
				return (node->_data->second);
			};

			const T& at( const Key& key ) const
			{
				node_pointer node;

				node = _rbt.find_key(key);
				if (node->_data->first != key)
					throw std::out_of_range("No such key exists");
				return (node->_data->second);
			};

			T& operator[]( const Key& key )
			{
				node_pointer node;

				node = _rbt.find_key(key);
				if (node->_data->first == key) 
					return (node->_data->second);
				else
					return ((*insert(ft::make_pair(key, mapped_type())).first).second);
			};
			
			// Iterators
			iterator begin()
			{
				return (iterator(_rbt.begin()));
			};

			const_iterator begin() const
			{
				return (const_iterator(_rbt.begin()));
			};
			
			iterator end()
			{
				return (iterator(_rbt.get_sentinal()));
			};

			const_iterator end() const
			{
				return (const_iterator(_rbt.get_sentinal()));
			};

			//Capacity
			bool empty() const
			{
				return _rbt.size() < 1 ? true : false;
			};

			size_type size() const
			{
				return (_rbt.size());
			};

			size_type max_size() const
			{
				if (std::is_same<T, char>::value || std::is_same<T, unsigned char>::value) { return _alloc.max_size() / 2; }

				return (_alloc.max_size());
			}

			//Modifiers
			void clear() 
			{
				_rbt.clear();
			}

			ft::pair<iterator, bool> insert( const value_type& value )
            {
                if (_rbt.find_val(_rbt.get_root(), value) == _rbt.get_sentinal())
				    return (ft::make_pair(iterator(_rbt.insert(value)), true));
                else
                    return (ft::make_pair(iterator(_rbt.find_val(_rbt.get_root(), value)), false));
			};

			template< class InputIt >
			void insert( InputIt first, InputIt last, typename ft::enable_if<!std::is_integral<InputIt>::value, InputIt>::type* = nullptr )
            {
                
                // _rbt.prettyPrint();
                for (; first != last; first++)
                {
                    // std::cout << "key: " << first->first << " val: " << first->second << std::endl;
                    if (_rbt.find_val(_rbt.get_root(), *first) == _rbt.get_sentinal())
                    {
                        // std::cout << "INSERTING" << std::endl;
				        _rbt.insert(*first);
                    }

                }
            }

			iterator insert (iterator position, const value_type& val)
			{
				(void)position;
				if (_rbt.find_val(_rbt.get_root(), val) == _rbt.get_sentinal())
                {
				    _rbt.insert(val);
                }
				return (iterator(_rbt.find_val(_rbt.get_root(), val)));
			}

			void erase( iterator pos )
            {
                iterator first = begin();
                while (first != end())
                {
                    if (first == pos)
					{
                        _rbt.deleteNode(pos->first);
						break;
					}
					first++;
                }
            }

			void erase( iterator first, iterator last )
            {
                while (first != last)
                    _rbt.deleteNode((*first++).first);
            }

			size_type erase( const Key& key )
            {
                if (_rbt.find_key(key) != _rbt.get_sentinal())
                {
                    _rbt.deleteNode(key);
                    return (1);
                }
                return (0);
            }
};


}
#endif