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
#include<iostream>
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
    const Tree<int> tree1(5);
    const Tree<int> tree2(tree1.insert(6));
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


BOOST_AUTO_TEST_CASE(test_tree_height)
{
    // test tree height
    const int inserts[] = {
           5,            15,
        2,    7,     12,    17,
      1, 3, 6, 8,  11, 13, 16, 18
    };
    const size_t heights[] = {
        2,               2,
       3,     3,     3,     3,
      4, 4,  4, 4,  4, 4,  4,  4
    };
    size_t len = sizeof(inserts)/sizeof(inserts[0]);
    BOOST_REQUIRE( len == sizeof(heights)/sizeof(heights[0]) );

    Option<Tree<int>> tree(Some(Tree<int>(10)));
    for (size_t i=0; i<len; ++i) {
        tree = tree->insert(inserts[i]);
        BOOST_CHECK_MESSAGE(tree->height() == heights[i],
                            "tree height " << tree->height()
                            << " should equal expected height "
                            << heights[i]);
    }
}

BOOST_AUTO_TEST_CASE(test_tree_to_list)
{
    const int inserts[] = {5, 4, 8, 9, 1};
    Option<Tree<int>> tree(10);
    std::list<int> mylist;
    mylist.push_back(10);
    size_t len = sizeof(inserts)/sizeof(inserts[0]);
    for (size_t i=0; i<len; ++i) {
        tree = tree->insert(inserts[i]);
        mylist.push_back(inserts[i]);
    }
    mylist.sort();
    len += 1;
    std::list<int> treelist(tree->toList());
    BOOST_CHECK(mylist == treelist);
}

BOOST_AUTO_TEST_CASE(test_tree_equality)
{
    // test whether == works
    const int inserts[] = {5, 4, 8, 9, 1};
    const size_t len = sizeof(inserts)/sizeof(inserts[0]);
    Option<Tree<int>> tree1(Some(Tree<int>(10)));
    Option<Tree<int>> tree2(Some(Tree<int>(10)));
    BOOST_CHECK_MESSAGE(tree1.get_bare() == tree2.get_bare(),
                        "tree1 and tree2 should be equal");
    for (size_t i=0; i<len; i++) {
        tree1 = tree1->insert(inserts[i]);
        tree2 = tree2->insert(inserts[i]);
    }
    BOOST_CHECK_MESSAGE(tree1.get_bare() == tree2.get_bare(),
                        "tree1 and tree2 should be equal after insertions");
    tree1 = tree1->insert(20);
    BOOST_CHECK_MESSAGE(tree1.get_bare() != tree2.get_bare(),
                        "tree1 and tree2 should NOT be equal now");
}

BOOST_AUTO_TEST_CASE(test_tree_iterators)
{
    // test iterators
    const int inserts[] = {5, 4, 8, 9, 1};
    const size_t len = sizeof(inserts)/sizeof(inserts[0]);
    Option<Tree<int>> tree(Some(Tree<int>(10)));
    std::list<int> test_list;
    test_list.push_back(10);
    for (size_t i=0; i<len; ++i) {
        tree = tree->insert(inserts[i]);
        test_list.push_back(inserts[i]);
    }
    test_list.sort();
    std::list<int> out_list;
    for (Tree<int>::iterator it=tree->begin();
         it != tree->end();
         ++it) {
        out_list.push_back(*it);
    }

    if (test_list != out_list) {
        // print a more-helpful output if there's a failure
        std::cout << "exp\tact" << std::endl;
        typename std::list<int>::iterator it_test = test_list.begin();
        typename std::list<int>::iterator it_out = out_list.begin();
        while (it_test != test_list.end() || it_out != out_list.end()) {
            if (it_test != test_list.end()) {
                std::cout << *it_test;
                ++it_test;
            }
            std::cout << "\t";
            if (it_out != out_list.end()) {
                std::cout << *it_out;
                ++it_out;
            }
            std::cout << std::endl;
        }
        BOOST_ERROR("Expected list != Actual list");
    }
}


BOOST_AUTO_TEST_CASE(test_rebalance)
{
    // test rebalancing
    Option<Tree<int>> tree(Some(Tree<int>(0)));
    // test insertions
    for (int i=1; i<10; i++) {
        tree = tree->insert(i);
    }
    BOOST_CHECK_MESSAGE(tree->height() == 4,
                        "tree height of " << tree->height()
                        << " should equal 4");
    BOOST_CHECK(tree->is_balanced());
    // test removals
    for (int i=0; i<4; i++) {
        tree = tree->remove(i);
    }
    BOOST_CHECK_MESSAGE(tree->height() == 3,
                        "tree height of " << tree->height()
                        << " should be equal to 3");
    BOOST_CHECK(tree->is_balanced());
}
