/**
 * @file
 * @brief Persistent Tree
 */

#pragma once

#include <iostream> // cout
#include <memory>   // shared_ptr

#include "option.h"

template<typename T>
class Tree;

template<typename T>
void print_tree(Option<Tree<T>> tree);


/**
 * @brief Persistent Tree
 *
 * Defines a persistent Tree. Trees that are created are not modifiable.
 * You can use the Option class as a container to allow "in-place" modification,
 * similar to clojure's refs (but without the transaction).
 *
 * @note The tree structure always has at least one node. This is because the
 * tree's node cannot itself be empty. Removal returns an Option<Tree>, so when
 * the final element is removed, None is returned.
 *
 * @see Option
 */
template<typename T>
class Tree
{
public:
    /**
     * @brief copy constructor
     *
     * Creates a copy of an existing tree
     */
    Tree(const Tree<T>& head);

    /**
     * @brief creates a new tree from a node
     */
    Tree(T node);

    /**
     * @brief in-place copy assignment operator
     *
     * This is the only public in-place operation. Use it sparingly on
     * shared trees.
     *
     * @param head new node value and children to be loaded in
     */
    Tree<T>& operator=(const Tree<T>& head);

    /**
     * @brief return a reference to the minimum value in the tree.
     *
     * @note behavior is undefined if the min is used after the
     * tree (or node) is destroyed
     */
    const T& min() const;

    /**
     * @brief return a reference to the maximum value in the tree.
     *
     * @note behavior is undefined if the max is used after the
     * tree (or node) is destroyed
     */
    const T& max() const;

    /**
     * @brief returns true if this node has no children
     */
    inline bool is_leaf() const {
        return m_child_left.is_none() && m_child_right.is_none();
    }

    /**
     * @brief returns True if val exists in this node or any children
     */
    bool contains(const T& val) const;

    /**
     * @brief returns the number of elements in the tree
     */
    size_t size() const;

    /**
     * @brief returns a new tree with child inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(Option<Tree<T>> child) const;

    /**
     * @brief returns a new tree with child inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(Tree<T> child) const;

    /**
     * @brief returns a new tree with child inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(T child) const;

    /**
     * @brief returns a new tree with node removed from it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    const Option<Tree<T>> remove(const T& node) const;

    /**
     * @brief returns a copy of the contained value
     */
    inline T operator*() const { return deref(); }

    /**
     * @brief returns a copy of the contained value
     */
    inline T deref() const { return m_node; }

    /**
     * @brief returns an Option containing the left subtree,
     * which could be a valid tree, or empty.
     */
    inline Option<Tree<T>> left() const { return m_child_left; };

    /**
     * @brief returns an Option containing the right subtree,
     * which could be a valid tree, or empty.
     */
    inline Option<Tree<T>> right() const { return m_child_right; };

private:

    /**
     * @brief Return a new tree with the minimum value removed.
     *
     * @param[out] min_node the node containing the
     * minimum value that was removed
     */
    const Option<Tree<T>> popMin
    (
        Option<Tree<T>>& min_node=None<Tree<T>>()
    ) const;

    /**
     * @brief Return a new tree with the maximum value removed.
     *
     * @param[out] min_node the node containing the
     * maximum value that was removed
     */
    const Option<Tree<T>> popMax
    (
        Option<Tree<T>>& max_node=None<Tree<T>>()
    ) const;

    // Private members

    /// the contained value
    T m_node;

    /// The left subtree
    Option<Tree<T>> m_child_left;

    /// The right subtree
    Option<Tree<T>> m_child_right;
};


/**
 * @brief print the nodes in the tree
 */
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





template<typename T>
Tree<T>::Tree(const Tree<T>& head) :
    m_node(*head),
    m_child_left(head.m_child_left),
    m_child_right(head.m_child_right)
{};

template<typename T>
Tree<T>::Tree(T node) :
    m_node(node)
{};



template<typename T>
Tree<T>& Tree<T>::operator=(const Tree<T>& head)
{
    m_node = head.m_node;
    m_child_left = head.m_child_left;
    m_child_right = head.m_child_right;
    return *this;
}

template<typename T>
const T& Tree<T>::min() const
{
    if (m_child_left.is_some()) {
        return m_child_left->min();
    } else {
        return m_node;
    }
}

template<typename T>
const T& Tree<T>::max() const
{
    if (m_child_right.is_some()) {
        return m_child_right->max();
    } else {
        return m_node;
    }
}


template<typename T>
bool Tree<T>::contains(const T& val) const
{
    if (m_node == val) {
        return true;
    } else if (m_node > val) {
        // go left
        if (m_child_left.is_some()) {
            return m_child_left->contains(val);
        } else {
            return false;
        }
    } else {
        // go right
        if (m_child_right.is_some()) {
            return m_child_right->contains(val);
        } else {
            return false;
        }
    }
}


template<typename T>
size_t Tree<T>::size() const
{
    size_t val = 1;
    if (m_child_left.is_some()) { val += m_child_left->size(); }
    if (m_child_right.is_some()) { val += m_child_right->size(); }
    return val;
}


template<typename T>
Tree<T> Tree<T>::insert(Option<Tree<T>> child) const
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

template<typename T>
Tree<T> Tree<T>::insert(Tree<T> child) const
{
    return insert(Some(child));
}

template<typename T>
Tree<T> Tree<T>::insert(T child) const
{
    return insert(Some(Tree<T>(child)));
}


template<typename T>
const Option<Tree<T>> Tree<T>::remove(const T& node) const
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




// Private methods



template<typename T>
const Option<Tree<T>> Tree<T>::popMin(Option<Tree<T>>& min_node) const
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


template<typename T>
const Option<Tree<T>> Tree<T>::popMax(Option<Tree<T>>& max_node) const
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



