#pragma once

#include <iostream> // cout
#include <memory>   // shared_ptr

#include "option.h"

template<typename T>
class Tree;

template<typename T>
void print_tree(Option<Tree<T>> tree);

template<typename T>
class Tree
{
public:
    Tree(const Tree<T>& head) :
        m_node(*head),
        m_child_left(head.m_child_left),
        m_child_right(head.m_child_right)
    {};
    Tree(T node) :
        m_node(node)
    {};

    Tree<T>& operator=(const Tree<T>& head)
    {
        m_node = head.m_node;
        m_child_left = head.m_child_left;
        m_child_right = head.m_child_right;
        return *this;
    }

    const T& min() const {
        if (m_child_left.is_some()) {
            return m_child_left->min();
        } else {
            return m_node;
        }
    }

    const T& max() const {
        if (m_child_right.is_some()) {
            return m_child_right->max();
        } else {
            return m_node;
        }
    }

    bool is_leaf() const {
        return left().is_none() &&
               right().is_none();
    }

    bool contains(const T& val) const
    {
        if (m_node == val) {
            return true;
        } else if (m_node > val) {
            // go left
            if (left().is_some()) {
                return left()->contains(val);
            } else {
                return false;
            }
        } else {
            // go right
            if (right().is_some()) {
                return right()->contains(val);
            } else {
                return false;
            }
        }
    }

    size_t size() const
    {
        size_t val = 1;
        if (m_child_left.is_some()) { val += m_child_left->size(); }
        if (m_child_right.is_some()) { val += m_child_right->size(); }
        return val;
    }

    Tree<T> insert(Option<Tree<T>> child) const
    {
        Tree<T> head(*this);
        if (child.is_some()) {
            std::shared_ptr<Tree<T>> value = child.get_ref();
            if (m_node > **value) {
                // left
                if (head.m_child_left.is_none()) {
                    head.m_child_left = child;
                } else {
                    head.m_child_left = head.m_child_left.get_ref()->insert(child);
                }
            } else {
                // right
                if (head.m_child_right.is_none()) {
                    head.m_child_right = child;
                } else {
                    head.m_child_right = head.m_child_right.get_ref()->insert(child);
                }
            }
            return head;
        } else {
            return Tree<T>(*this);
        }
    }

    Tree<T> insert(Tree<T> child) const
    {
        return insert(Some(child));
    }

    Tree<T> insert(T child) const
    {
        return insert(Some(Tree<T>(child)));
    }

    const Option<Tree<T>> remove(const T& node) const
    {
        Tree<T> head(*this);
        if (m_node == node) {
            // remove this node
            if (is_leaf()) {
                return None<Tree<T>>();
            } else if (head.left().is_some()) {
                // left side
                Option<Tree<T>> max_node = None<Tree<T>>();
                head.m_child_left = head.left()->popMax(max_node);
                // max_node gets our children
                max_node->m_child_left = head.m_child_left;
                max_node->m_child_right = head.m_child_right;
                return max_node;
            } else {
                // right side
                Option<Tree<T>> min_node = None<Tree<T>>();
                head.m_child_right = head.right()->popMin(min_node);
                // min_node gets our children
                min_node->m_child_left = head.m_child_left;
                min_node->m_child_right = head.m_child_right;
                return min_node;
            }
        } else if (m_node > node) {
            // go left
            if (head.m_child_left.is_none()) {
                return Some(*this);
            } else {
                head.m_child_left = head.m_child_left->remove(node);
                return Some(head);
            }
        } else {
            // go right
            if (head.m_child_right.is_none()) {
                return Some(*this);
            } else {
                head.m_child_right = head.m_child_right->remove(node);
                return Some(head);
            }
        }
    }

    T operator*() const { return deref(); }
    T deref() const { return m_node; }
    Option<Tree<T>> left() const { return m_child_left; };
    Option<Tree<T>> right() const { return m_child_right; };
private:

    const Option<Tree<T>> popMin(Option<Tree<T>>& min_node=None<Tree<T>>()) const
    {
        if (m_child_left.is_some()) {
            Tree<T> head(*this);
            head.m_child_left = head.m_child_left->popMin(min_node);
            return head;
        } else {
            // this is the min node
            min_node = Some(*this);
            return m_child_right;
        }
    }

    const Option<Tree<T>> popMax(Option<Tree<T>>& max_node=None<Tree<T>>()) const
    {
        if (m_child_right.is_some()) {
            Tree<T> head(*this);
            head.m_child_right = head.m_child_right->popMax(max_node);
            return head;
        } else {
            // this is the max node
            max_node = Some(*this);
            return m_child_left;
        }
    }

    const Option<Tree<T>> parentOf(const T& node) const
    {
        if (node == m_node) {
            return None<Tree<T>>();
        } else if (m_node > node) {
            // go left
            if (left().is_none()) {
                return m_child_left;
            } else if (left()->deref() == node) {
                // found the parent (us)
                return *this;
            } else {
                // haven't found parent yet
                return left()->parentOf(node);
            }
        } else {
            // go right
            if (right().is_none()) {
                return m_child_right;
            } else if (right()->deref() == node) {
                // found the parent (us)
                return *this;
            } else {
                // haven't found parent yet
                return right()->parentOf(node);
            }
        }
    }


    T m_node;
    Option<Tree<T>> m_child_left;
    Option<Tree<T>> m_child_right;
};


template<typename T>
void print_tree(Option<Tree<T>> tree)
{
    using namespace std;
    if (tree.is_none()) {
        return;
    } else {
        print_tree(tree->left());
        cout << tree->deref() << endl;
        print_tree(tree->right());
    }
}




