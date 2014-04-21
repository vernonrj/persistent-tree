/**
 * @file
 * @brief Persistent Tree
 *
 * Contains a persistent binary search tree implementation.
 * Shared Pointers are used to manage the tree's memory, allowing
 * for parts of the tree to be used. Immutability is heavily,
 * but not totally, enforced.
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
     * @brief returns the maximum height of the tree
     */
    size_t height() const;

    /**
     * @brief returns a new tree with node inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(Option<Tree<T>> node) const {
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
    Tree<T> insert(Tree<T> node) const {
        return insert(node.deref());
    }

    /**
     * @brief returns a new tree with node inserted into it.
     *
     * @note this is NOT an in-place operation.
     * The previous version of the tree is preserved.
     */
    Tree<T> insert(T node) const;

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
     * @brief internal constructor for supplying children
     */
    Tree(T node,
         Option<Tree<T>> left,
         Option<Tree<T>> right) :
        m_node(node),
        m_child_left(left),
        m_child_right(right)
    {};

    /**
     * @brief blocked copy assignment operator
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
size_t Tree<T>::height() const
{
    size_t hgt_left = (m_child_left.is_some() ? m_child_left->height() : 0);
    size_t hgt_right = (m_child_right.is_some() ? m_child_right->height() : 0);
    return std::max(hgt_left, hgt_right) + 1;
}


template<typename T>
Tree<T> Tree<T>::insert(T node) const
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
        Tree<T> head(m_node,
                     m_child_left->popMax(max_node),
                     m_child_right);
        // max_node gets our children
        return Some(Tree<T>(max_node->deref(),
                            head.m_child_left,
                            head.m_child_right));
    } else {
        // At least has a right side.
        // Use minimum from right side to replace this node
        Option<Tree<T>> min_node = None<Tree<T>>();
        Tree<T> head(m_node,
                     m_child_left,
                     m_child_right->popMin(min_node));
        // min_node gets our children
        return Some(Tree<T>(min_node->deref(),
                            head.m_child_left,
                            head.m_child_right));
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



