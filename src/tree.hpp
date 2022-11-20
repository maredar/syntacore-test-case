#ifndef STC_SRC_TREE_HPP_INCLUDED
#define STC_SRC_TREE_HPP_INCLUDED

#include <stdexcept>
#include <queue>

namespace Containers {
namespace except {

class invalid_init_exception : public std::exception {

};

} //namespace except

namespace {

struct Node {
    Node* left;
    Node* right;

    int key;

    bool is_black;
    int childs_counter;

    Node(int key, bool is_black, int childs_counter = 0) 
        : is_black{ is_black }, key{ key }, childs_counter{ childs_counter },
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
    RBTree() : _root{ new Node(0, true, 0) } {
        //empty
    }

    ~RBTree() {
        _clear_tree();
    }

private:
    void _swap() noexcept {

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
            Node* temp = queue.front();
            queue.pop();
            if (temp->left != nullptr) {
                queue.push(temp->left);
            }
            if (temp->right != nullptr) {
                queue.push(temp->right);
            }
            delete temp;
        }
    }

private:
    Node* _root;
};

} //namespace Containers

#endif