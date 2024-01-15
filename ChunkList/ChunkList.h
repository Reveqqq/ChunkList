#pragma once
#include <iterator>
#include <memory>
#include <list>
#include <algorithm>
#include <exception>

namespace fefu_laboratory_two {
	template <typename T>
	class Allocator {
	public:
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;


		constexpr Allocator() noexcept {};

		constexpr Allocator(const Allocator& other) noexcept = default;

		template <class U>
		constexpr Allocator(const Allocator<U>& other) noexcept {};

		~Allocator() = default;

		pointer allocate(size_type _Count) {
			auto ptr = static_cast<pointer>(malloc(sizeof(value_type) * _Count));
			if (ptr)
				return ptr;

			throw std::bad_alloc();
		};

		void deallocate(pointer p, const size_t _Count) noexcept {
			(void)_Count;
			free(p);
		};
	};

	template <typename ValueType>
	class IChunk {
	public:
		virtual ~IChunk() = default;
		virtual int GetChunkSize() = 0;
		virtual size_t size() const noexcept = 0;
		virtual ValueType& at(size_t pos) = 0;
		virtual ValueType& operator[](std::ptrdiff_t n) = 0;
	};

	template <typename ValueType>
	class ChunkList_iterator {
	protected:
		IChunk<ValueType>* list = nullptr; //
		ValueType* current_value; //указатель на текущий элемент из чанка
		int k = 0; //индекс элемента в чанк листе
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = ValueType;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

		virtual int GetIndex() { return k; };

		constexpr ChunkList_iterator() noexcept = default;

		ChunkList_iterator(IChunk<ValueType>* chunk, int index, ValueType* value) :
			list(chunk),
			k(index),
			current_value(value)
		{};

		ChunkList_iterator(const ChunkList_iterator& other) noexcept = default;

		ChunkList_iterator& operator=(const ChunkList_iterator&) = default;

		virtual ~ChunkList_iterator() = default;

		friend void swap(ChunkList_iterator<ValueType>& a, ChunkList_iterator<ValueType>& b) {
			std::swap(a.current_value, b.current_value);
		};

		friend bool operator==(const ChunkList_iterator<ValueType>& lhs,
			const ChunkList_iterator<ValueType>& rhs) {
			return lhs.current_value == rhs.current_value;
		};
		friend bool operator!=(const ChunkList_iterator<ValueType>& lhs,
			const ChunkList_iterator<ValueType>& rhs) {
			return !(lhs.current_value == rhs.current_value);
		};

		reference operator*() const { return *current_value; };
		pointer operator->() const { return current_value; };

		ChunkList_iterator operator++(int) {
			if (k + 1 == list->size())
				return ChunkList_iterator();
			k++;
			current_value = &list->at(k);
			return *this;
		};
		ChunkList_iterator operator--(int) {
			if (k - 1 == -1)
				throw std::exception();
			k--;
			current_value = &list->at(k);
			return *this;
		};

		ChunkList_iterator& operator++() {
			if (k + 1 == list->size()) {
				this->current_value = nullptr;
				this->list = nullptr;
				this->k = 0;
				return *this;
			}
			k++;
			current_value = &list->at(k);
			return *this;
		};

		ChunkList_iterator& operator--() {
			if (k - 1 == -1)
				throw std::exception();
			k--;
			current_value = &list->at(k);
			return *this;
		};

		ChunkList_iterator operator+(const difference_type& n) const {
			return ChunkList_iterator(list, k + n, &list->at(k + n));
		};


		ChunkList_iterator operator-(const difference_type& n) const {
			return ChunkList_iterator(list, k - n, &list->at(k - n));
		};

		ChunkList_iterator& operator+=(const difference_type& n) {
			k += n;
			current_value = &list->at(k);
			return (this);
		};

		ChunkList_iterator& operator-=(const difference_type& n) {
			k -= n;
			current_value = &list->at(k);
			return (this);
		};

		/*difference_type operator-(const ChunkList_iterator<ValueType>& rhs) {
		};*/

		reference operator[](const difference_type& n) {
			return list[n];
		};

		friend bool operator<(const ChunkList_iterator<ValueType>& lhs,
			const ChunkList_iterator<ValueType>& rhs) {
			return rhs.current_value - lhs.current_value > 0;
		};
		friend bool operator<=(const ChunkList_iterator<ValueType>& lhs,
			const ChunkList_iterator<ValueType>& rhs) {
			return !(rhs.current_value > lhs.current_value);
		};
		friend bool operator>(const ChunkList_iterator<ValueType>& lhs,
			const ChunkList_iterator<ValueType>& rhs) {
			return rhs.current_value < lhs.current_value;
		};
		friend bool operator>=(const ChunkList_iterator<ValueType>& lhs,
			const ChunkList_iterator<ValueType>& rhs) {
			return !(lhs.current_value < rhs.current_value);
		};
		// operator<=> will be handy
	};

	template <typename ValueType>
	class ChunkList_const_iterator : public ChunkList_iterator<ValueType> {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = ValueType;
		using difference_type = std::ptrdiff_t;
		using const_pointer = const ValueType*;
		using const_reference = const ValueType&;
		constexpr ChunkList_const_iterator() : ChunkList_iterator<ValueType>() {};
		ChunkList_const_iterator(IChunk<ValueType>* chunk, int index, ValueType* value) :
			ChunkList_iterator<ValueType>(chunk, index, value) {};

		ChunkList_const_iterator(const ChunkList_const_iterator& other) noexcept = default;

		ChunkList_const_iterator& operator=(const ChunkList_const_iterator&) = default;

		~ChunkList_const_iterator() override = default;

		friend void swap(ChunkList_const_iterator<ValueType>& a, ChunkList_const_iterator<ValueType>& b) {
			std::swap(a.current_value, b.current_value);
		};

		friend bool operator==(const ChunkList_const_iterator<ValueType>& lhs,
			const ChunkList_const_iterator<ValueType>& rhs) {
			return lhs.current_value == rhs.current_value;
		};
		friend bool operator!=(const ChunkList_const_iterator<ValueType>& lhs,
			const ChunkList_const_iterator<ValueType>& rhs) {
			return !(lhs.current_value == rhs.current_value);
		};

		const_reference operator*() const { return *this->current_value; };
		const_pointer operator->() const { return this->current_value; };
		const_reference operator[](const difference_type& n) {
			return this->list[n];
		};

		ChunkList_const_iterator operator++(int) {
			if (this->k + 1 == this->list->size())
				return ChunkList_const_iterator();
			this->k++;
			this->current_value = &this->list->at(this->k);
			return *this;
		};

		ChunkList_const_iterator operator--(int) {
			return ChunkList_iterator<ValueType>::operator--(0);
		};

		ChunkList_const_iterator& operator++() {
			if (this->k + 1 == this->list->size()) {
				this->current_value = nullptr;
				this->list = nullptr;
				this->k = 0;
				return *this;
			}
			this->k++;
			this->current_value = &this->list->at(this->k);
			return *this;
		};

		ChunkList_const_iterator& operator--() {
			if (this->k - 1 == -1)
				throw std::exception();
			this->k--;
			this->current_value = &this->list->at(this->k);
			return *this;
		};

		ChunkList_const_iterator operator+(const difference_type& n) const {
			return ChunkList_const_iterator(this->list, this->k + n, &this->list->at(this->k + n));
		};


		ChunkList_const_iterator operator-(const difference_type& n) const {
			return ChunkList_const_iterator(this->list, this->k - n, &this->list->at(this->k - n));
		};

		ChunkList_const_iterator& operator+=(const difference_type& n) {
			this->k += n;
			this->current_value = &this->list->at(this->k);
			return (this);
		};

		ChunkList_const_iterator& operator-=(const difference_type& n) {
			this->k -= n;
			this->current_value = &this->list->at(this->k);
			return (this);
		};

	};

	template <typename T, int N, typename Allocator = Allocator<T>>
	class ChunkList : public IChunk<T> {
		class Chunk {
		public:
			T* list = nullptr; //list с элементами
			Chunk* prev = nullptr;
			Chunk* next = nullptr;
			int chunk_size = 0;	//кол-во занятых мест в чанке
			Allocator allocator;
			Chunk() {
				list = allocator.allocate(N);
			}

			T* copyData() {
				T* data = allocator.allocate(N);
				for (int i = 0; i < N; i++)
					data[i] = list[i];
				return data;
			}
		};
	protected:
		Chunk* first_chunk = nullptr;
		int list_size = 0;
	public:

		using value_type = T;
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename std::allocator_traits<Allocator>::pointer;
		using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
		using iterator = ChunkList_iterator<value_type>;
		using const_iterator = ChunkList_const_iterator<value_type>;

		int GetChunkSize() override { return N; } //return capacity of Chunk // delete?

		/// @brief Default constructor. Constructs an empty container with a
		/// default-constructed allocator.
		ChunkList() : first_chunk(new Chunk()) {};

		/// @brief Constructs an empty container with the given allocator
		/// @param alloc allocator to use for all memory allocations of this container
		/// s
		/*explicit ChunkList(const Allocator& alloc);*/

		/// @brief Constructs the container with count copies of elements with value
		/// and with the given allocator
		/// @param count the size of the container
		/// @param value the value to initialize elements of the container with
		/// @param alloc allocator to use for all memory allocations of this container
		ChunkList(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
			: first_chunk(new Chunk())
		{
			int k = 0;
			Chunk* tmp = first_chunk;
			while (true) {
				tmp->allocator = alloc;
				for (int j = 0; j < N; j++) {
					tmp->list[j] = value;
					k++;
					if (k == count)
						break;
				}
				tmp->next = new Chunk();
				tmp = tmp->next;
			}
		};

		/// @brief Constructs the container with count default-inserted instances of
		/// T. No copies are made.
		/// @param count the size of the container
		/// @param alloc allocator to use for all memory allocations of this container
		explicit ChunkList(size_type count, const Allocator& alloc = Allocator())
			: first_chunk(new Chunk())
		{
			int k = 0;
			Chunk* tmp = first_chunk;
			while (true) {
				tmp->allocator = alloc;
				for (int j = 0; j < N; j++) {
					tmp->list[j] = T();
					k++;
					if (k == count)
						break;
				}
				tmp->next = new Chunk();
				tmp = tmp->next;
			}
		};

		/// @brief Constructs the container with the contents of the range [first,
		/// last).
		/// @tparam InputIt Input Iterator
		/// @param first, last 	the range to copy the elements from
		/// @param alloc allocator to use for all memory allocations of this container
		template <class InputIt>
		ChunkList(InputIt first, InputIt last, const Allocator& alloc = Allocator());

		/// @brief Copy constructor. Constructs the container with the copy of the
		/// contents of other.
		/// @param other another container to be used as source to initialize the
		/// elements of the container with
		ChunkList(const ChunkList& other) {
			auto oldList = other.first_chunk;
			first_chunk = new Chunk();
			auto newList = first_chunk;
			while (oldList != nullptr) {
				newList->list = other.first_chunk->copyData();
				newList->chunk_size = other.first_chunk->chunk_size;
				if (oldList->next != nullptr) {
					newList->next = new Chunk();
					auto tmp = newList;
					newList = newList->next;
					newList->prev = tmp;
				}
				oldList = oldList->next;
			}
			list_size = other.list_size;
		};

		/// @brief Constructs the container with the copy of the contents of other,
		/// using alloc as the allocator.
		/// @param other another container to be used as source to initialize the
		/// elements of the container with
		/// @param alloc allocator to use for all memory allocations of this container
		ChunkList(const ChunkList& other, const Allocator& alloc) {
			this = ChunkList(other);
			Chunk* tmp = this->first_chunk;
			while (tmp != nullptr) {
				tmp->allocator = alloc;
				tmp = tmp->next;
			}
		};

		/**
		 * @brief Move constructor.
		 *
		 * Constructs the container with the contents of other using move semantics.
		 * Allocator is obtained by move-construction from the allocator belonging to
		 * other.
		 *
		 * @param other another container to be used as source to initialize the
		 * elements of the container with
		 */
		ChunkList(ChunkList&& other) {
			first_chunk = other.first_chunk;
			list_size = other.list_size;
			other.clear();
		};

		/**
		 * @brief Allocator-extended move constructor.
		 * Using alloc as the allocator for the new container, moving the contents
		 * from other; if alloc != other.get_allocator(), this results in an
		 * element-wise move.
		 *
		 * @param other another container to be used as source to initialize the
		 * elements of the container with
		 * @param alloc allocator to use for all memory allocations of this container
		 */
		ChunkList(ChunkList&& other, const Allocator& alloc);

		/// @brief Constructs the container with the contents of the initializer list
		/// init.
		/// @param init initializer list to initialize the elements of the container
		/// with
		/// @param alloc allocator to use for all memory allocations of this container
		ChunkList(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {};

		/// @brief Destructs the ChunkList.
		~ChunkList() override {
			clear();
		};

		/// @brief Copy assignment operator. Replaces the contents with a copy of the
		/// contents of other.
		/// @param other another container to use as data source
		/// @return *this
		ChunkList& operator=(const ChunkList& other) {
			(this) = new ChunkList(other);
			return (*this);
		};

		/**
		 * Move assignment operator.
		 *
		 * Replaces the contents with those of other using move semantics
		 * (i.e. the data in other is moved from other into this container).
		 * other is in a valid but unspecified state afterwards.
		 *
		 * @param other another container to use as data source
		 * @return *this
		 */
		ChunkList& operator=(ChunkList&& other) {
			if (this == &other)
				return this;
			this->first_chunk = other.first_chunk;
			this->list_size = other.list_size;
			other.clear();

			return *this;
		};

		/// @brief Replaces the contents with those identified by initializer list
		/// ilist.
		/// @param ilist
		/// @return this
		ChunkList& operator=(std::initializer_list<T> ilist);

		/// @brief Replaces the contents with count copies of value
		/// @param count
		/// @param value
		void assign(size_type count, const T& value) {
			if (count < 0)
				throw std::out_of_range("Out of scope");
			clear();
			for (int i = 0; i < count; i++)
				push_back(value);
			list_size = count;
		};

		/// @brief Replaces the contents with copies of those in the range [first,
		/// last).
		/// @tparam InputIt
		/// @param first
		/// @param last
		template <class InputIt>
		void assignIt(InputIt first, InputIt last) {};

		/// @brief Replaces the contents with the elements from the initializer list
		/// ilis
		/// @param ilist
		void assign(std::initializer_list<T> ilist) {};

		/// @brief Returns the allocator associated with the container.
		/// @return The associated allocator.
		allocator_type get_allocator() const noexcept {
			return first_chunk->allocator;
		};

		/// ELEMENT ACCESS

		/// @brief Returns a reference to the element at specified location pos, with
		/// bounds checking. If pos is not within the range of the container, an
		/// exception of type std::out_of_range is thrown.
		/// @param pos position of the element to return
		/// @return Reference to the requested element.
		/// @throw std::out_of_range
		reference at(size_type pos) override { //at кидает exception если за пределами в отличии от []
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			if (pos >= max_size())
				throw std::out_of_range("Out of range");

			Chunk* tmp = first_chunk;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// @brief Returns a const reference to the element at specified location pos,
		/// with bounds checking. If pos is not within the range of the container, an
		/// exception of type std::out_of_range is thrown.
		/// @param pos position of the element to return
		/// @return Const Reference to the requested element.
		/// @throw std::out_of_range
		const_reference at(size_type pos) const {
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			if (pos >= max_size())
				throw std::out_of_range("Out of range");

			Chunk* tmp = first_chunk;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// @brief Returns a reference to the element at specified location pos. No
		/// bounds checking is performed.
		/// @param pos position of the element to return
		/// @return Reference to the requested element.
		reference operator[](difference_type pos) override {
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			Chunk* tmp = first_chunk;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// @brief Returns a const reference to the element at specified location pos.
		/// No bounds checking is performed.
		/// @param pos position of the element to return
		/// @return Const Reference to the requested element.
		const_reference operator[](difference_type pos) const {
			int chunk_index = pos / N;
			int elemnt_index = pos % N;
			Chunk* tmp = first_chunk;
			while (chunk_index > 0) {
				tmp = tmp->next;
				chunk_index--;
			}
			return tmp->list[elemnt_index];
		};

		/// @brief Returns a reference to the first element in the container.
		/// Calling front on an empty container is undefined.
		/// @return Reference to the first element
		reference front() {
			if (list_size > 0)
				return first_chunk->list[0];
			else
				throw std::logic_error("Empty container");
		};

		/// @brief Returns a const reference to the first element in the container.
		/// Calling front on an empty container is undefined.
		/// @return Const reference to the first element
		const_reference front() const {
			if (list_size > 0)
				return first_chunk->list[0];
			else
				throw std::logic_error("Empty container");
		};

		/// @brief Returns a reference to the last element in the container.
		/// Calling back on an empty container causes undefined behavior.
		/// @return Reference to the last element.
		reference back() {
			if (list_size == 0)
				throw std::logic_error("Empty");
			Chunk* tmp = first_chunk;
			while (tmp->next != nullptr) {
				tmp = tmp->next;
			}
			return tmp->list[tmp->chunk_size - 1];
		};

		/// @brief Returns a const reference to the last element in the container.
		/// Calling back on an empty container causes undefined behavior.
		/// @return Const Reference to the last element.
		const_reference back() const {
			if (list_size == 0)
				throw std::logic_error("Empty");
			Chunk* tmp = first_chunk;
			while (tmp->next != nullptr) {
				tmp = tmp->next;
			}
			return tmp->list[tmp->chunk_size - 1];
		};

		/// ITERATORS

		/// @brief Returns an iterator to the first element of the ChunkList.
		/// If the ChunkList is empty, the returned iterator will be equal to end().
		/// @return Iterator to the first element.
		iterator begin() noexcept {
			return ChunkList_iterator<T>(this, 0, &at(0));
		};



		/*operator ChunkList<const T, N>() {
			ChunkList<const T, N> newChunkList;
			newChunkList.first_chunk = this->first_chunk;
			newChunkList.list_size = this->list_size;
			return newChunkList;
		}*/

		/// @brief Returns an iterator to the first element of the ChunkList.
		/// If the ChunkList is empty, the returned iterator will be equal to end().
		/// @return Iterator to the first element.
		const_iterator begin() const noexcept { //TODO: как сделать каст к 
			//<ChunkList<const T,N>
//ChunkList<const T, N>& const_obj = const_cast<ChunkList<const T,N>&>(this);
			return ChunkList_const_iterator<T>(
				this,
				0,
				&at(0)
			);
		};

		/// @brief Same to begin()
		const_iterator cbegin() const noexcept { return begin(); };

		/// @brief Returns an iterator to the element following the last element of
		/// the ChunkList. This element acts as a placeholder; attempting to access it
		/// results in undefined behavior.
		/// @return Iterator to the element following the last element.
		iterator end() noexcept {
			return ChunkList_iterator<T>();
		};

		/// @brief Returns an constant iterator to the element following the last
		/// element of the ChunkList. This element acts as a placeholder; attempting to
		/// access it results in undefined behavior.
		/// @return Constant Iterator to the element following the last element.
		const_iterator end() const noexcept {
			return ChunkList_const_iterator<T>();
		};

		/// @brief Same to end()
		const_iterator cend() const noexcept { return end(); };

		/// CAPACITY

		/// @brief Checks if the container has no elements
		/// @return true if the container is empty, false otherwise
		bool empty() const noexcept { return list_size == 0; };

		/// @brief Returns the number of elements in the container
		/// @return The number of elements in the container.
		size_type size() const noexcept override { return list_size; };

		/// @brief Returns the maximum number of elements the container is able to
		/// hold due to system or library implementation limitations
		/// @return Maximum number of elements.
		size_type max_size() const noexcept {
			int r = list_size % N;
			return (r == 0 ? list_size : list_size + N - r);
		};

		/// @brief Requests the removal of unused capacity.
		/// It is a non-binding request to reduce the memory usage without changing
		/// the size of the sequence. All iterators and references are invalidated.
		/// Past-the-end iterator is also invalidated.
		void shrink_to_fit();

		/// MODIFIERS

		/// @brief Erases all elements from the container.
		/// nvalidates any references, pointers, or iterators referring to contained
		/// elements. Any past-the-end iterators are also invalidated.
		void clear() noexcept {
			Chunk* cur = first_chunk;
			while (cur != nullptr) {
				Chunk* tmp = cur;
				cur = cur->next;
				delete tmp;
			}
			list_size = 0;
			first_chunk = nullptr;
		};

		/// @brief Inserts value before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param value element value to insert
		/// @return Iterator pointing to the inserted value.
		iterator insert(const_iterator pos, const T& value) { //TODO: assert is correct
			//if (pos == cend())
			//	return end();

			//if (max_size() > list_size) {
			//	int i = 0;
			//	int index = pos.GetIndex();
			//	Chunk* tmp = first_chunk;
			//	while (tmp->next != nullptr)
			//		tmp = tmp->next;
			//	ChunkList_iterator<T> it =
			//		ChunkList_iterator<T>(
			//			tmp,
			//			tmp->chunk_size - 1,
			//			tmp->list[chunk_size - 1]);
			//	for (; it != pos; it--, i++)
			//		at(list_size - i) = at(list_size - i - 1);
			//	at(index) = value;
			//	list_size++;
			//}
			//else {
			//	//
			//}

			//int index = pos.GetIndex();
			//for (int i = index + 1; i < list_size; i++) {
			//	if (i + 1 < max_size())
			//		at(i + 1) = at(i);
			//}
			//at(index) = value;
			//list_size++;
			//return ChunkList_iterator<T>(this, index, &at(index));
		};

		/// @brief Inserts value before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param value element value to insert
		/// @return Iterator pointing to the inserted value.
		iterator insert(const_iterator pos, T&& value) {};

		/// @brief Inserts count copies of the value before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param count number of elements to insert
		/// @param value element value to insert
		/// @return Iterator pointing to the first element inserted, or pos if count
		/// == 0.
		iterator insert(const_iterator pos, size_type count, const T& value) {};

		/// @brief Inserts elements from range [first, last) before pos.
		/// @tparam InputIt Input Iterator
		/// @param pos iterator before which the content will be inserted.
		/// @param first,last the range of elements to insert, can't be iterators into
		/// container for which insert is called
		/// @return Iterator pointing to the first element inserted, or pos if first
		/// == last.
		template <class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last) {};

		/// @brief Inserts elements from initializer list before pos.
		/// @param pos iterator before which the content will be inserted.
		/// @param ilist initializer list to insert the values from
		/// @return Iterator pointing to the first element inserted, or pos if ilist
		/// is empty.
		iterator insert(const_iterator pos, std::initializer_list<T> ilist) {};

		/// @brief Inserts a new element into the container directly before pos.
		/// @param pos iterator before which the new element will be constructed
		/// @param ...args arguments to forward to the constructor of the element
		/// @return terator pointing to the emplaced element.
		template <class... Args>
		iterator emplace(const_iterator pos, Args&&... args);

		/// @brief Removes the element at pos.
		/// @param pos iterator to the element to remove
		/// @return Iterator following the last removed element.
		iterator erase(const_iterator pos) { //сдвигаем все элементы влево удаляя текущий
			//TODO: удалять чанк если удалили последний элемент
			auto index = pos.GetIndex();
			if (index + 1 == list_size) {
				list_size--;
				return ChunkList_iterator<T>();
			}
			for (int i = index + 1; i < list_size; i++) {
				at(i - 1) = at(i);
			}
			list_size--;
			return ChunkList_iterator<T>(this, index, &at(index));
		};

		/// @brief Removes the elements in the range [first, last).
		/// @param first,last range of elements to remove
		/// @return Iterator following the last removed element.
		iterator erase(const_iterator first, const_iterator last) { //TODO: CHECK
			int i = 0;
			int temp = 0;
			int index = last.GetIndex();
			auto it = (*this).begin();

			for (it; it != first; it++, i++);
			for (it = first; it != last; it++, temp++, i++);

			for (auto it = last; it != (*this).end(); it++, i++)
				at(i - 1) = at(i);

			list_size -= temp;
			return ChunkList_iterator<T>(this, index, &at(index));
		};

		/// @brief Appends the given element value to the end of the container.
		/// The new element is initialized as a copy of value.
		/// @param value the value of the element to append
		void push_back(const T& value) {
			if (first_chunk == nullptr)
				first_chunk = new Chunk();

			Chunk* tmp = first_chunk;
			while (tmp->next != nullptr)
				tmp = tmp->next;
			if (tmp->chunk_size == N)
			{
				tmp->next = new Chunk();
				tmp = tmp->next;
			}
			tmp->list[tmp->chunk_size] = value;
			tmp->chunk_size++;
			list_size++;
		};

		/// @brief Appends the given element value to the end of the container.
		/// Value is moved into the new element.
		/// @param value the value of the element to append
		void push_back(T&& value) {
			Chunk* tmp = first_chunk;
			while (tmp->next != nullptr)
				tmp = tmp->next;
			if (tmp->chunk_size == N)
			{
				tmp->next = new Chunk();
				tmp = tmp->next;
			}
			tmp->list[tmp->chunk_size] = std::move(value); //TODO: correct??
			tmp->chunk_size++;
			list_size++;
		};

		/// @brief Appends a new element to the end of the container.
		/// @param ...args arguments to forward to the constructor of the element
		/// @return A reference to the inserted element.
		template <class... Args>
		reference emplace_back(Args&&... args);

		/// @brief Removes the last element of the container.
		void pop_back() {
			this->list_size--;
			Chunk* tmp = this->first_chunk;
			while (tmp->next != nullptr)
				tmp = tmp->next;
			tmp->chunk_size--;
		};

		/// @brief Prepends the given element value to the beginning of the container.
		/// @param value the value of the element to prepend
		void push_front(const T& value) {
			insert(cbegin(), value);
		};

		/// @brief Prepends the given element value to the beginning of the container.
		/// @param value moved value of the element to prepend
		void push_front(T&& value) {
			insert(cbegin(), std::move(value));
		};

		/// @brief Inserts a new element to the beginning of the container.
		/// @param ...args arguments to forward to the constructor of the element
		/// @return A reference to the inserted element.
		template <class... Args>
		reference emplace_front(Args&&... args);

		/// @brief Removes the first element of the container.
		void pop_front();

		/// @brief Resizes the container to contain count elements.
		/// If the current size is greater than count, the container is reduced to its
		/// first count elements. If the current size is less than count, additional
		/// default-inserted elements are appended
		/// @param count new size of the container
		void resize(size_type count);

		/// @brief Resizes the container to contain count elements.
		/// If the current size is greater than count, the container is reduced to its
		/// first count elements. If the current size is less than count, additional
		/// copies of value are appended.
		/// @param count new size of the container
		/// @param value the value to initialize the new elements with
		void resize(size_type count, const value_type& value);

		/// @brief Exchanges the contents of the container with those of other.
		/// Does not invoke any move, copy, or swap operations on individual elements.
		/// All iterators and references remain valid. The past-the-end iterator is
		/// invalidated.
		/// @param other container to exchange the contents with
		void swap(ChunkList& other) {
			Chunk* tempFirst_chunk;
			int tempList_Size;

			tempFirst_chunk = other.first_chunk;
			tempList_Size = other.list_size;

			other.first_chunk = first_chunk;
			other.list_size = list_size;

			first_chunk = tempFirst_chunk;
			list_size = tempList_Size;
		};

		/// COMPARISIONS

		/// @brief Checks if the contents of lhs and rhs are equal
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator==(const ChunkList<U, N, Alloc>& lhs,
			const ChunkList<U, N, Alloc>& rhs);

		/// @brief Checks if the contents of lhs and rhs are not equal
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator!=(const ChunkList<U, N, Alloc>& lhs,
			const ChunkList<U, N, Alloc>& rhs);

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator>(const ChunkList<U, N, Alloc>& lhs, const ChunkList<U, N, Alloc>& rhs);

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator<(const ChunkList<U, N, Alloc>& lhs, const ChunkList<U, N, Alloc>& rhs);

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator>=(const ChunkList<U, N, Alloc>& lhs,
			const ChunkList<U, N, Alloc>& rhs);

		/// @brief Compares the contents of lhs and rhs lexicographically.
		/// @param lhs,rhs ChunkLists whose contents to compare
		template <class U, int N, class Alloc>
		friend bool operator<=(const ChunkList<U, N, Alloc>& lhs,
			const ChunkList<U, N, Alloc>& rhs);

		// operator <=> will be handy
	};

	/// NON-MEMBER FUNCTIONS

	/// @brief  Swaps the contents of lhs and rhs.
	/// @param lhs,rhs containers whose contents to swap
	template <class T, int N, class Alloc>
	void swap(ChunkList<T, N, Alloc>& lhs, ChunkList<T, N, Alloc>& rhs);

	/// @brief Erases all elements that compare equal to value from the container.
	/// @param c container from which to erase
	/// @param value value to be removed
	/// @return The number of erased elements.
	template <class T, int N, class Alloc, class U>
	typename ChunkList<T, N, Alloc>::size_type erase(ChunkList<T, N, Alloc>& c, const U& value);

	/// @brief Erases all elements that compare equal to value from the container.
	/// @param c container from which to erase
	/// @param pred unary predicate which returns ​true if the element should be
	/// erased.
	/// @return The number of erased elements.
	template <class T, int N, class Alloc, class Pred>
	typename ChunkList<T, N, Alloc>::size_type erase_if(ChunkList<T, N, Alloc>& c, Pred pred);
}  // namespace fefu_laboratory_two
