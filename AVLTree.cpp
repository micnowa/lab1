//
// Created by Michał Nowaliński on 20-Dec-18.
//

#include<iostream>
#include <iomanip>

using namespace std;


/**
 * AVL Tree is an extension to Binary Search Tree. Nodes are added and kept in the same way
 * as in BST(Binary Search Tree). AVL is found to be significant improvement to BST. It is
 * called self-organising BST, due to the fact it is balanced. By that we understand that
 * we avoid situation when data is structured into a list. Balanced in that case means that
 * every two subtrees whose root is same node have heights different at the most by 1. It
 * is achieved via adding operation of balancing after adding or removing a node.
 * @tparam t1 type of key in nodes, it needs to have overwritten operators: >, <, =, ==, !=
 * @tparam t2 type of value in nodes, it needs to have overwritten operators: >, <, =, ==, !=
 */
template<typename t1, typename t2>
class AVLTree {
    /**
     * Structure symbolizing a node in the trees
     */
    struct Node {
        /**
         * key in Node
         */
        t1 key;
        /**
         * value in Node
         */
        t2 value;
        /**
         * parent of the node
         */
        Node *parent;
        /**
         * left subtree root
         */
        Node *left;
        /**
         * right subtree root
         */
        Node *right;
        /**
         * height of tree
         */
        int height;
    };

    /**
     * Root node
     */
    Node *root;

    /**
     * Clears tree
     * @param node root of tree
     */
    void makeEmpty(Node *node) {
        if (node == NULL) return;
        makeEmpty(node->left);
        makeEmpty(node->right);
        delete node;
    }

    /**
     * Creates new node with given key and value
     * @param key key of new node
     * @param value value of new node
     * @return pointer to new node
     */
    Node *newNode(t1 key, t2 value) {
        Node *temp = new Node;
        temp->key = key;
        temp->value = value;
        temp->left = temp->right = NULL;
        temp->parent = NULL;
        return temp;
    }

     /**
      * Looks for node with given key
      * @param node node from which recursive searching is performed
      * @param key key to be looked for
      * @return node with given key
      */
    Node *findKey(Node *node, t1 key) {
        if (node == nullptr) return nullptr;
        else if (node->key == key) return node;
        else if (key > node->key) return findKey(node->right, key);
        return findKey(node->left, key);
    }

    /**
     * Looks for node with given value
     * @param node node from which recursive searching is performed
     * @param value value to be looked for
     * @return node with given value
     */
    Node *findValue(Node *node, t2 value) {
        if (node == nullptr) return nullptr;
        else if (node->value == value) return node;
        else if (value > node->value) return findValue(node->right, value);
        return findValue(node->left, value);
    }


    /**
     * Inserts data into tree and performs balancing
     * @param key data to be inserted
     * @param node root ot tree
     * @return node with data that was created
     */
    Node *insert(t1 key, t2 value, Node *node) {
        if (node == NULL) {
            node = newNode(key, value);
            node->key = key;
            node->value = value;
            node->height = 0;
            node->left = node->right = NULL;
        } else if (key < node->key) {
            Node *leftChild = insert(key, value, node->left);
            node->left = leftChild;
            leftChild->parent = node;
            if (height(node->left) - height(node->right) == 2) {
                if (key < node->left->key)
                    node = singleRightRotate(node);
                else
                    node = doubleRightRotate(node);
            }
        } else if (key > node->key) {
            Node *rightChild = insert(key, value, node->right);
            node->right = rightChild;
            rightChild->parent = node;
            if (height(node->right) - height(node->left) == 2) {
                if (key > node->right->key)
                    node = singleLeftRotate(node);
                else
                    node = doubleLeftRotate(node);
            }
        }

        node->height = max(height(node->left), height(node->right)) + 1;
        return node;
    }

    /**
     * Single right rotation of subtree
     * @param node node root of subtree
     * @return
     */
    Node *singleRightRotate(Node *&node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        if (tmp->right != nullptr) tmp->right->parent = node;
        tmp->parent = node->parent;
        tmp->right = node;
        node->parent = tmp;

        node->height = max(height(node->left), height(node->right)) + 1;
        tmp->height = max(height(tmp->left), node->height) + 1;
        return tmp;
    }

    /**
     * Single left rotation of subtree
     * @param node node root of subtree
     * @return node
     */
    Node *singleLeftRotate(Node *&node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        if (tmp->left != nullptr)
            tmp->left->parent = node;
        tmp->parent = node->parent;
        tmp->left = node;
        node->parent = tmp;


        node->height = max(height(node->left), height(node->right)) + 1;
        tmp->height = max(height(node->right), node->height) + 1;
        return tmp;
    }

    /**
     * Double left rotation of subtree
     * @param node node root of subtree
     * @return node
     */
    Node *doubleLeftRotate(Node *&node) {
        node->right = singleRightRotate(node->right);
        return singleLeftRotate(node);
    }

    /**
     * Double right rotation of subtree
     * @param node node
     * @return node
     */
    Node *doubleRightRotate(Node *&node) {
        node->left = singleLeftRotate(node->left);
        return singleRightRotate(node);
    }

    /**
     * Returns min value of subtree
     * @param node root of subtree
     * @return Min value of subtree
     */
    Node *findMin(Node *node) {
        if (node == NULL) return NULL;
        else if (node->left == NULL) return node;
        else return findMin(node->left);
    }

    /**
     * Returns max value of subtree
     * @param node root of subtree
     * @return Max value of subtree
     */
    Node *findMax(Node *node) {
        if (node == NULL) return NULL;
        else if (node->right == NULL) return node;
        else return findMax(node->right);
    }

    /**
     * Removes node with data from subtree
     * @param key data with which node shall be removed
     * @param node root of subtree
     * @return node on which the function stopped looking for wanted node
     */
    Node *remove(t1 key, t2 value, Node *node) {
        Node *temp;

        if (node == NULL) return NULL;
        else if (key < node->key) node->left = remove(key, value, node->left);
        else if (key > node->key) node->right = remove(key, value, node->right);

        else if (node->left && node->right) {
            temp = findMin(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = remove(node->key, node->value, node->right);
        } else {
            temp = node;
            if (node->left == NULL) node = node->right;
            else if (node->right == NULL) node = node->left;
            delete temp;
        }
        if (node == NULL) return node;

        node->height = max(height(node->left), height(node->right)) + 1;

        if (height(node->left) - height(node->right) == 2) {
            if (height(node->left->left) - height(node->left->right) == 1) return singleLeftRotate(node);
            else return doubleLeftRotate(node);
        } else if (height(node->right) - height(node->left) == 2) {
            if (height(node->right->right) - height(node->right->left) == 1) return singleRightRotate(node);
            else return doubleRightRotate(node);
        }
        return node;
    }

    /**
     * Returns height of given subtree or -1 if root is equal to null pointer
     * @param node root of subtree
     * @return height of given subtree or -1 if root is equal to null pointer
     */
    int height(Node *node) {
        return node == NULL ? -1 : node->height;
    }

    /**
     * Return balance of given node, which means difference between height of right and left subtrees
     * @param node node whoose balance is checked
     * @return balance
     */
    int getBalance(Node *node) {
        return node == NULL ? 0 : height(node->left) - height(node->right);
    }

    /**
     * Prints subtree to standard output
     * @param node root of subtree
     * @param space indent between data printed
     */
    void print(Node *node, int space) {
        int COUNT = 10;
        if (node == NULL) return;
        space += COUNT;
        print(node->right, space);

        printf("\n");
        for (int i = COUNT; i < space; i++) printf(" ");
        cout << node->key << "  " << node->value << endl;
        print(node->left, space); // Process left child
    }

public:
    template<typename K, typename I>
    class Iterator {
        Node *it;
    public:
        /**
         * Default constructor
         */
        Iterator() { it = nullptr; }

        /**
         * Constructor with element iterator points to
         * @param node
         */
        Iterator(Node *element) {
            it = element;
        }

        /**
         * Destructor
         */
        ~Iterator() {}

        /**
         * Copying constructor
         * @param cc iterator to be copied
         */
        Iterator(const Iterator &cc) { it = cc.it; }


        /**
         * Overwritten operator =
         * @param iterator iterator ro be asigned
         * @return object
         */
        Iterator operator=(const Iterator &iterator) {
            if (this == &iterator) return *this;
            it = iterator.it;
            return *this;
        }

        /**
         * Overwritten operator +. It moves iterator by length position forwards
         * @param length number by which iterator is moved forwards
         * @return iterator
         */
        Iterator operator+(int length) {
            if (length > 0)for (int i = 0; it && it->next && i < length; i++) it = it->next;
            return *this;
        }

        /**
         * Overwritten operator +. It moves iterator by length position backword
         * @param length number by which iterator is moved backword
         * @return iterator
         */
        Iterator operator-(int length) {
            if (length > 0)for (int i = 0; it && it->next && i < length; i++) it = it->prev;
            return *this;
        }


        /**
         * Overwritten operator ++. Moves forward by one
         * @return iterator
         */
        Iterator &operator++() {
            Node *p;

            if (it == nullptr) {
                if (it == nullptr) return nullptr;
                while (it->left != nullptr) {
                    it = it->left;
                }
            } else if (it->right != nullptr) {
                it = it->right;

                while (it->left != nullptr) {
                    it = it->left;
                }
            } else {
                p = it->parent;
                while (p != nullptr && it == p->right) {
                    it = p;
                    p = p->parent;
                }
                it = p;
            }
            return *this;
        }

        /**
         * Overwritten operator++
         * @return next iterator
         */
        const Iterator<K, I> operator++(int) {
            if (!it) return *this;
            else {
                Node *p;
                auto *temporary = new Iterator(it);
                if (it == nullptr) {
                    while (it->left != nullptr) {
                        it = it->left;
                    }
                } else if (it->right != nullptr) {
                    it = it->right;

                    while (it->left != nullptr) {
                        it = it->left;
                    }
                } else {
                    p = it->parent;
                    while (p != nullptr && it == p->right) {
                        it = p;
                        p = p->parent;
                    }
                    it = p;
                }
                return *temporary;
            }
        }

        /**
         * Overwritten operator --. Moves backword by one
         * @return iterator
         */
        Iterator &operator--() {
            Node *p;

            if (it == nullptr) {
                if (it == nullptr) return nullptr;
                while (it->right != nullptr) {
                    it = it->right;
                }
            } else if (it->left != nullptr) {
                it = it->left;

                while (it->right != nullptr) {
                    it = it->right;
                }
            } else {
                p = it->parent;
                while (p != nullptr && it == p->left) {
                    it = p;
                    p = p->parent;
                }
                it = p;
            }
            return *this;
        }

        /**
         * Overwritten operator --. Moves backword by one
         * @return iterator
         */
        const Iterator<K, I> operator--(int) {
            if (!it) return *this;
            else {
                Node *p;
                auto *temporary = new Iterator(it);
                if (it == nullptr) {
                    while (it->right != nullptr) {
                        it = it->right;
                    }
                } else if (it->left != nullptr) {
                    it = it->left;

                    while (it->right != nullptr) {
                        it = it->right;
                    }
                } else {
                    p = it->parent;
                    while (p != nullptr && it == p->left) {
                        it = p;
                        p = p->parent;
                    }
                    it = p;
                }
                return *temporary;
            }
        }

        /**
         * Overwritten operator ==, compares to iterators
         * @param iterator iterator to be compared
         * @return true if iterators point to same object, false otherwise
         */
        bool operator==(Iterator iterator) const { return it == iterator.it; }

        /**
         * Overwritten operator ==, compares to iterators
         * @param iterator iterator to be compared
         * @return false if iterators point to same object, true otherwise
         */
        bool operator!=(Iterator iterator) const { return it != iterator.it; }

        /**
         * Overwritten operator *, return object via accessing pointer
         * @return iterator object
         */
        Node &operator*() const { return *it; }

        /**
         * Overwritten operator->. Used to access iterator pointer
         * @return iterator pointer
         */
        Node *operator->() const { return it; }

        /**
         * returns key
         * @return key
         */
        t1 getKey() { return it->key; }

        /**
         * returns value
         * @return value
         */
        t2 getValue() { return it->value; }

        /**
         * Friend function used to printing ring using cout
         * @param output output
         * @param iter iterator
         * @return ostream
         */
        friend ostream &operator<<(ostream &output, const Iterator &iter) {
            output << *iter;
            return output;
        }

    };

    typedef Iterator<t1, t2> TreeIterator;
    typedef Iterator<const t1, const t2> ConstTreeIterator;

    /**
     * returns iterator to begin
     * @return begin iterator
     */
    TreeIterator begin() {
        Node *curr = root;
        if (curr != NULL)
            while (curr->left != NULL)
                curr = curr->left;

        return TreeIterator(curr);
    }

    /**
     * returns iterator to end
     * @return end iterator
     */
    TreeIterator end() {
        return TreeIterator(nullptr);
    }

    /**
     * returns iterator to last element
     * @return iterator to last element
     */
    TreeIterator last() { return root ? TreeIterator(findMax(root)) : TreeIterator(nullptr); }

    /**
     * Searches for iterator with given value
     * @param value value
     * @return iterator with given value
     */
    TreeIterator find(const t1 &value) {
        for (TreeIterator iter = begin(); iter != last(); ++iter) if (iter->key == value) return iter;
        return TreeIterator(nullptr);
    }

    /**
     * returns iterator pointing to the first element
     * @return iterator pointing to the first element
     */
    ConstTreeIterator constBegin() const { return ConstRingIterator(root); }

    /**
     * returns iterator pointing to the last element
     * @return iterator pointing to the last element
     */
    ConstTreeIterator constEnd() const { return ConstRingIterator(root); }

    /**
     * returns last iterator
     * @return last iterator
     */
    ConstTreeIterator constLast() const {
        return !root ? ConstTreeIterator(nullptr) : ConstRingIterator(findMin(root));
    }

    /**
     * Look for iterator with given value
     * @param value value
     * @return iterator with given value
     */
    ConstTreeIterator constFind(const t1 &value) const {
        for (ConstTreeIterator iter = begin(); iter != last(); ++iter) if (iter->key == value) return iter;
        return ConstTreeIterator(nullptr);
    }

    /**
     * Default constructor
     */
    AVLTree() {
        root = NULL;
    }

    /**
     * COpying contrcutor
     * @param tree tree based on which new tree shall be created
     */
    AVLTree(AVLTree &tree) {
        root = NULL;
        for (TreeIterator it = tree.begin(); it != tree.end(); it++){
            insert(it->key, it->value);
        }
    }

    /**
     * Inserts node with given data.
     * @param x data with which node shall be inserted
     */
    void insert(t1 x, t2 y) {
        root = insert(x, y, root);
    }

    /**
     * Removes node with given data, if tree does not have such node nothing happens
     * @param x data with which node shall be removed
     */
    void remove(t1 x) {
        t2 y = searchKey(x)->value;
        root = remove(x, y, root);
    }

    /**
     * Looks for Node with given key
     * @param key key which a Node shall have
     * @return Node that has such key
     */
    Node *searchKey(t1 key) {
        return findKey(root, key);
    }

    /**
     * Looks for Node with given value
     * @param value value which a Node shall have
     * @return Node that has such value
     */
    Node *searchValue(t2 value) {
        return findValue(root, value);
    }

    /**
     * Prints tree to standard output
     */
    void print() {
        if (root == nullptr) cout << "Empty tree" << endl;
        print(root, 1);
    }

    /**
     * Overwritten operator[]
     * @param key key of element we are looking for
     * @return value of given element
     */
    t2 operator[](t1 key) {
        if (searchKey(key) == nullptr) {
            throw std::invalid_argument("Tree does not have such key");
        }
        t2 val = searchKey(key)->value;
        return val;
    }

    /**
     * Overwritten operator()
     * @param value value of element we are looking for
     * @return key of given element
     */
    t1 operator()(t2 value) {
        if (searchValue(value) == nullptr) {
            throw std::invalid_argument("Tree does not have such key");
        }
        t1 key = searchValue(value)->key;
        return key;
    }

    /**
     * Operator == that checks if two trees are equal
     * @param tree1 first tree
     * @param tree2 second tree
     * @return true if trees are equal, false otherwise
     */
    friend bool operator==(AVLTree &tree1, AVLTree &tree2) {
        if(tree2.root == nullptr && tree1.root == nullptr) return true;
        TreeIterator it = tree2.begin();
        for (TreeIterator itTree = tree1.begin(); itTree != tree1.end(); itTree++) {
            if (it == nullptr && itTree == nullptr) return true;
            if (it == nullptr || itTree == nullptr) return false;
            if (it->key != itTree->key && it->value != itTree->value) return false;
            it++;
        }
        return true;
    }


    /**
     * Overwritten operator=
     * @param tree tree to which new tree is created
     * @return reference to newly created tree
     */
    AVLTree& operator=(const AVLTree &tree) {
        auto *newTree = new AVLTree(tree);
        return *newTree;
    }

};