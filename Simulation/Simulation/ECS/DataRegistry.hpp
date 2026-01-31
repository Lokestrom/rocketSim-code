#pragma once
#include "helpers/ID.hpp"

#include <unordered_map>
#include <typeindex>
#include <concepts>
#include <type_traits>
#include <vector>

#include "database/Vector.hpp"

template<typename T>
concept DataType =
	requires { typename T::value_type; };

template<DataType TypeName>
class DataArray {
public:
	using T = TypeName::value_type;
	DataArray()
		: _data()
	{}

	size_t add(ID::UUID id, const T& data);
	size_t add(ID::UUID id, T&& data);
	template<typename... Args>
	size_t emplace(ID::UUID id, Args&&... args);
	size_t remove(ID::UUID id) noexcept;

	DataArray(const DataArray&) = delete;
	DataArray& operator=(const DataArray&) = delete;

	DataArray(DataArray&&) noexcept = default;
	DataArray& operator=(DataArray&&) noexcept = default;

	std::vector<T>& getData() noexcept { return _data; }
	const std::vector<ID::UUID>& getIDs() const noexcept { return _ids; }


    bool contains(ID::UUID id) const noexcept { return std::ranges::binary_search(_ids, id); };

	/*
		Invalidated on moving of the DataArray
		not on add/remove/emplace
		if add/remove/emplace returns a index less than the current iterator position
		it will be invalidated but in a predictable way depending on the operation
	*/
	class Iterator {
	public:
		Iterator(size_t index, DataArray<TypeName>& dataArray)
			: _index(index), _dataArray(dataArray)
		{}
		bool operator==(const Iterator& other) const noexcept {
			return _index == other._index;
		}
		bool operator!=(const Iterator& other) const noexcept {
			return _index != other._index;
		}
		Iterator& operator++() noexcept {
			++_index;
			return *this;
		}
		std::pair<ID::UUID, T&> operator*() noexcept {
			return { _dataArray._ids[_index], _dataArray._data[_index] };
		}

		ID::UUID ID() const noexcept {
			return _dataArray._ids[_index];
		}
		T& data() noexcept {
			return _dataArray._data[_index];
		}
		void setPosition(size_t index) noexcept {
			_index = index;
		}

	private:
		size_t _index;
		DataArray<TypeName>& _dataArray;
	};

	Iterator begin() noexcept {
        return Iterator{ 0, *this };
	}
	Iterator end() noexcept {
        return Iterator{ _data.size(), *this };
	}

	T& operator[] (ID::UUID id) noexcept {
		assert(_idToIndexMap.contains(id) && "Cant find the id in the array");
		return _data[_idToIndexMap[id]];
	}

private:
	std::vector<ID::UUID> _ids;
	std::vector<T> _data;
	std::unordered_map<ID::UUID, size_t> _idToIndexMap;

	//IDEA: use a handle-based system instead of map
	//potential benefit: faster since no hashing, but direct indexing
	//but has to be benchmarked since it is more memory consuming and complex
	//note: it may have an impact on the insert/remove/emplace performance
	//but should be worth it for the operator[] performance
	//maybe allow user to choose between the two systems at compile time
	//if benchmarks show it is worth it

private:
	size_t insetPoint(ID::UUID id) const noexcept;
    void updateIDToIndexMap() noexcept {
        _idToIndexMap.clear();
        for (size_t i = 0; i < _ids.size(); ++i) {
            _idToIndexMap[_ids[i]] = i;
        }
	}
};

struct DataRegistry {
    using ComponentID = size_t;

    template <DataType T>
    static ComponentID getID() {
        static const ComponentID id = [] {
            const ComponentID newID = componentCount++;

            if (removers.size() <= newID)
                removers.resize(newID + 1);

            removers[newID] = [](ID::UUID id) {
                get<T>().remove(id);
                };

            return newID;
            }();
        return id;
    }

	template <DataType T>
	static DataArray<T>& get() {
		static DataArray<T> instance;
		return instance;
	}

    static void removeComponent(ID::UUID entityID, ComponentID componentID) {
        removers[componentID](entityID);
    }

private:

    inline static size_t componentCount = 0;
    using removeFunc = void(*)(ID::UUID);
    inline static std::vector<removeFunc> removers;
};

template<DataType... Components>
class DataView {
public:
    static constexpr size_t Count = sizeof...(Components);

    DataView()
        : _arrays(DataRegistry::get<Components>()...)
    {
    }

    class Iterator {
    public:
        Iterator(std::tuple<DataArray<Components>&...>& arrays, bool end = false)
            : _arrays(arrays),
			_ids(createIDArray(arrays, std::index_sequence_for<Components...>{}))
        {
            if (end) {
                _ended = true;
            }
            else {
                advance_to_match();
            }
        }

        bool operator==(const Iterator& other) const noexcept {
            return _ended == other._ended;
        }

        bool operator!=(const Iterator& other) const noexcept {
            return !(*this == other);
        }

        Iterator& operator++() {
            ++_indices[min_id_index()];
            advance_to_match();
            return *this;
        }

        auto operator*() {
            return deref(std::index_sequence_for<Components...>{});
        }

    private:
        std::tuple<DataArray<Components>&...>& _arrays;
		std::array<const std::vector<ID::UUID>*, Count> _ids;
        std::array<size_t, Count> _indices{};
        bool _ended{ false };

    private:
        void advance_to_match() {
            while (true) {
                if (any_exhausted()) {
                    _ended = true;
                    return;
                }

                if (all_equal()) {
                    return;
                }

                ++_indices[min_id_index()];
            }
        }

        bool any_exhausted() const {
            return exhausted_impl(std::index_sequence_for<Components...>{});
        }

        template<size_t... I>
        bool exhausted_impl(std::index_sequence<I...>) const {
            return ((std::get<I>(_indices) >=
                std::get<I>(_arrays).getIDs().size()) || ...);
        }

        ID::UUID getCurrentID(size_t arrayIndex) const {
            return (*_ids[arrayIndex])[_indices[arrayIndex]];
		}

        bool all_equal() {
            for (size_t i = 1; i < Count; ++i)
				if (getCurrentID(i) != getCurrentID(0))
                    return false;
            return true;
        }

        size_t min_id_index() {
            size_t min = 0;
            for (size_t i = 1; i < Count; ++i)
                if (getCurrentID(i) < getCurrentID(min))
                    min = i;
            return min;
        }

        template<size_t... I>
        auto deref(std::index_sequence<I...>) {
            ID::UUID id =
                std::get<0>(_arrays).getIDs()[_indices[0]];

            return std::tuple<
                ID::UUID,
                typename Components::value_type&...
            >{
                id,
                    std::get<I>(_arrays).getData()[_indices[I]]...
            };
        }

        template<size_t... I>
        auto createIDArray(std::tuple<DataArray<Components>&...>& arrays,
            std::index_sequence<I...>) {
            return std::array<const std::vector<ID::UUID>*, Count>{
                &std::get<I>(arrays).getIDs()...
            };
        }

    };

    Iterator begin() {
        return Iterator{ _arrays };
    }

    Iterator end() {
        return Iterator{ _arrays, true };
    }

private:
    std::tuple<DataArray<Components>&...> _arrays;
};

template<DataType T>
inline size_t DataArray<T>::add(ID::UUID id, const T& data)
{
	size_t pos = insetPoint(id);
	_ids.insert(_ids.begin() + pos, id);
	_data.insert(_data.begin() + pos, data);
	updateIDToIndexMap();
	return pos;
}

template<DataType T>
inline size_t DataArray<T>::add(ID::UUID id, T&& data)
{
	size_t pos = insetPoint(id);
	_ids.insert(_ids.begin() + pos, id);
	_data.insert(_data.begin() + pos, std::move(data));
    updateIDToIndexMap();
	return pos;
}

template<DataType T>
inline size_t DataArray<T>::remove(ID::UUID id) noexcept
{
	assert(std::ranges::binary_search(_ids, id) && "Cant find the id in the array");
	auto index = std::ranges::lower_bound(_ids, id) - _ids.begin();
	_ids.erase(_ids.begin() + index);
	_data.erase(_data.begin() + index);
    updateIDToIndexMap();
	return index;
}

template<DataType T>
template<typename ...Args>
inline size_t DataArray<T>::emplace(ID::UUID id, Args&& ...args)
{
	size_t pos = insetPoint(id);
	_ids.insert(_ids.begin() + pos, id);
	//may not work
	_data.insert(_data.begin() + pos, args...);
    updateIDToIndexMap();
	return pos;
}

template<DataType T>
inline size_t DataArray<T>::insetPoint(ID::UUID id) const noexcept
{
	assert(!std::ranges::binary_search(_ids, id) && "ID already in the array, cant insert the same id twice");
	return std::ranges::upper_bound(_ids, id) - _ids.begin();
}
