#include "BST.h"
#include <iostream>

void makeRootCopy(BinarySearchTree::Node* source, BinarySearchTree::Node* target){
    delete target->left;
    delete target->right;
    target->left = nullptr;
    target->right = nullptr;
    if(source->left != nullptr){
        target->left = new BinarySearchTree::Node(source->left->keyValuePair.first, source->left->keyValuePair.second);
        target->left->parent = target;
        makeRootCopy(source->left, target->left);
    }
    if(source->right != nullptr){
        target->right = new BinarySearchTree::Node(source->right->keyValuePair.first, source->right->keyValuePair.second);
        target->right->parent = target;
        makeRootCopy(source->right, target->right);
    }
}

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right) :
        keyValuePair(std::make_pair(key, value)),
        parent(parent),
        left(left),
        right(right)
{}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) {
    //delete _root;
    if (other._root == nullptr) {
        //_root = nullptr;
        return;
    }
    _root = new Node(other._root->keyValuePair.first, other._root->keyValuePair.second);
    makeRootCopy(other._root, this->_root);
    //_root = other._root;
    _size = other._size;
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this == &other) {
        return *this;
    }
    delete _root;
    if (other._root == nullptr) {
     //_root = nullptr;
        return *this;
    }
    _root = new Node(other._root->keyValuePair.first, other._root->keyValuePair.second);
    makeRootCopy(other._root, this->_root);
    //_root = other._root;
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    delete _root;
    _root = new Node(other._root->keyValuePair.first, other._root->keyValuePair.second);
    makeRootCopy(other._root, this->_root);
    //_root = other._root;
    other._root = nullptr;
    _size = other._size;
    other._size = 0;
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete _root;
    _root = new Node(other._root->keyValuePair.first, other._root->keyValuePair.second);
    makeRootCopy(other._root, this->_root);
    //_root = other._root;
    other._root = nullptr;
    _size = other._size;
    other._size = 0;
    return *this;
}

BinarySearchTree::~BinarySearchTree(){
    delete this->_root;
}

BinarySearchTree::Iterator::Iterator(BinarySearchTree::Node *node) {
    _node = node;
}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if(_node->right != nullptr){
        _node = _node->right;
        while(_node->left != nullptr){
            _node = _node->left;
        }
        return *this;
    }
    while(_node->parent != nullptr && _node->parent->right == _node){
        _node = _node->parent;
    }
    _node = _node->parent;
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if(_node->left != nullptr){
        _node = _node->left;
        while(_node->right != nullptr){
            _node = _node->right;
        }
        return *this;
    }
    while(_node->parent != nullptr && _node->parent->left == _node){
        _node = _node->parent;
    }
    _node = _node->parent;
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
}

bool BinarySearchTree::Iterator::operator==(const BinarySearchTree::Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const BinarySearchTree::Iterator &other) const {
    return _node != other._node;
}

//private
void BinarySearchTree::insert(const Key &key, const Value &value, Node *node) {
    if (node == nullptr) {
        _root = new Node(key, value);
        _size++;
        return;
    }
    if (key < node->keyValuePair.first) {
        if (node->left == nullptr) {
            node->left = new Node(key, value);
            node->left->parent = node;
            _size++;
            return;
        }
        insert(key, value, node->left);
    } else if (key > node->keyValuePair.first) {
        if (node->right == nullptr) {
            node->right = new Node(key, value);
            node->right->parent = node;
            _size++;
            return;
        }
        insert(key, value, node->right);
    } else {
        node->keyValuePair.second = value;
    }
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    insert(key, value, _root);
}

//private
void BinarySearchTree::erase(const Key &key, BinarySearchTree::Node *node) {
    if (node == nullptr) {
        return;
    }
    if (key < node->keyValuePair.first) {
        erase(key, node->left);
    } else if (key > node->keyValuePair.first) {
        erase(key, node->right);
    } else {
        if (node->left == nullptr && node->right == nullptr) {
            if (node->parent == nullptr) {
                _root = nullptr;
            } else if (node->parent->left == node) {
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
            delete node;
            _size--;
        } else if (node->left == nullptr) {
            if (node->parent == nullptr) {
                _root = node->right;
                _root->parent = nullptr;
            } else if (node->parent->left == node) {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            } else {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            delete node;
            _size--;
        } else if (node->right == nullptr) {
            if (node->parent == nullptr) {
                _root = node->left;
                _root->parent = nullptr;
            } else if (node->parent->left == node) {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            } else {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            delete node;
            _size--;
        } else {
            Node *tmp = node->right;
            while (tmp->left != nullptr) {
                tmp = tmp->left;
            }
            node->keyValuePair = tmp->keyValuePair;
            erase(tmp->keyValuePair.first, tmp);
        }
    }
}

void BinarySearchTree::erase(const Key &key) {
    erase(key, _root);
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    return Iterator(findKey(key));
}

//private
BinarySearchTree::Node* BinarySearchTree::find(Key key, Node* node) const{
    if(node == nullptr){
        return nullptr;
    }
    if(key < node->keyValuePair.first){
        return find(key, node->left);
    } else if(key > node->keyValuePair.first){
        return find(key, node->right);
    } else{
        return node;
    }
}

BinarySearchTree::Node* BinarySearchTree::findKey(const Key &key) const {
    if(_root == nullptr){
        return nullptr;
    }
    return find(key, _root);
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
    Node *firstKey = findKey(key);
    Iterator secondKey(firstKey);
    secondKey++;
    return std::make_pair(Iterator(firstKey), secondKey);
    //return std::make_pair(min(key), max(key));
}

BinarySearchTree::Iterator BinarySearchTree::min(const Key &key) const{
    Node* node = findKey(key);
    while(node->left != nullptr){
        node = node->left;
    }
    return Iterator(node);
}

BinarySearchTree::Iterator BinarySearchTree::max(const Key &key) const{
    Node* node = findKey(key);
    while(node->right != nullptr){
        node = node->right;
    }
    return Iterator(node);
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node* node = _root;
    while(node != nullptr && node->left != nullptr){
        node = node->left;
    }
    return Iterator(node);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Node* node = _root;
    while(node != nullptr && node->right != nullptr){
        node = node->right;
    }
    return Iterator(node);
}

size_t BinarySearchTree::size() const {
    return _size;
}

//private
void BinarySearchTree::print(BinarySearchTree::Node *node) {
    if(node == nullptr){
        return;
    }
    print(node->left);
    std::cout << node->keyValuePair.first << " " << node->keyValuePair.second << std::endl;
    print(node->right);
}

void BinarySearchTree::print() {
    print(_root);
}

//private
void BinarySearchTree::printWithValues(BinarySearchTree::Node *node) {
    if(node == nullptr){
        return;
    }
    printWithValues(node->left);
    std::cout << node->keyValuePair.first << " " << node->keyValuePair.second << std::endl;
    printWithValues(node->right);
}

void BinarySearchTree::printWithValues() {
    printWithValues(_root);
}