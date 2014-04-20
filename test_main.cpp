/**
 * @file
 * @brief Persistent Tree Unit Tests
 *
 * Contains unit tests for the Option Type as well as the Persistent
 * Tree. on invoking make all, run ./test to run these tests
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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Trees
#include<boost/test/unit_test.hpp>

// link with -lboost_unit_test_framework
#include "option.h"
#include "tree.h"

BOOST_AUTO_TEST_CASE(test_option_some)
{
    // Test Some
    BOOST_CHECK( Some<int>(5).is_some() );
    Option<int> i = Some<int>(5);
    Option<int> i2(i);
    BOOST_CHECK( i2.is_some() );
    BOOST_CHECK( !i2.is_none() );
}


BOOST_AUTO_TEST_CASE(test_option_none)
{
    // test None
    BOOST_CHECK( None<int>().is_none() );
    Option<int> i = None<int>();
    BOOST_CHECK( i.is_none() );
    BOOST_CHECK( !i.is_some() );
}


BOOST_AUTO_TEST_CASE(test_option_deref_none)
{
    // Test dereferencing options
    Option<int> i = None<int>();
    try {
        // should throw an exception when dereferencing,
        // since it is a None
        i.get_ref();
        // oops! didn't throw
        BOOST_FAIL( "Deref of None should have thrown exception" );
    } catch (...) {
        // exception got thrown correctly
    }
}


BOOST_AUTO_TEST_CASE(test_tree_create)
{
    // Test tree creation
    Tree<int> tree(5);
    BOOST_CHECK( tree.deref() == 5 );
    BOOST_CHECK( tree.contains(5) );
}


BOOST_AUTO_TEST_CASE(test_tree_insert)
{
    // Test Tree insertion
    Tree<int> tree1(5);
    Tree<int> tree2(tree1.insert(6));
    // trees are persistent data structures tree1 should not have
    // been modified by the insert()
    BOOST_CHECK( !tree1.contains(6) );
    // but tree2 should have the new element (obviously)
    BOOST_CHECK( tree2.contains(6) );
    Option<Tree<int>> opt_tree(Some(Tree<int>(5)));
    const int inserts[] = { 6, 0, 1, 4 };
    const size_t len = sizeof(inserts)/sizeof(inserts[0]);
    for (size_t i=0; i<len; ++i) {
        opt_tree = opt_tree->insert(inserts[i]);
        BOOST_CHECK( opt_tree->contains(inserts[i]) );
        BOOST_CHECK_MESSAGE( opt_tree->size() == i + 2,
                             "Tree size " << opt_tree->size()
                             << " is not equal to size " << i + 2 );
    }
    for (size_t i=0; i<len; ++i) {
        BOOST_CHECK( opt_tree->contains(inserts[i]) );
    }
}


BOOST_AUTO_TEST_CASE(test_tree_remove)
{
    // test Tree Removal
    Option<Tree<int>> tree(Some(Tree<int>(5)));
    const int inserts[] = { 6, 0, 1, 4, 8, 10, 3 };
    const size_t len = sizeof(inserts)/sizeof(inserts[0]);
    for (size_t i=0; i<len; ++i) {
        tree = tree->insert(inserts[i]);
    }
    for (size_t i=0; i<len; ++i) {
        for (size_t j=i; j<len; ++j) {
            BOOST_CHECK( tree->contains(inserts[i]) );
        }
        tree = tree->remove(inserts[i]);
        BOOST_CHECK_MESSAGE( !tree->contains(inserts[i]),
                             "tree shouldn't contain " << inserts[i]);
        BOOST_CHECK_MESSAGE( tree->size() == len - i,
                             "Tree size " << tree->size()
                             << " is not equal to size " << len - i );
    }
}


BOOST_AUTO_TEST_CASE(test_tree_min_max)
{
    // test mins and maxes
    Option<Tree<int>> tree(Some(Tree<int>(5)));
    const int inserts[] = { 6, 4, 1, 0, 8, 10, 3 };
    const size_t len = sizeof(inserts)/sizeof(inserts[0]);
    const int maxes[] = { 6, 6, 6, 6, 8, 10, 10 };
    const int mins[] = {5, 4, 1, 0, 0, 0, 0 };
    for (size_t i=0; i<len; ++i) {
        tree = tree->insert(inserts[i]);
        BOOST_CHECK_MESSAGE( tree->max() == maxes[i],
                             "Tree max (" << tree->max()
                             << ") should be " << maxes[i] );
        BOOST_CHECK_MESSAGE( tree->min() == mins[i],
                             "Tree min (" << tree->min()
                             << ") should be " << mins[i] );
    }
}


