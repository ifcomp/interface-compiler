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
	using Index = typename Container::key_type;
	using Value = typename Container::mapped_type;
	using Iterator = typename Container::const_iterator;
};

template<typename ValueT>
struct IndexTraits<std::vector<ValueT>>
{
	using Container = std::vector<ValueT>;

	using Position = typename Container::size_type;
	using Index = typename Container::size_type;
	using Value = typename Container::value_type;
	using Iterator = typename Container::const_iterator;
};

template<typename ValueT>
struct IndexTraits<std::set<ValueT>>
{
	using Container = std::set<ValueT>;

	using Position = typename Container::size_type;
	using Index = typename Container::key_type;
	using Value = typename Container::value_type;
	using Iterator = typename Container::const_iterator;
};


// ********************************************



template<typename ContainerT>
class IndexIterator;


template<typename ContainerT>
class Index
{
	friend IndexIterator<ContainerT>;

public:
	using Traits = IndexTraits<ContainerT>;

	typename Traits::Position position() const;
	typename Traits::Index key() const;
	const typename Traits::Value& value() const;
	typename Traits::Iterator base() const;

	bool first() const;
	bool last() const;

	~Index();

private:
	Index(const ContainerT* container, typename Traits::Position position, typename Traits::Iterator iterator);

	const ContainerT* _container;
	typename Traits::Position _position;
	typename Traits::Iterator _iterator;
};

template<typename ContainerT>
Index<ContainerT>::Index(const ContainerT* container, typename Traits::Position position, typename Traits::Iterator iterator)
	: _container(container), _position(position), _iterator(iterator)
{
}

template<typename ContainerT>
Index<ContainerT>::~Index()
{
}


template<typename ContainerT>
typename IndexTraits<ContainerT>::Position
Index<ContainerT>::position() const
{
	return _position;
}


template<typename ContainerT>
struct IndexKeyLookup;

template<typename KeyT, typename ValueT>
struct IndexKeyLookup < std::map<KeyT, ValueT> >
{
	typename IndexTraits<std::map<KeyT, ValueT>>::Iterator iterator;
	typename IndexTraits<std::map<KeyT, ValueT>>::Position position;
	typename IndexTraits<std::map<KeyT, ValueT>>::Index key() const { return iterator->first; }
};

template<typename ValueT>
struct IndexKeyLookup < std::vector<ValueT> >
{
	typename IndexTraits<std::vector<ValueT>>::Iterator iterator;
	typename IndexTraits<std::vector<ValueT>>::Position position;
	typename IndexTraits<std::vector<ValueT>>::Index key() const { return position; }
};

template<typename ValueT>
struct IndexKeyLookup < std::set<ValueT> >
{
	typename IndexTraits<std::set<ValueT>>::Iterator iterator;
	typename IndexTraits<std::set<ValueT>>::Position position;
	typename IndexTraits<std::set<ValueT>>::Index key() const { return *iterator; }
};

template<typename ContainerT>
typename IndexTraits<ContainerT>::Index
Index<ContainerT>::key() const
{
	return IndexKeyLookup<ContainerT>{_iterator, _position}.key();
}


template<typename ContainerT>
typename IndexTraits<ContainerT>::Iterator
Index<ContainerT>::base() const
{
	return _iterator;
}



template<typename ContainerT>
struct IndexValueLookup
{
	typename IndexTraits<ContainerT>::Iterator iterator;
	const typename IndexTraits<ContainerT>::Value& value() const { return *iterator; }
};

template<typename KeyT, typename ValueT>
struct IndexValueLookup < std::map<KeyT, ValueT> >
{
	typename IndexTraits<std::map<KeyT, ValueT>>::Iterator iterator;
	const typename IndexTraits<std::map<KeyT, ValueT>>::Value& value() const { return iterator->second; }
};

template<typename ContainerT>
const typename IndexTraits<ContainerT>::Value&
Index<ContainerT>::value() const
{
	return IndexValueLookup<ContainerT>{_iterator}.value();
}


template<typename ContainerT>
bool Index<ContainerT>::first() const
{
	if(!_container)
		return false;

	return _iterator == _container->begin();
}

template<typename ContainerT>
bool Index<ContainerT>::last() const
{
	if(!_container)
		return false;

	return _iterator == (--_container->end());
}




// ********************************************


template<typename ContainerT>
class IndexIterator
	: public boost::iterator_facade < IndexIterator<ContainerT>, Index<ContainerT>, boost::forward_traversal_tag >
{
public:
	using Traits = IndexTraits<ContainerT>;

	IndexIterator();
	IndexIterator(const IndexIterator& other);
	IndexIterator(IndexIterator&& other);
	IndexIterator(const ContainerT* container, typename Traits::Position position, typename Traits::Iterator iterator);

	~IndexIterator();

private:
	friend class boost::iterator_core_access;

	void increment();
	bool equal(IndexIterator const& other) const;
	Index<ContainerT>& dereference() const;

	std::unique_ptr<Index<ContainerT>> _index;
};


template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator()
	: _index(new Index<ContainerT>(nullptr, 0, typename Traits::Iterator()))
{
}

template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator(const IndexIterator& other)
	: _index(new Index<ContainerT>(other._index->_container, other._index->_position, other._index->_iterator))
{
}

template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator(IndexIterator&& other)
	: _index(std::move(other._index))
{
}

template<typename ContainerT>
IndexIterator<ContainerT>::IndexIterator(const ContainerT* container, typename Traits::Position position, typename Traits::Iterator iterator)
	: _index(new Index<ContainerT>(container, position, iterator))
{
}

template<typename ContainerT>
IndexIterator<ContainerT>::~IndexIterator()
{
}

template<typename ContainerT>
void IndexIterator<ContainerT>::increment()
{
	++_index->_position;
	++_index->_iterator;
}

template<typename ContainerT>
bool IndexIterator<ContainerT>::equal(IndexIterator const& other) const
{
	if(_index->_container != other._index->_container)
		throw std::invalid_argument("container mismatch");

    return _index->_iterator == other._index->_iterator;
}


template<typename ContainerT>
Index<ContainerT>&
IndexIterator<ContainerT>::dereference() const
{
	return *_index;
}



// ********************************************

template<typename ContainerT>
class IndexList
{
public:
	IndexList(const ContainerT& container);
	IndexList(ContainerT&& container);
	IndexList(IndexList&& other);

	~IndexList();

	IndexIterator<ContainerT> begin() const;
	IndexIterator<ContainerT> end() const;

private:
	const ContainerT* _container;
	bool _ownership;
};

template<typename ContainerT>
IndexList<ContainerT>::IndexList(const ContainerT& container)
	: _container(&container), _ownership(false)
{
}

template<typename ContainerT>
IndexList<ContainerT>::IndexList(ContainerT&& container)
	: _container(new ContainerT(container)), _ownership(true)
{
}

template<typename ContainerT>
IndexList<ContainerT>::IndexList(IndexList&& other)
	: _container(std::move(other._container)), _ownership(std::move(other._ownership))
{
}

template<typename ContainerT>
IndexList<ContainerT>::~IndexList()
{
	if(_ownership)
	{
		delete _container;
		_container = nullptr;
		_ownership = false;
	}
}

template<typename ContainerT>
IndexIterator<ContainerT>
IndexList<ContainerT>::begin() const
{
	return IndexIterator<ContainerT>(_container, 0, _container->begin());
}

template<typename ContainerT>
IndexIterator<ContainerT>
IndexList<ContainerT>::end() const
{
	return IndexIterator<ContainerT>(_container, _container->size(), _container->end());
}


template<typename ContainerT>
IndexList<ContainerT> indices(const ContainerT& container)
{
	return IndexList<ContainerT>(container);
}

template<typename ContainerT>
IndexList<ContainerT> indices(ContainerT&& container)
{
	return IndexList<ContainerT>(std::move(container));
}

} } } // namespace: Everbase::InterfaceCompiler::IndexList
