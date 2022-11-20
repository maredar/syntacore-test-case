#ifndef STC_SRC_TREE_HPP_INCLUDED
#define STC_SRC_TREE_HPP_INCLUDED

#include <stdexcept>
#include <queue>
#include <iostream>

namespace Containers {
namespace except {

class invalid_init_exception : public std::exception {

};

class null_node_access_exception : public std::exception {

};

} //namespace except

namespace {

struct Node {
    enum color_t { BLACK, RED } color;

    Node* left;
    Node* right;

    int key;
    unsigned childs_counter;

    Node(int key, color_t color = color_t::RED, unsigned childs_counter = 0) 
        : color{ color }, key{ key }, childs_counter{ childs_counter },
            left{ nullptr }, right{ nullptr } 
    {
        //empty
    }
};

} //namespace

/*
* Self-balancing red-black tree with additional 
* uint variable in each node to count how many 
* childs each node has
*/
class RBTree {
public:
    RBTree() : _root{ nullptr } {
        //empty
    }

    ~RBTree() {
        _clear_tree();
    }

public:
    void print() noexcept {
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
            std::cout << node->key << std::endl;
        }
    }

    void insert(int key) {
        Node* node = new Node(key, Node::RED);
        if (_root == nullptr) {
            node->color = Node::BLACK;
            _root = node;
            return;
        }
        _insert(node, _root);
    }

private:
    void _insert(Node* node, Node* current_node) {
        if (node->key < current_node->key) {
            if (current_node->left == nullptr) {
                current_node->left = node;
            } 
            else {
                _insert(node, current_node->left);
            }
        }
        else {
            if (current_node->right == nullptr) {
                current_node->right = node;
            }
            else {
                _insert(node, current_node->right);
            }
        }
    }

    void _recolor(Node* node) {
        if (node == nullptr) {
            throw except::null_node_access_exception();
        }
        if (node->color == Node::RED) {
            node->color = Node::BLACK;
        }
        else {
            node->color = Node::RED;
        }
    }

    void _rotate_left(Node* rotation_root) {
        if (rotation_root == nullptr) {
            throw except::null_node_access_exception();
        }
        if (rotation_root->right == nullptr) {
            throw except::null_node_access_exception();
        }
        Node* new_root = rotation_root->right;
        rotation_root->right = new_root->left;
        new_root->left = rotation_root;
    }

    void _rotate_right(Node* rotation_root) {
        if (rotation_root == nullptr) {
            throw except::null_node_access_exception();
        }
        if (rotation_root->left == nullptr) {
            throw except::null_node_access_exception();
        }
        Node* new_root = rotation_root->left;
        rotation_root->left = new_root->right;
        new_root->right = rotation_root;
    }
    
    void _rebalance() noexcept {

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
    }

private:
    Node* _root;
};

} //namespace Containers

#endif