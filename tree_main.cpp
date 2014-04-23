#include<iostream>

#include "tree.h"

/**
 * @brief print the nodes in the tree
 */
template<typename T>
void print_tree(const Option<Tree<T>> tree)
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
bool print_tree_levels(const Option<Tree<T>> tree, int level=-1)
{
    using namespace std;
    if (tree.is_none()) {
        cout << "  ";
        return false;
    } else if (level < 0) {
        cout << "printing tree levels" << endl;
        for (;;) {
            cout << "Level " << level + 1 << ": ";
            if (!print_tree_levels(tree, ++level)) {
                cout << endl;
                return false;
            }
            cout << endl;
        }
    } else if (level == 0) {
        cout << tree->deref() << " ";
        return true;
    } else {
        bool more_to_do = print_tree_levels(tree->left(), level-1);
        more_to_do |= print_tree_levels(tree->right(), level-1);
        return more_to_do;
    }
}

int
main(void)
{
    using namespace std;
    Option<Tree<int>> tree(5);
    cout << "tree size (should be 1): " << tree->size() << endl;

    Option<Tree<int>> mod(tree->insert(4));

    cout << "mod (size should be 2): " << mod->size() << endl;
    print_tree(mod);
    mod = mod->insert(7);
    mod = mod->insert(10);
    mod = mod->insert(0);

    cout << "mod" << endl;
    print_tree_levels(mod);
    cout << endl << "STATS:" << endl;
    cout << "size: " << mod->size()
         << " height: " << mod->height() << endl;

    cout << "min: " << mod->min() << endl;
    cout << "max: " << mod->max() << endl;
    cout << endl;

    cout << "removed (5):" << endl;
    print_tree(mod->remove(5));

    cout << "removed (7):" << endl;
    print_tree(mod->remove(7));

    cout << "removed (0):" << endl;
    print_tree(mod->remove(0));

    cout << "removed (3: non-elem):" << endl;
    print_tree(mod->remove(3));

    cout << "same?" << endl;
    print_tree(mod);

    cout << "tree contains 5? " << mod->contains(5) << endl;
    cout << "tree contains 3? " << mod->contains(3) << endl;

    cout << "tree is same?" << endl;
    print_tree(tree);

    Option<Tree<int>> unbal(Tree<int>(0));
    for (int i=1; i<10; i++) {
        unbal = unbal->insert(i);
    }
    print_tree_levels(unbal);
    unbal = unbal->balance();
    cout << "balanced now? " << unbal->is_balanced() << endl;
    print_tree_levels(unbal);
    unbal = unbal->remove(0);
    unbal = unbal->remove(1);
    unbal = unbal->remove(2);
    unbal = unbal->remove(3);
    cout << "balanced now? " << unbal->is_balanced() << endl;
    print_tree_levels(unbal);
}

