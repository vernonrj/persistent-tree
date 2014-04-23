/**
 * @file
 * @brief Persistent Tree
 *
 * Contains a persistent binary search tree implementation.
 * Shared Pointers are used to manage the tree's memory, allowing
 * for parts of the tree to be used.
 * No balancing algorithm is currently implemented.
 *
 *
 * Copyright (C) 2014  Vernon R. Jones
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <list>     // used for iterators
#include <memory>   // shared_ptr

#include "option.h"

template<typename T> class TreeIter;

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
    Tree(const T node);

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
    inline size_t size() const { return m_size; };

    /**
     * @brief returns the maximum height of the tree
     */
    size_t height() const;

    /**
     * @brief returns a new tree with node inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(const Option<Tree<T>> node) const {
        if (node.is_some()) {
            return insert(node->deref());
        } else {
            // empty node? Just return this tree
            return Tree<T>(*this);
        }
    }

    /**
     * @brief returns a new tree with node inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(const Tree<T> node) const {
        return insert(node.deref());
    }

    /**
     * @brief returns a new tree with node inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(const T node) const;

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
    inline const T operator*() const { return deref(); }

    /**
     * @brief returns a copy of the contained value
     */
    inline const T& deref() const { return m_node; }

    /**
     * @brief returns an Option containing the left subtree,
     * which could be a valid tree, or empty.
     */
    inline const Option<Tree<T>> left() const { return m_child_left; };

    /**
     * @brief returns an Option containing the right subtree,
     * which could be a valid tree, or empty.
     */
    inline const Option<Tree<T>> right() const { return m_child_right; };

    std::list<T> toList() const {
        std::list<T> l;
        if (m_child_left.is_some()) {
            l.splice(l.end(), m_child_left->toList());
        }
        l.push_back(m_node);
        if (m_child_right.is_some()) {
            l.splice(l.end(), m_child_right->toList());
        }
        return l;
    }

    bool operator==(const Tree<T>& rhs) const {
        if (rhs.size() != size()) {
            return false;
        } else {
            std::list<T> us(toList());
            std::list<T> them(rhs.toList());
            typename std::list<T>::iterator it_us=us.begin();
            typename std::list<T>::iterator it_them=them.begin();
            for (;
                 it_us != us.end() && it_them != them.end();
                 ++it_us, ++it_them) {
                if (*it_us != *it_them) {
                    return false;
                }
            }
            return true;
        }
    }

    inline bool operator!=(const Tree<T>& rhs) const {
        return !operator==(rhs);
    }

    class TreeIter;
    typedef TreeIter iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    iterator begin() const { return iterator(*this, 0); };
    iterator end() const { return iterator(*this, size()); };
private:

    /**
     * @brief internal constructor for supplying children
     */
    Tree(const T node,
         const Option<Tree<T>> left,
         const Option<Tree<T>> right) :
        m_node(node),
        m_child_left(left),
        m_child_right(right),
        m_size(tree_size(left) + tree_size(right))
    {};

    /**
     * @brief blocked copy assignment operator, not implemented
     */
    Tree<T>& operator=(const Tree<T>& head);

    /**
     * @brief remove this node from the tree, preserving any children
     */
    const Option<Tree<T>> removeThisNode() const;

    /**
     * @brief Return a new tree with the minimum value removed.
     *
     * @param[out] min_node the node containing the
     * minimum value that was removed
     */
    const Option<Tree<T>> popMin(Option<Tree<T>>& min_node=None<Tree<T>>()) const;

    /**
     * @brief Return a new tree with the maximum value removed.
     *
     * @param[out] min_node the node containing the
     * maximum value that was removed
     */
    const Option<Tree<T>> popMax(Option<Tree<T>>& max_node=None<Tree<T>>()) const;

    // Private members

    /// the contained value
    const T m_node;

    /// The left subtree
    const Option<Tree<T>> m_child_left;

    /// The right subtree
    const Option<Tree<T>> m_child_right;

    /// the number of nodes contained
    const size_t m_size;
};



template<typename T>
size_t tree_size(const Option<Tree<T>> tree)
{
    if (tree.is_none()) {
        return 0;
    } else {
        return tree_size(tree->left()) +
               1 +
               tree_size(tree->right());
    }
}






template<typename T>
Tree<T>::Tree(const Tree<T>& head) :
    m_node(*head),
    m_child_left(head.m_child_left),
    m_child_right(head.m_child_right),
    m_size(tree_size(head.m_child_left) + 1 + tree_size(head.m_child_right))
{};


template<typename T>
Tree<T>::Tree(const T node) :
    m_node(node),
    m_size(1)
{};



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
size_t Tree<T>::height() const
{
    size_t hgt_left = (m_child_left.is_some() ? m_child_left->height() : 0);
    size_t hgt_right = (m_child_right.is_some() ? m_child_right->height() : 0);
    return std::max(hgt_left, hgt_right) + 1;
}


template<typename T>
Tree<T> Tree<T>::insert(const T node) const
{
    if (m_node > node) {
        // left
        if (m_child_left.is_none()) {
            // empty left side, at a leaf: do the insertion here
            return Tree<T>(m_node,
                           Some(Tree<T>(node)),
                           m_child_right);
        } else {
            // non-empty left side, continue recursing to a leaf
            std::shared_ptr<Tree<T>> lchld = m_child_left.get_ref();
            return Tree<T>(m_node,
                           Some(lchld->insert(node)),
                           m_child_right);
        }
    } else {
        // right
        if (m_child_right.is_none()) {
            // empty right side, at a leaf: do the insertion here
            return Tree<T>(m_node,
                           m_child_left,
                           Some(Tree<T>(node)));
        } else {
            // non-empty right side, continue recursing to a leaf
            std::shared_ptr<Tree<T>> rchld = m_child_right.get_ref();
            return Tree<T>(m_node,
                           m_child_left,
                           rchld->insert(node));
        }
    }
}


template<typename T>
const Option<Tree<T>> Tree<T>::remove(const T& node) const
{
    if (m_node == node) {
        return removeThisNode();
    } else if (m_node > node) {
        // go left to find the node to remove
        if (m_child_left.is_none()) {
            // oops, node doesn't exist in the tree!
            // don't throw an error, just act normal
            return Some(*this);
        } else {
            // continue recursing to the left
            Tree<T> head(m_node,
                         m_child_left->remove(node),
                         m_child_right);
            return Some(head);
        }
    } else {
        // go right to find the node to remove
        if (m_child_right.is_none()) {
            // oops, node doesn't exist in the tree!
            // don't throw an error, you didn't see nuthin
            return Some(*this);
        } else {
            // continue recursing to the right
            Tree<T> head(m_node,
                         m_child_left,
                         m_child_right->remove(node));
            return Some(head);
        }
    }
}




// Private methods


template<typename T>
const Option<Tree<T>> Tree<T>::removeThisNode() const
{
    // remove this node
    if (is_leaf()) {
        // Has neither left side or right side,
        // so the node can be safely removed
        return None<Tree<T>>();
    } else if (m_child_left.is_some()) {
        // At least has a left side.
        // Use maximum from left side to replace this node
        Option<Tree<T>> max_node = None<Tree<T>>();
        Option<Tree<T>> lchld = m_child_left->popMax(max_node);
        // max_node gets our children
        return Some(Tree<T>(max_node->deref(),
                            lchld,
                            m_child_right));
    } else {
        // At least has a right side.
        // Use minimum from right side to replace this node
        Option<Tree<T>> min_node = None<Tree<T>>();
        Option<Tree<T>> rchld = m_child_right->popMin(min_node);
        // min_node gets our children
        return Some(Tree<T>(min_node->deref(),
                            m_child_left,
                            rchld));
    }
}


template<typename T>
const Option<Tree<T>> Tree<T>::popMin(Option<Tree<T>>& min_node) const
{
    if (m_child_left.is_some()) {
        return Some(Tree<T>(m_node,
                            m_child_left->popMin(min_node),
                            m_child_right));
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
        return Some(Tree<T>(m_node,
                            m_child_left,
                            m_child_right->popMax(max_node)));
    } else {
        // this is the max node
        max_node = Some(*this);
        return m_child_left;
    }
}






/**
 * @brief Iterator for Tree data structure
 */
template<typename T>
class Tree<T>::TreeIter
{
public:
    /**
     * @brief constructor for tree iterator
     */
    TreeIter(const Tree& tree, size_t index) :
        m_tree(tree),
        m_index(index),
        m_size(tree.size())
    {};

    /**
     * @brief increment iterator
     */
    Tree<T>::iterator& operator++() {
        m_index++;
        return *this;
    }

    /**
     * @brief dereference iterator
     */
    const T operator*() const {
        return deref(m_tree, 0);
    }

    /**
     * @brief return true if this iterator is equal to another iterator
     */
    bool operator==(const Tree<T>::TreeIter& rhs) const {
        return m_tree == rhs.m_tree && m_index == rhs.m_index;
    }

    /**
     * @brief return false if this iterator is not equal to another iterator
     */
    bool operator!=(const Tree<T>::TreeIter& rhs) const {
        return !operator==(rhs);
    }

private:

    /**
     * @brief alternate dereference method
     */
    const T deref(Option<Tree<T>> tree, size_t place) const {
        if (m_index >= place + tree->size()) {
            return 0;
        }
        size_t left_size = tree_size(tree->left());
        // this is super inefficient
        if (m_index == (place + left_size)) {
            // found it
            return tree->deref();
        } else if (m_index < (place + left_size)) {
            return deref(tree->left(), place);
        } else {
            return deref(tree->right(), place + left_size + 1);
        }
    }

    /**
     * @brief Tree to refer to
     */
    const Tree<T>& m_tree;

    /**
     * @brief current index we're on
     */
    size_t m_index;

    /**
     * @brief size of tree
     */
    const size_t m_size;
};



