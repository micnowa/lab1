//
// Created by Michał Nowaliński on 19.10.2018.
//


#include <iostream>
#include <string>
#include <map>
#include "Ring.cpp"
#include "AVLTree.cpp"

using namespace std;

typedef AVLTree<int, string>::TreeIterator Iterator;


/**
 * Algorithm produces new Ring object based on two rings provided. It lookups the 1st and 2nd ring
 * in the way that it starts from star1 and start2 positions respectively, and gathers data. Lookup is
 * performed via iterators. It copies steps1 elements from the first ring and adds it in new ring, then
 * it copies steps2 elements form the second. Elements pointed by iterators are added in new ring.
 * Algorithm does that times times. If clockwise is set to false, then iterators are moved backwards
 * instead of forward.
 * @tparam K type of key
 * @tparam T type of info
 * @param ring1 first ring
 * @param start1 position from which algorithm start gathering data from 1st ring
 * @param steps1 how many elements are skipped every gathering in first ring
 * @param ring2 second ring
 * @param start2 position form which algorithm start gathering data from 2nd ring
 * @param steps2 how many elements are skipped every gathering in second ring
 * @param times how many times the algorithm shall run
 * @param clockwise if true algorithm lookups through ring clockwise, if false counterclockwise
 * @return Ring object created on basis of running algorithm
 */
template<typename K, typename T>
Ring<K, T>
produce(Ring<K, T> ring1, int start1, int steps1, Ring<K, T> ring2, int start2, int steps2, int times, bool clockwise1,
        bool clockwise2, bool begin) {
    if (ring1.getHead() == nullptr || ring2.getHead() == nullptr) return Ring<K, T>();
    auto *ring = new Ring<K, T>();
    typename Ring<K, T>::RingIterator it1 = ring1.begin();
    typename Ring<K, T>::RingIterator it2 = ring2.begin();

    it1 + start1;
    it2 + start2;
    int i = 0;
    while (i != times) {
        for (int j = 0; j < steps1; j++) {
            if (begin) ring->addBegin(it1.getKey(), it1.getInfo());
            else ring->addEnd(it1.getKey(), it1.getInfo());
            if (clockwise1) it1++;
            else it1--;
        }
        for (int j = 0; j < steps2; j++) {
            if (begin) ring->addBegin(it2.getKey(), it2.getInfo());
            else ring->addEnd(it2.getKey(), it2.getInfo());
            if (clockwise2) it2++;
            else it2--;
        }
        i++;
    }

    return *ring;
}


int main() {
    auto *tree = new AVLTree<int, string>();
    auto *treeRemovedOddElements = new AVLTree<int, string>();
    int n = 15;
    string plants[15] = {"arbuz", "banan", "cytryna", "dynia", "eukaliptus", "fasola", "groszek", "hiacynt", "irys",
                         "jablko", "koper", "lilia", "mango", "nektarynka", "orzech"};
    for (int i = 0; i < n; i++) {
        tree->insert(i + 1, plants[i]);
        treeRemovedOddElements->insert(i + 1, plants[i]);
    }
    for (int i = 0; i < n; i += 2) {
        treeRemovedOddElements->remove(i + 1);
    }


    tree->print();
    cout << "**************************" << endl;
    treeRemovedOddElements->print();

    Iterator it_first = tree->begin();
    Iterator it_first_last = tree->last();


    // Test of removing and iterator ++
    it_first++;
    for (Iterator it = treeRemovedOddElements->begin(); it != treeRemovedOddElements->end(); it++) {
        if (it->value == it_first->value && it->key == it_first->key) {
            cout << it->key << "    " << it->value << " Elements corresponds" << endl;
        } else throw std::invalid_argument("eit key or value does not correspond, or iterator is not working");
        it_first++;
        it_first++;
    }

    cout << endl;


    // Test of removing and iterator --
    it_first_last--;
    for (Iterator it = treeRemovedOddElements->last(); it != treeRemovedOddElements->end(); it--) {
        if (it->value == it_first_last->value && it->key == it_first_last->key) {
            cout << it->key << "    " << it->value << ", Elements corresponds" << endl;
        } else throw std::invalid_argument("eit key or value does not correspond, or iterator is not working");
        it_first_last--;
        it_first_last--;
    }

    cout << endl;

    // Test of operator [] and looking for value of given key
    for (Iterator it = tree->begin(); it != tree->end(); it++) {
        int key = it->key;
        if ((*tree)[key] == it->value) {
            cout << it->key << "    " << it->value << ", Elements correspond" << endl;
        }
    }

    cout << endl;

    // Test of operator () and looking for key of given value
    int i = 0;
    for (Iterator it = tree->begin(); it != tree->end(); it++) {
        int key = (*tree)(plants[i]);
        if (key == it->key) {
            cout << it->key << "    " << it->value << ", Elements correspond" << endl;
        }
        i++;
    }

    cout << endl;
    // Test of copying constructor, operator== and operator=
    auto *newTreeCopyingConstructor = new AVLTree<int, string>(*tree);
    AVLTree<int, string> newTreeOperatorEquals = *newTreeCopyingConstructor;
    if(*tree == *tree) cout<<"Operator== works!"<<endl;
    if(*tree == *newTreeCopyingConstructor) cout<<"Copying Contructor works"<<endl;
    if(*tree == newTreeOperatorEquals) cout<<"Operator= works"<<endl;

    return 0;
}

