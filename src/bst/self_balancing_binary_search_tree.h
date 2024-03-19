#ifndef TRANSACTIONS_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define TRANSACTIONS_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include "../common/base_class.h"
#include "../common/data.h"

namespace s21 {

	enum Color {
		Black,
		Red,
	};

	class BSTNode {
	public:
		BSTNode* parent = nullptr;
		BSTNode* link[2] = {nullptr,nullptr};
		/*BSTNode* left = nullptr;
		BSTNode* right = nullptr;*/
		Data data;
		bool color = Color::Black;

		BSTNode() = default;
		explicit BSTNode(const Data& value) : data(value) {
			color = Color::Red;
		};
	};

	class SelfBalancingBinarySearchTree :BaseClass
	{
	public:
		class BSTIterator;
		using Node = BSTNode;
		using Pointer = Node*;
		using Iterator = BSTIterator;
		using ValueType = Data;
		Pointer leaf_ ;

		SelfBalancingBinarySearchTree() {
			root_ = nullptr;
			leaf_ = new Node();
		}

		~SelfBalancingBinarySearchTree() {
			// ������� �����
			Clear();
			delete leaf_;
		}

		
		// internal class BSTIterator
		class BSTIterator {
			friend class SelfBalancingBinarySearchTree;

		public:
			BSTIterator(Pointer node, Pointer leaf) : current_(node), leaf_(leaf) {}
			BSTIterator() : current_(nullptr), leaf_(nullptr) {}
			BSTIterator(const BSTIterator& obj) { 
				current_ = obj.current_; 
				leaf_ = obj.leaf_;
			}

			BSTIterator& operator=(const BSTIterator& obj) noexcept {
				current_ = obj.current_;
				leaf_ = obj.leaf_;
				return *this;
			}

			BSTIterator& operator--() noexcept {
				if (current_->link[0] != leaf_) {
					current_ = current_->link[0];
					while (current_->link[1] != leaf_) {
						current_ = current_->link[1];
					}
				}
				else {
					Pointer tmp = current_->parent;
					current_ = (tmp && tmp->link[1] == current_) ? tmp : nullptr;
					
					/*while (tmp && tmp->data.GetKey() > current_->data.GetKey()) {
						current_ = tmp;
						tmp = tmp->parent;
					}
					current_ = tmp;*/
				}
				return *this;
			}

			BSTIterator& operator++() noexcept {
				if (current_->link[1] != leaf_) {
					current_ = current_->link[1];
					while (current_->link[0] != leaf_) {
						current_ = current_->link[0];
					}
				}
				else {
					Pointer tmp = current_->parent;
					current_ = (tmp && tmp->link[0] == current_) ? tmp : nullptr;
					/*while (tmp && tmp->data.GetKey() < current_->data.GetKey()) {
						current_ = tmp;
						tmp = tmp->parent;
					}
					current_ = tmp;*/
				}
				return *this;
			}

			bool operator!=(const BSTIterator& other) const noexcept {
				return (current_ != other.current_);
			}

			ValueType* operator->() noexcept { return &(current_->data); }

		private:
			Pointer current_;
			Pointer leaf_;
		};

		std::string Set(std::string key, Student student, int validity = 0);
		Student Get(std::string key);
		bool Exists(std::string key);
		bool Del(std::string key);
		std::string Update(std::string param);
		std::string Keys(std::string param);
		std::string Rename(std::string param);
		std::string Ttl(std::string param);
		std::string Find(std::string param);
		std::string Showall(std::string param);
		std::string Upload(std::string param);
		std::string Export(std::string param);
		/*Iterator Begin();
		Iterator End();*/
		void Clear();
		Pointer GetMinForCurrentNode(Pointer node);
		int CountChildren(Pointer node);
		Pointer GetChild(Pointer node);
		Pointer GetParent(Pointer node);
		Pointer Search(std::string key);
		bool RemoveNode(Pointer node);
		void RemoveNodeWithoutChildren(Pointer node);
		void RemoveNodeWithOneChild(Pointer node);
		void RemoveNodeWithTwoChild(Pointer node);
		void RebalanceTree(Pointer node);
		bool NodeIsLeftChild(Pointer node);
		Pointer MinValueFromRight(Pointer node);
		void ExcludeNode(Pointer a, Pointer b);


	private:
		Pointer root_;
		void Swap(Pointer a, Pointer b);
		void LeftRotation(Pointer node);
		void RightRotation(Pointer node);
		void BalanceTree(Pointer node);


	};

}  // namespace s21

#endif  // TRANSACTIONS_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_


#pragma region MyRegion



//template <class Key, class T>
//class Map {
//public:
//	class MapConstIterator;
//
//	// List Member type
//	using KeyType = Key;
//	using MappedType = T;
//	using ValueType = std::pair<const KeyType, MappedType>;
//	using Reference = ValueType&;
//	using ConstReference = const ValueType&;
//	using SizeType = size_t;
//	using Node = BSTNode<KeyType, MappedType>;
//	using Pointer = Node*;
//	using Iterator = MapIterator;
//	using ConstIterator = MapConstIterator;
//	using Pair = std::pair<Iterator, bool>;
//
//
//
//	// Map Member functions
//	Map() = default;
//	Map(std::initializer_list<ValueType> const& items);
//	explicit Map(const Map& m);
//	Map(Map&& m) noexcept;
//	~Map();
//	Map& operator=(Map&& m) noexcept;
//
//	// Map Element access
//	T& At(const Key& key);
//	T& operator[](const Key& key);
//
//	// Map Iterators
//	Iterator Begin();
//	Iterator End();
//
//	// Map Capacity
//	bool Empty() const;
//	SizeType Size();
//	SizeType MaxSize();
//
//	// Map Modifiers
//	void Clear();
//	Pair Insert(const ValueType& value);
//	Pair Insert(const Key& key, const T& obj);
//	Pair InsertOrAssign(const Key& key, const T& obj);
//	void Erase(Iterator pos);
//	void Swap(Map& other);
//	void Merge(Map& other);
//
//	// Map Lookup
//	bool Contains(const Key& key);
//
//	
//
//private:
//	Pointer root_ = nullptr;
//	SizeType size_{ 0 };
//
//	// Support functions
//	void CopyTree(const Map& other);
//	void EraseList(Pointer tmp);  // delete element, that has not children
//	void EraseLeftBranch(
//		Pointer tmp);  // delete element, that has only left branch
//	void EraseRightBranch(
//		Pointer tmp);             // delete element, that has only right branch
//	void EraseNode(Pointer tmp);  // delete element, that has two children
//	Pair Search(const Key& key);
//};
//
//// Constructors
//template <class Key, class T>
//Map<Key, T>::Map(std::initializer_list<ValueType> const& items) {
//	for (auto it : items) {
//		Insert(it);
//	}
//}
//
//template <class Key, class T>
//Map<Key, T>::Map(const Map& m) {
//	CopyTree(m);
//}
//
//template <class Key, class T>
//Map<Key, T>::Map(Map&& m) noexcept {
//	Swap(m);
//}
//
//template <class Key, class T>
//Map<Key, T>::~Map() {
//	Clear();
//}
//
//template <class Key, class T>
//Map<Key, T>& Map<Key, T>::operator=(Map&& m) noexcept {
//	Clear();
//	Swap(m);
//	return *this;
//}
//
//template <class Key, class T>
//T& Map<Key, T>::At(const Key& key) {
//	Pair res = Search(key);
//	if (!res.second) {
//		throw std::out_of_range("This element is not exists");
//	}
//	return res.first->second;
//}
//
//template <class Key, class T>
//T& Map<Key, T>::operator[](const Key& key) {
//	Pair res = Search(key);
//	if (!res.second) {
//		auto tmp = Insert(key, T());
//		return tmp.first->second;
//	}
//	return res.first->second;
//}
//
//// Map Iterators
//template <class Key, class T>
//typename Map<Key, T>::Iterator Map<Key, T>::Begin() {
//	if (!root_) {
//		return Iterator();
//	}
//	else {
//		Iterator begin(root_);
//		while (begin.current_->link[0]) {
//			begin = begin.current_->link[0];
//		}
//		return begin;
//	}
//}
//
//template <class Key, class T>
//typename Map<Key, T>::Iterator Map<Key, T>::End() {
//	return Iterator();
//}
//
//// Map Capacity
//template <class Key, class T>
//bool Map<Key, T>::Empty() const {
//	return root_ == nullptr;
//}
//
//template <class Key, class T>
//typename Map<Key, T>::SizeType Map<Key, T>::Size() {
//	return size_;
//}
//
//template <class Key, class T>
//typename Map<Key, T>::SizeType Map<Key, T>::MaxSize() {
//	return std::numeric_limits<SizeType>::max() / sizeof(Node);
//}
//
//// Map Modifiers
//template <class Key, class T>
//void Map<Key, T>::Clear() {
//	for (Iterator i = Begin(); i != End();) {
//		Iterator tmp = i;
//		++i;
//		Erase(tmp);
//	}
//}
//
//template <class Key, class T>
//std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::Insert(
//	const ValueType& value) {
//	Pointer tmp = new Node(value);
//	if (!root_) {  // if set is empty
//		root_ = tmp;
//	}
//	else {
//		Pair top = Search(value.first);
//		if (top.second) {
//			delete tmp;
//			return std::make_pair(Iterator(root_), false);
//		}
//		else {
//			tmp->parent = top.first.current_;
//			if (top.first->first > value.first) {
//				top.first.current_->link[0] = tmp;
//			}
//			else {
//				top.first.current_->link[1] = tmp;
//			}
//		}
//	}
//	++size_;
//	return std::make_pair(Iterator(tmp), true);
//}
//
//template <class Key, class T>
//std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::Insert(
//	const Key& key, const T& obj) {
//	return Insert(std::make_pair(key, obj));
//}
//
//template <class Key, class T>
//std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::InsertOrAssign(
//	const Key& key, const T& obj) {
//	Pair res = Search(key);
//	if (!res.second) {
//		return Insert(key, obj);
//	}
//	else {
//		res.first->second = obj;
//		return std::make_pair(res.first, true);
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::Erase(Iterator pos) {
//	Pointer tmp = pos.current_;
//	if (tmp) {
//		if (!tmp->link[0] && !tmp->link[1]) {
//			EraseList(tmp);
//		}
//		else if (tmp->link[0] && !tmp->link[1]) {
//			EraseLeftBranch(tmp);
//		}
//		else if (!tmp->link[0] && tmp->link[1]) {
//			EraseRightBranch(tmp);
//		}
//		else if (tmp->link[0] && tmp->link[1]) {
//			EraseNode(tmp);
//		}
//		delete tmp;
//		size_--;
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::Swap(Map& other) {
//	if (this != &other) {
//		std::swap(root_, other.root_);
//		std::swap(size_, other.size_);
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::Merge(Map& other) {
//	for (Iterator i = other.Begin(); i != other.End(); ++i) {
//		Insert(i.current_->node_pair);
//	}
//}
//
//// Map Lookup
//template <class Key, class T>
//bool Map<Key, T>::Contains(const Key& key) {
//	Pair res = Search(key);
//	return res.second;
//}
//
//
//
//// Other functions
//template <class Key, class T>
//void Map<Key, T>::CopyTree(const Map& other) {
//	if (other.root_) {
//		s21::Queue<Pointer> tmp;
//		tmp.Push(other.root_);
//		while (!tmp.Empty()) {
//			int n = tmp.Size();
//			for (int i = 0; i < n; i++) {
//				Pointer curr = tmp.Front();
//				tmp.Pop();
//				Insert(curr->node_pair);
//				if (curr->link[0]) tmp.Push(curr->link[0]);
//				if (curr->link[1]) tmp.Push(curr->link[1]);
//			}
//		}
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::EraseList(Pointer tmp) {
//	if (size_ == 1)
//		root_ = nullptr;
//	else {
//		if (tmp->parent->node_pair.first > tmp->node_pair.first)
//			tmp->parent->link[0] = nullptr;
//		else
//			tmp->parent->link[1] = nullptr;
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::EraseLeftBranch(Pointer tmp) {
//	if (tmp == root_) {
//		root_ = tmp->link[0];
//		root_->parent = nullptr;
//	}
//	else {
//		tmp->link[0]->parent = tmp->parent;
//		if (tmp->parent->node_pair.first > tmp->node_pair.first)
//			tmp->parent->link[0] = tmp->link[0];
//		else
//			tmp->parent->link[1] = tmp->link[0];
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::EraseRightBranch(Pointer tmp) {
//	if (tmp == root_) {
//		root_ = tmp->link[1];
//		root_->parent = nullptr;
//	}
//	else {
//		tmp->link[1]->parent = tmp->parent;
//		if (tmp->parent->node_pair.first > tmp->node_pair.first)
//			tmp->parent->link[0] = tmp->link[1];
//		else
//			tmp->parent->link[1] = tmp->link[1];
//	}
//}
//
//template <class Key, class T>
//void Map<Key, T>::EraseNode(Pointer tmp) {
//	Iterator it(tmp);
//	--it;
//	Iterator p(it.current_->parent);
//
//	if (it.current_->link[0]) {
//		if (it->first > p->first) {
//			p.current_->link[1] = it.current_->link[0];
//		}
//		else {
//			p.current_->link[0] = it.current_->link[0];
//		}
//		it.current_->link[0]->parent = p.current_;
//	}
//	else {
//		if (it->first > p->first)
//			p.current_->link[1] = nullptr;
//		else
//			p.current_->link[0] = nullptr;
//	}
//
//	it.current_->link[0] = tmp->link[0];
//	it.current_->link[1] = tmp->link[1];
//	it.current_->parent = tmp->parent;
//
//	if (tmp->link[0]) tmp->link[0]->parent = it.current_;
//	if (tmp->link[1]) tmp->link[1]->parent = it.current_;
//	if (tmp == root_)
//		root_ = it.current_;
//	else {
//		if (it->first > tmp->parent->node_pair.first) {
//			tmp->parent->link[1] = it.current_;
//		}
//		else {
//			tmp->parent->link[0] = it.current_;
//		}
//	}
//}
//
//template <class Key, class T>
//std::pair<typename Map<Key, T>::Iterator, bool> Map<Key, T>::Search(
//	const Key& key) {
//	Pointer it = root_, top = it;
//	while (it) {
//		top = it;
//		if (it->node_pair.first == key) {
//			return std::make_pair(Iterator(top), true);
//		}
//		else if (it->node_pair.first > key) {
//			it = it->link[0];
//		}
//		else {
//			it = it->link[1];
//		}
//	}
//	return std::make_pair(Iterator(top), false);
//}

#pragma endregion