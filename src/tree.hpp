#ifndef STC_SRC_TREE_HPP_INCLUDED
#define STC_SRC_TREE_HPP_INCLUDED

#include <stdexcept>
#include <queue>
#include <iostream>
#include <algorithm>

namespace STC {
namespace Containers {
namespace except {

class invalid_init_exception : public std::exception {

};

class null_node_access_exception : public std::exception {

};

class key_already_exists_exception : public std::exception {

};

} //namespace except

namespace {

struct Node {
    enum color_t { BLACK, RED } color;

    Node* left;
    Node* right;
    Node* parent;

    int key;

    Node(int key, color_t color = color_t::RED) 
        : color{ color }, key{ key }, left{ nullptr }, right{ nullptr }, parent{ nullptr } 
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
    void print()
    {
        _print("", _root, false);    
    }


    void insert(int key) {
        Node* node = new Node(key, Node::RED);
        if (_root == nullptr) {
            node->color = Node::BLACK;
            _root = node;
            return;
        }
        _insert(node, _root);
        _rebalance(node);
    }

private:
   void _print(const std::string& prefix, const Node* node, bool isLeft)
    {
        if( node != nullptr )
        {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──" );

            // print the value of the node
            std::cout << node->key << std::endl;

            // enter the next tree level - left and right branch
            _print( prefix + (isLeft ? "│   " : "    "), node->left, true);
            _print( prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

    void _insert(Node* node, Node* current_node) {
        if (node->key == current_node->key) {
            throw except::key_already_exists_exception();
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
            throw except::null_node_access_exception();
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
    
    void _rebalance(Node* inserted) noexcept {
        if (inserted == nullptr) {
            throw except::null_node_access_exception();
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
    }

private:
    Node* _root;
};

} //namespace Containers
} //namespace STC
#endif