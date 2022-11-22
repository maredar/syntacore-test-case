#ifndef STC_SRC_TREE_HPP_INCLUDED
#define STC_SRC_TREE_HPP_INCLUDED

#include <stdexcept>
#include <queue>
#include <iostream>
#include <algorithm>
#include <stack>

namespace STC {
namespace Containers {
namespace except {

class null_node_access_exception : public std::exception {
public:
    null_node_access_exception(std::string&& node_access_code) {
        _err_msg = std::move(node_access_code);
        _err_msg += " code tries to access 'nullptr' node!";
    }

public:
    const char* what() const noexcept override {
        return _err_msg.c_str();
    }

private:
    std::string _err_msg;
};

class key_already_exists_exception : public std::exception {
public:
    key_already_exists_exception(const int key) : _err_msg{} {
        _err_msg += "Failed to add key: ";
        _err_msg += std::to_string(key) + ". ";
        _err_msg += "Key already exists!";
    }

public:
    const char* what() const noexcept override {
        return _err_msg.c_str();
    }

private:
    std::string _err_msg;
};

class invalid_index_requested_exception : public std::exception {
public:
    invalid_index_requested_exception(const size_t index) : _err_msg{} {
        _err_msg += "Requested index '" + std::to_string(index) + "' ";
        _err_msg += " is out of bound!";
    }

public:
    const char* what() const noexcept override {
        return _err_msg.c_str();
    }

private:
    std::string _err_msg;
};

} //namespace except

struct Node {
    enum color_t { BLACK, RED } color;
    int key;

    Node* left;
    Node* right;
    Node* parent;

    Node(int key, color_t color = color_t::RED) 
        : color{ color }, key{ key }, left{ nullptr }, right{ nullptr }, parent{ nullptr } 
    {
        //empty
    }
};

/*
* Self-balancing red-black tree
*/
class RBTree {
public:
    RBTree() : _root{ nullptr } {
        //empty
    }
 
    RBTree(const RBTree& that) : _root{ nullptr } {
        if(&that == this) {
            return;
        }
        if(that._root == nullptr) {
            std::cerr << "Warning: tree constructed from empty tree." << std::endl;
            return;
        }
        _deep_copy(_root, that._root);
    }

    RBTree(RBTree&& that) : _root{ nullptr } {
        if (&that == this) {
            return;
        }
        if (that._root == nullptr) {
            std::cerr << "Warning: tree move constructed from empty tree." << std::endl;
            return;
        }
        _root = std::move(that._root);
        that._root = nullptr;
    }

    ~RBTree() {
        _clear_tree();
    }

public:
    RBTree& operator=(const RBTree& that) {
        if (&that == this) {
            return *this;
        }
        _clear_tree();
        _deep_copy(_root, that._root);
    }

    RBTree& operator=(RBTree&& that) {
        if (&that == this) {
            return *this;
        }
        _clear_tree();
        _root = std::move(that._root);
        that._root = nullptr;
        return *this;
    }

public:
    void print() const noexcept
    {
        _print("#", _root, false);    
    }

    void insert(const int key) {
        Node* node = new Node(key, Node::RED);
        if (_root == nullptr) {
            node->color = Node::BLACK;
            _root = node;
            return;
        }
        _insert(node, _root);
        _rebalance(node);
    }

    int find_from_begin(const size_t index) const {
        std::stack<Node *> s;
        Node* curr = _root;

        size_t counter = index;

        while ((curr != nullptr || s.empty() == false)) {
            while (curr !=  nullptr) {
                s.push(curr);
                curr = curr->left;
            }
            curr = s.top();
            s.pop();
            counter--;
            if (counter == 0) {
                break;
            }
            curr = curr->right;
        }
        if (counter != 0) {
            throw except::invalid_index_requested_exception(index);
        }
        return curr->key;
    }

    size_t count_nodes_less(const int key) const noexcept {
        size_t counter = _count_nodes_less(key, _root);
        return counter;
    }

    size_t size() const noexcept {
        return _size(_root);
    }

private:
    void _deep_copy(Node* dest_node, const Node* source_node) {
        if (source_node == nullptr) {
            return;
        }
        dest_node = new Node(*source_node);
        _deep_copy(dest_node->left, source_node->left);
        _deep_copy(dest_node->right, source_node->right);
    }

    size_t _count_nodes_less(const int key, Node* subtree_root) const noexcept {
        if (subtree_root == nullptr) {
            return 0;
        }

        if (key <= subtree_root->key) {
            return _count_nodes_less(key, subtree_root->left);
        }
        else {
            return _size(subtree_root->left) + 1 + _count_nodes_less(key, subtree_root->right);
        }
    }

    size_t _size(Node* root) const noexcept {
        if (root == nullptr) {
            return 0;
        }
        else {
            return _size(root->left) + 1 + _size(root->right);
        }
    }

    void _print(const std::string& prefix, const Node* node, bool isLeft) const noexcept
    {
        if( node != nullptr )
        {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──" );
            std::cout << node->key;
            switch (node->color) {
                case Node::BLACK:
                    std::cout << " b";
                    break;
                case Node::RED:
                    std::cout << " r";
                    break;
            }
            std::cout << std::endl;

            _print( prefix + (isLeft ? "│   " : "    "), node->left, true);
            _print( prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

    void _insert(Node* node, Node* current_node) {
        if (node->key == current_node->key) {
            throw except::key_already_exists_exception(node->key);
        }

        if (node->key < current_node->key) {
            if (current_node->left == nullptr) {
                current_node->left = node;
                node->parent = current_node;
            } 
            else {
                _insert(node, current_node->left);
            }
        }
        else {
            if (current_node->right == nullptr) {
                current_node->right = node;
                node->parent = current_node;
            }
            else {
                _insert(node, current_node->right);
            }
        }
    }

    void _rotate_left(Node* rotation_root) {
        if (rotation_root == nullptr) {
            throw except::null_node_access_exception("_rotate_left(Node*)");
        }

        Node* new_root = rotation_root->right; 
        rotation_root->right = new_root->left;

        if (rotation_root->right != nullptr) {
            rotation_root->right->parent = rotation_root;
        }
        new_root->parent = rotation_root->parent;

        if (rotation_root->parent == nullptr) {
            _root = new_root;
        }
        else if (rotation_root->parent->left == rotation_root) {
            rotation_root->parent->left = new_root;
        }
        else if (rotation_root->parent->right == rotation_root) {
            rotation_root->parent->right = new_root;
        }

        new_root->left = rotation_root;
        rotation_root->parent = new_root;
    }

    void _rotate_right(Node* rotation_root) {
        if (rotation_root == nullptr) {
            throw except::null_node_access_exception("_rotate_right(Node*)");
        }
        Node* new_root = rotation_root->left; 
        rotation_root->left = new_root->right;

        if (rotation_root->left != nullptr) {
            rotation_root->left->parent = rotation_root;
        }
        new_root->parent = rotation_root->parent;

        if (rotation_root->parent == nullptr) {
            _root = new_root;
        }
        else if (rotation_root->parent->left == rotation_root) {
            rotation_root->parent->left = new_root;
        }
        else if (rotation_root->parent->right == rotation_root) {
            rotation_root->parent->right = new_root;
        }

        new_root->right = rotation_root;
        rotation_root->parent = new_root;
    }
    
    void _rebalance(Node* inserted) {
        if (inserted == nullptr) {
            return;
        }
        Node* parent = nullptr;
        Node* grand_parent = nullptr;

        while((inserted != _root) && (inserted->color != Node::BLACK) &&
              (inserted->parent->color == Node::RED)) 
        {
            parent = inserted->parent;
            grand_parent = inserted->parent->parent;

            if (parent == grand_parent->left) {
                Node* uncle = grand_parent->right;

                if (uncle != nullptr && uncle->color == Node::RED) {
                    grand_parent->color = Node::RED;
                    parent->color = Node::BLACK;
                    uncle->color = Node::BLACK;
                    inserted = grand_parent;
                }
                else {
                    if (inserted == parent->right) {
                        _rotate_left(parent);
                        inserted = parent;
                        parent = inserted->parent;
                    }
                    else {
                        _rotate_right(grand_parent);
                        std::swap(parent->color, grand_parent->color);
                        inserted = parent;
                    }
                }
            }
            else {
                Node* uncle = grand_parent->left;

                if ((uncle != nullptr) && (uncle->color == Node::RED)) {
                    grand_parent->color = Node::RED;
                    parent->color = Node::BLACK;
                    uncle->color = Node::BLACK;
                    inserted = grand_parent;
                }
                else {
                    if (inserted == parent->left) {
                        _rotate_right(parent);
                        inserted = parent;
                        parent = inserted->parent;
                    }
                    else {
                        _rotate_left(grand_parent);
                        std::swap(parent->color, grand_parent->color);
                        inserted = parent;
                    }
                }
            }
        }
        _root->color = Node::BLACK;
    }

    void _clear_tree() noexcept {
        if (_root == nullptr) {
            return;
        }
        std::queue<Node*> queue{};
        queue.push(_root);
        while (!queue.empty()) {
            Node* node = queue.front();
            queue.pop();
            if (node->left != nullptr) {
                queue.push(node->left);
            }
            if (node->right != nullptr) {
                queue.push(node->right);
            }
            delete node;
        }
        _root = nullptr;
    }

private:
    Node* _root;
};

} //namespace Containers
} //namespace STC
#endif