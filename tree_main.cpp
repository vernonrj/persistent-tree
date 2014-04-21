#include<iostream>

#include "tree.h"

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
    print_tree(mod);

    cout << "min: " << mod->min() << endl;
    cout << "max: " << mod->max() << endl;

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
}

