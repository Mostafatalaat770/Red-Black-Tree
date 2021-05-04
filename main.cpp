#include <iostream>
#include <string.h>
#include <stdexcept>
using namespace std;

class Node
{
public:
    char *data;
    Node *parent;
    Node *left_child;
    Node *right_child;
    bool isRed = true;
    Node *grandparent()
    {
        return this->parent->parent;
    }
    Node *uncle()
    {
        if (this->grandparent()->left_child == NULL)
            return NULL;
        return strcmp(this->grandparent()->left_child->data, parent->data) == 0 ? this->grandparent()->right_child : this->grandparent()->left_child;
    }
};

class RBT
{
private:
    Node *root;
    int get_height(Node *node);
    bool get_search(Node *root, char *target);
    void get_insert(Node *root, char *target);
    void fix_tree(Node *newNode);
    void rotate(Node *newNode, int mode);
    Node *new_node(Node *parent, char *data);
    void print_tree(Node *node);

public:
    RBT(char *filename)
    {
        char temp[30];
        bool isRoot = true;
        FILE *f = fopen(filename, "r");

        while (!feof(f))
        {
            fscanf(f, "%s\n", temp);
            char *word = (char *)malloc(sizeof(char) * strlen(temp) + 1);
            strcpy(word, temp);
            if (isRoot)
            {
                root = new_node(NULL, word);
                isRoot = false;
                continue;
            }
            try
            {
                insert(word);
            }
            catch (const logic_error &e)
            {
            }
        }
        fclose(f);
    }
    int size;
    bool search(char *target)
    {
        return get_search(root, target);
    }
    void insert(char *data)
    {
        get_insert(root, data);
    }
    int height()
    {
        return get_height(root);
    };
    void print()
    {
        print_tree(root);
    }
};

Node *RBT::new_node(Node *parent, char *data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left_child = NULL;
    newNode->right_child = NULL;
    newNode->parent = parent;
    newNode->isRed = parent == NULL ? false : true;
    ++size;
    return newNode;
}

void RBT::rotate(Node *newNode, int mode)
{
    Node *grandparent = newNode->grandparent();
    Node *gGrandparent = grandparent != NULL ? grandparent->parent : NULL;
    Node *parent = newNode->parent;
    Node *uncle = grandparent != NULL ? newNode->uncle() : NULL;

    switch (mode)
    {
    case 0:
        uncle->isRed = false;
        newNode->parent->isRed = false;
        grandparent->isRed = true;
        RBT::fix_tree(grandparent);
        break;

    case 1: //right-right
        if (gGrandparent != NULL)
        {
            if (strcmp(grandparent->data, gGrandparent->data) > 0)
                gGrandparent->right_child = parent;
            else
                gGrandparent->left_child = parent;
        }
        else
            root = parent;
        parent->parent = gGrandparent;
        grandparent->right_child = parent->left_child;
        if (parent->left_child != NULL)
            parent->left_child->parent = grandparent;
        parent->left_child = grandparent;
        grandparent->parent = parent;
        grandparent->isRed = true;
        parent->isRed = false;
        break;
    case 2: //right-left
        newNode->parent = grandparent;
        grandparent->right_child = newNode;
        parent->left_child = newNode->right_child;
        if (newNode->right_child != NULL)
            newNode->right_child->parent = parent;
        newNode->right_child = parent;
        parent->parent = newNode;
        rotate(newNode->right_child, 1);
        break;
    case 3: //left-left
        if (gGrandparent != NULL)
        {
            if (strcmp(grandparent->data, gGrandparent->data) > 0)
                gGrandparent->right_child = parent;
            else
                gGrandparent->left_child = parent;
        }
        else
            root = parent;

        parent->parent = gGrandparent;
        grandparent->left_child = parent->right_child;
        if (parent->right_child != NULL)
            parent->right_child->parent = grandparent;
        parent->right_child = grandparent;
        grandparent->parent = parent;
        grandparent->isRed = true;
        parent->isRed = false;
        break;
    case 4: //left-right
        newNode->parent = grandparent;
        grandparent->left_child = newNode;
        parent->right_child = newNode->left_child;
        if (newNode->left_child != NULL)
            newNode->left_child->parent = parent;
        newNode->left_child = parent;
        parent->parent = newNode;
        rotate(newNode->left_child, 3);
        break;
    }
}

void RBT::fix_tree(Node *newNode)
{
    if (newNode->parent == NULL)
    {
        newNode->isRed = false;
        return;
    }
    if (newNode->parent->isRed == false)
    {
        return;
    }
    Node *uncle = newNode->grandparent() != NULL ? newNode->uncle() : NULL;
    if (uncle != NULL && uncle->isRed)
    {
        rotate(newNode, 0);
        return;
    }
    Node *grandparent = newNode->grandparent();

    bool isRight;
    if (grandparent->left_child == NULL)
        isRight = true;
    else if (grandparent->right_child == NULL)
        isRight = false;
    else
        isRight = strcmp(grandparent->left_child->data, newNode->parent->data) == 0 ? false : true;

    if (isRight)
    {
        //right-right 1
        if (strcmp(newNode->data, newNode->parent->data) > 0)
            rotate(newNode, 1); //right-right 1

        else
            rotate(newNode, 2); //right-left 2
    }
    else if (!isRight)
    {
        if (strcmp(newNode->data, newNode->parent->data) < 0)
            rotate(newNode, 3); //left-left 3

        else
            rotate(newNode, 4); //left-right 4
    }
}

void RBT::get_insert(Node *node, char *target)
{
    if (strcmp(node->data, target) == 0)
    {
        throw std::logic_error("");
    }
    if (node->left_child != NULL && strcmp(target, node->data) < 0)
    {
        get_insert(node->left_child, target);
    }
    else if (node->right_child != NULL && strcmp(target, node->data) > 0)
    {
        get_insert(node->right_child, target);
    }
    else if (node->left_child == NULL && strcmp(target, node->data) < 0)
    {
        Node *newNode = RBT::new_node(node, target);
        node->left_child = newNode;
        fix_tree(newNode);
        return;
    }
    else if (node->right_child == NULL && strcmp(target, node->data) > 0)
    {
        Node *newNode = RBT::new_node(node, target);
        node->right_child = newNode;
        fix_tree(newNode);
        return;
    }
}

int RBT::get_height(Node *node)
{
    if (node == NULL)
        return -1;
    return 1 + max(get_height(node->left_child), get_height(node->right_child));
}

bool RBT::get_search(Node *node, char *target)
{

    if (node == NULL)
        return false;

    if (strcmp(node->data, target) > 0)
        return get_search(node->left_child, target);
    if (strcmp(node->data, target) < 0)
        return get_search(node->right_child, target);

    return true;
}

void RBT::print_tree(Node *node)
{
    if (node == NULL)
        return;
    print_tree(node->left_child);
    cout << node->data << "  ";
    if (node->isRed == true)
        cout << "red" << endl;
    else
        cout << "black" << endl;
    if (node->parent == NULL)
        cout << " parent is " << node->parent << endl;
    else
        cout << " parent is " << node->parent->data << endl;

    if (node->left_child == NULL)
        cout << " left child is " << node->left_child << endl;
    else
        cout << " left child is " << node->left_child->data << endl;

    if (node->right_child == NULL)
        cout << " right child is " << node->right_child << endl;
    else
        cout << " right child is " << node->right_child->data << endl;

    print_tree(node->right_child);
}
int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage: %s filename1\n", argv[0]);
        exit(1);
    }

    int mode;
    bool isLoaded = false, exit = false;
    RBT *dict;
    while (!exit)
    {
        do
        {
            cout << "Choose the operation you would like to perform: 1- load dictionary 2- print dictionary size "
                 << "3- insert a word 4- look-up a word 5- exit: ";
            cin >> mode;

        } while (mode < 1 || mode > 6);

        switch (mode)
        {
        case 1:
            dict = new RBT(argv[1]);
            isLoaded = true;
            cout << "Dictionary has been loaded successfully!" << endl;
            break;
        case 2:
            if (isLoaded)
            {
                cout << "Height of the tree = " << dict->height() << endl;
                cout << "Size of the dictionary = " << dict->size << endl;
            }
            else
                cout << "The dictionary isn't loaded yet!" << endl;
            break;
        case 3:
            if (isLoaded)
            {
                string temp;
                cout << "Type the word you would like to insert: ";
                cin >> temp;
                char newWord[temp.length() + 1];
                strcpy(newWord, temp.c_str());
                try
                {
                    dict->insert(newWord);
                }
                catch (const logic_error &e)
                {
                    cout << "ERROR: Word already in the dictionary!" << endl;
                }
            }
            else
                cout << "The dictionary isn't loaded yet!" << endl;
            break;
        case 4:
            if (isLoaded)
            {
                string temp;
                cout << "Type the word you would like to search: ";
                cin >> temp;
                char target[temp.length() + 1];
                strcpy(target, temp.c_str());
                bool isFound = dict->search(target);
                printf("The word %s.\n", isFound ? "exists" : "doesn't exist");
            }
            else
                cout << "The dictionary isn't loaded yet!" << endl;
            break;
        case 5:
            exit = true;
            break;
        }
    }
    return 0;
}
