#include <map>
#include <vector>
#include <set>
#include <memory>
#include <boost/iterator/iterator_facade.hpp>

#include <iostream>


namespace Everbase { namespace InterfaceCompiler { namespace IndexList {

// ********************************************

template<typename ContainerT>
struct IndexTraits;

template<typename KeyT, typename ValueT>
struct IndexTraits<std::map<KeyT, ValueT>>
{
	using Container = std::map<KeyT, ValueT>;

	using Position = typename Container::size_type;
	using Key = typename Container::key_type;
	using Value = typename Container::mapped_type;
	using Iterator = typename Container::const_iterator;

	static Key key(Iterator iterator, Position position) { return iterator->first; }
	static const Value& value(Iterator iterator) { return iterator->second; }
};

template<typename ValueT>
struct IndexTraits<std::vector<ValueT>>
{
	using Container = std::vector<ValueT>;

	using Position = typename Container::size_type;
	using Key = typename Container::size_type;
	using Value = typename Container::value_type;
	using Iterator = typename Container::const_iterator;

	static Key key(Iterator iterator, Position position) { return position; }
	static const Value& value(Iterator iterator) { return *iterator; }
};

template<typename ValueT>
struct IndexTraits<std::set<ValueT>>
{
	using Container = std::set<ValueT>;

	using Position = typename Container::size_type;
	using Key = typename Container::key_type;
	using Value = typename Container::value_type;
	using Iterator = typename Container::const_iterator;

	static Key key(Iterator iterator, Position position) { return *iterator; }
	static const Value& value(Iterator iterator) { return *iterator; }
};

template<typename ContainerT>
struct IndexTraits<ContainerT&> : public IndexTraits<ContainerT>
{
	using Container = typename IndexTraits<ContainerT>::Container;

	using Position = typename IndexTraits<ContainerT>::Position;
	using Key = typename IndexTraits<ContainerT>::Key;
	using Value = typename IndexTraits<ContainerT>::Value;
	using Iterator = typename IndexTraits<ContainerT>::Iterator;

	using IndexTraits<ContainerT>::key;
	using IndexTraits<ContainerT>::value;
};


// ********************************************



template<typename ContainerT>
class IndexIterator;


template<typename ContainerT>
class Index
{
public:
	using Traits = IndexTraits<ContainerT>;

	typename Traits::Position position() const;
	typename Traits::Key key() const;
	const typename Traits::Value& value() const;
	typename Traits::Iterator base() const;

	bool first() const;
	bool last() const;

	Index(const Index& other);
	Index(Index&& other);

private:
	friend IndexIterator<ContainerT>;

	Index(const ContainerT& container, typename Traits::Position position, typename Traits::Iterator iterator);

	const ContainerT& _container;
	typename Traits::Position _position;
	typename Traits::Iterator _iterator;
};

template<typename ContainerT>
Index<ContainerT>::Index(const Index& other)
	: _container(other._container)
	, _position(other._position)
	, _iterator(other._iterator)
{
}

template<typename ContainerT>
Index<ContainerT>::Index(Index&& other)
	: _container(other._container)
	, _position(std::move(other._position))
	, _iterator(std::move(other._iterator))
{
}

template<typename ContainerT>
Index<ContainerT>::Index(const ContainerT& container, typename Traits::Position position, typename Traits::Iterator iterator)
	: _container(container)
	, _position(position)
	, _iterator(iterator)
{
}

template<typename ContainerT>
typename IndexTraits<ContainerT>::Position
Index<ContainerT>::position() const
{
	return _position;
}

template<typename ContainerT>
typename IndexTraits<ContainerT>::Key
Index<ContainerT>::key() const
{
	return Traits::key(_iterator, _position);
}

template<typename ContainerT>
typename IndexTraits<ContainerT>::Iterator
Index<ContainerT>::base() const
{
	return _iterator;
}

template<typename ContainerT>
const typename IndexTraits<ContainerT>::Value&
Index<ContainerT>::value() const
{
	return Traits::value(_iterator);
}

template<typename ContainerT>
bool Index<ContainerT>::first() const
{
	return _iterator == _container.begin();
}

template<typename ContainerT>
bool Index<ContainerT>::last() const
{
	return _iterator == --(_container.end());
}




// ********************************************


template<typename ContainerT>
class IndexList;


template<typename ContainerT>
class IndexIterator
	: public boost::iterator_facade < IndexIterator<ContainerT>, const Index<ContainerT>, boost::forward_traversal_tag >
{
public:
	using Traits = IndexTraits<ContainerT>;

	IndexIterator(const IndexIterator& other);
	IndexIterator(IndexIterator&& other);

private:
	friend class boost::iterator_core_access;

	void increment();
	bool equal(IndexIterator const& other) const;
	const Index<ContainerT>& dereference() const;

private:
	friend IndexList<ContainerT>;

	IndexIterator(const ContainerT& container, typename Traits::Position position, typename Traits::Iterator iterator);

	Index<ContainerT> _index;
};

template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator(const IndexIterator& other)
	: _index(other._index)
{
}

template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator(IndexIterator&& other)
	: _index(std::move(other._index))
{
}

template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator(const ContainerT& container, typename Traits::Position position, typename Traits::Iterator iterator)
	: _index(container, position, iterator)
{
}

template<typename ContainerT>
void IndexIterator<ContainerT>::increment()
{
	++(_index._position);
	++(_index._iterator);
}

template<typename ContainerT>
bool IndexIterator<ContainerT>::equal(IndexIterator const& other) const
{
    return _index._iterator == other._index._iterator;
}

template<typename ContainerT>
const Index<ContainerT>&
IndexIterator<ContainerT>::dereference() const
{
	return _index;
}



// ********************************************

template<typename ContainerT>
class IndexList
{
public:
	IndexList(ContainerT&& container);
	IndexList(IndexList&& other);

	IndexIterator<ContainerT> begin() const;
	IndexIterator<ContainerT> end() const;

private:
	ContainerT _container;
};

template<typename ContainerT>
IndexList<ContainerT>::IndexList(ContainerT&& container)
	: _container(std::forward<ContainerT>(container))
{
}

template<typename ContainerT>
IndexList<ContainerT>::IndexList(IndexList&& other)
	: _container(other._container)
{
}

template<typename ContainerT>
IndexIterator<ContainerT>
IndexList<ContainerT>::begin() const
{
	return IndexIterator<ContainerT>(_container, 0, _container.begin());
}

template<typename ContainerT>
IndexIterator<ContainerT>
IndexList<ContainerT>::end() const
{
	return IndexIterator<ContainerT>(_container, _container.size(), _container.end());
}

template<typename ContainerT>
IndexList<ContainerT> indices(ContainerT&& container)
{
	return IndexList<ContainerT>(std::forward<ContainerT>(container));
}

} } } // namespace: Everbase::InterfaceCompiler::IndexList
