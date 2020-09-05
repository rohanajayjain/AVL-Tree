// Author: Rohan Ajay Jain
// Student ID: z5275538
// Platform: Unix
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int  value; //value (int) of this item 
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;


//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;


AVLTree *CloneAVLTree(AVLTree *T);
AVLTreeNode *CloneAVLTreeNode(AVLTreeNode *node);
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2);
void StoreInOrderNode(AVLTreeNode *node, AVLTreeNode array[], int *idx);
AVLTreeNode *CombineUnionArrays(AVLTree *T, AVLTreeNode array1[], AVLTreeNode array2[], 
                                        AVLTreeNode array3[], int T1Size, int T2Size);
AVLTreeNode *AVLNodeFromArray(AVLTreeNode array[], int first, int last);
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2);
int InsertNode(AVLTree *T, int k, int v);
int AttachNode(AVLTree *T, AVLTreeNode *node, AVLTreeNode *newNode);
int AttachLeft(AVLTree *T, AVLTreeNode *node, AVLTreeNode *newNode);
int AttachRight(AVLTree *T, AVLTreeNode *node, AVLTreeNode *newNode);
int Max(int a, int b);
int Height(AVLTreeNode *node);
void HeightSetter(AVLTreeNode *node);
int BalanceNum(AVLTreeNode *node);
AVLTreeNode *RotateLeft(AVLTreeNode *nodeX);
AVLTreeNode *RotateRight(AVLTreeNode *nodeY);
int DeleteNode(AVLTree *T, int k, int v);
AVLTreeNode *MinNode(AVLTreeNode *node);
AVLTreeNode *Search(AVLTree *T, int k, int v);
void FreeAVLTree(AVLTree *T);
void FreeAVLNode(AVLTreeNode *node);
void PrintAVLTree(AVLTree *T);
void PrintAVLNode(AVLTreeNode *node);
void ChildChecker(AVLTreeNode *parent, AVLTreeNode *nodeFrom, AVLTreeNode *nodeTo);


// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v) {
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree() {
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

// put your time complexity analysis of CreateAVLTree() here
/* Analysis:
The function separates strings iteratively in the buffer by a delimiter and attaches
(key, value) nodes to the AVLTree using InsertNode. It takes O(log n) time to attach a
node, and if there are n number of nodes, then it takes O(n log n) time to attach all
nodes.
*/
AVLTree *CreateAVLTree(const char *filename) {
    // put your code here
    char *ptr;
    char input[BUFSIZ];
    char delim[] = "\n \r\t,()";
    int flag = 0;
    int key, value;
    int alternate = 0;
    long numBytes;
    AVLTree *tree = newAVLTree();
    
    
    // User input from keyboard
    if (strcmp(filename, "stdin") == 0) {
        printf("Enter input: ");
        // Takes in user input and then processes key/value by separating input via delim
        while (flag != 1) {
            if (fgets(input, BUFSIZ, stdin) != NULL) {
                ptr = strtok(input, delim);
                while (ptr != NULL) {
                    if (strcmp(ptr, "end") == 0) {
                        flag = 1;
                        ptr = NULL;
                    } else {
                        // Alternates between taking key/value
                        if (alternate == 0) {
                            key = strtol(ptr, NULL, 10);
                            alternate = 1;
                        } else {
                            value = strtol(ptr, NULL, 10);
                            alternate = 0;
                            if (InsertNode(tree, key, value) == 0) {
                                fprintf(stderr, "Invalid input!");
                                exit(1);
                            }
                        }
                        ptr = strtok(NULL, delim);
                    }
                }
            }
        }
    // User input from file
    } else {
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            fprintf(stderr, "Invalid input! File is invalid.");
            exit(1);
        } 
        fseek(fp, 0, SEEK_END);
        numBytes = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fread(input, 1, numBytes, fp);
        fclose(fp);
        
        ptr = strtok(input, delim);
        while (ptr != NULL) {
            if (alternate == 0) {
                key = strtol(ptr, NULL, 10);
                alternate = 1;
            } else {
                value = strtol(ptr, NULL, 10);
                alternate = 0;
                if (InsertNode(tree, key, value) == 0) {
                    fprintf(stderr, "Invalid input!");
                    exit(1);
                }
            }
            ptr = strtok(NULL, delim);
        }
    }
    return tree;
}

// put your time complexity analysis for CloneAVLTree() here
/* Analysis:
    int key; //key of this item
CloneAVLTree calls CloneAVLTreeNode
    int key; //key of this item which runs recursively to clone the left
child and right child of the origin
    int key; //key of this itemal node. Since it must clone every node of the
tree, if there are n nodes, then the time complexity is O(n).
*/
AVLTree *CloneAVLTree(AVLTree *T) {
    // put your code here
    AVLTree *newTree = newAVLTree();
    newTree->root = CloneAVLTreeNode(T->root);
    newTree->size = T->size;

    return newTree;
}

AVLTreeNode *CloneAVLTreeNode(AVLTreeNode *node) {
    if (node == NULL) {
        return NULL;
    }
    AVLTreeNode *cloneNode = newAVLTreeNode(node->key, node->value);
    // Recursively create clone of left child
    cloneNode->left = CloneAVLTreeNode(node->left);
    if (cloneNode->left != NULL) {
        cloneNode->left->parent = cloneNode;
    }
    // Recursively create clone of right child
    cloneNode->right = CloneAVLTreeNode(node->right);
    if (cloneNode->right != NULL) {
        cloneNode->right->parent = cloneNode;
    }
    cloneNode->height = node->height;

    return cloneNode;
}
 
// put your time complexity for AVLTreesUnion() here
/* Analysis:
AVLTreesUnion calls StoreInOrderNode which recursively traverses a tree via in-order
traversal and stores each node to an array. The time complexity of StoreInOrderNode 
is thus O(n) where n is the number of nodes in a tree. As we are finding the union
between two trees, let m be the number of nodes in the first tree and n be the number
of nodes in the second tree meaning the time complexity of StoreInOrderNode for both
trees is O(m + n).

CombineUnionArrays iterates through two provided arrays simultaneously and adds non-
duplicate nodes from each array to a third array. As such, as CombineUnionArrays is
called on the arrays created by StoreInOrderNode for T1 and T2, the time complexity
is thus O(m + n) for CombineUnionArrays.

Finally, AVLNodeFromArray recursively traverses the union array provided by 
CombineUnionArrays. As it goes through each node in the union array, it is therefore,
O(m + n) time complexity.

Putting it all together, the entire function is O(m + n) time complexity.
*/
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2) {
    //put your code here
    int idx1 = 0, idx2 = 0;
    AVLTree *unionTree = newAVLTree();
    AVLTreeNode T1Array[T1->size];
    AVLTreeNode T2Array[T2->size];
    AVLTreeNode T3Array[T1->size + T2->size];

    StoreInOrderNode(T1->root, T1Array, &idx1);
    StoreInOrderNode(T2->root, T2Array, &idx2);

    memcpy(T3Array, CombineUnionArrays(unionTree, T1Array, T2Array, 
                    T3Array, T1->size, T2->size), T1->size + T2->size);
    
    unionTree->root = AVLNodeFromArray(T3Array, 0, (unionTree->size)-1);
    
    return unionTree;
}

void StoreInOrderNode(AVLTreeNode *node, AVLTreeNode array[], int *idx) {
    if (node != NULL) {
        // Store left child
        StoreInOrderNode(node->left, array, idx);
        // Store current node and increment index
        array[*idx] = *node;
        (*idx)++;
        // Store right child
        StoreInOrderNode(node->right, array, idx);
    }
}

AVLTreeNode *CombineUnionArrays(AVLTree *T, AVLTreeNode array1[], AVLTreeNode array2[], 
                                        AVLTreeNode array3[], int T1Size, int T2Size) {

    int idx1 = 0, idx2 = 0, idx3 = 0;

    // Traverse through array1 and array2
    while (idx1 < T1Size && idx2 < T2Size) {
        // Node 1 key < Node 2 key
        if (array1[idx1].key < array2[idx2].key) {
         array3[idx3] = array1[idx1];
            idx1++;

        // Node 1 key > Node 2 key
        } else if (array1[idx1].key > array2[idx2].key) {
         array3[idx3] = array2[idx2];
            idx2++;
            
        // Node 1 key == Node 2 key
        } else {
            // Node 1 value < Node 2 value
            if (array1[idx1].value < array2[idx2].value) {
             array3[idx3] = array1[idx1];
                idx1++;
            // Node 1 value > Node 2 value
            } else if (array1[idx1].value > array2[idx2].value) {
             array3[idx3] = array2[idx2];
                idx2++;
            // Node found in both arrays
            } else {
             array3[idx3] = array1[idx1];
                idx1++;
                idx2++;
            }
        }
        idx3++;
        T->size++;
    }
    
    // More elements remaining in first array
    while (idx1 < T1Size) {
     array3[idx3] = array1[idx1];
        idx1++;
        idx3++;
        T->size++;
    }
    
    // More elements remaining in second array
    while (idx2 < T2Size) {
     array3[idx3] = array2[idx2];
        idx2++;
        idx3++;
        T->size++;
    }

    return array3;
}

AVLTreeNode *CombineInterArrays(AVLTree *T, AVLTreeNode array1[], AVLTreeNode array2[],
AVLTreeNode array3[], int T1Size, int T2Size) {
    int idx1 = 0, idx2 = 0, idx3 = 0;

    // Traverse through array1 and array2
    while (idx1 < T1Size && idx2 < T2Size) {
        // Node 1 key < Node 2 key
        if (array1[idx1].key < array2[idx2].key) {
            idx1++;
        
        // Node 1 key > Node 2 key
        } else if (array1[idx1].key > array2[idx2].key) {
            idx2++;
        
        // Node 1 key == Node 2 key
        } else {
            // Node 1 value < Node 2 value
            if (array1[idx1].value < array2[idx2].value) {
                idx1++;
            
            // Node 1 value > Node 2 value
            } else if (array1[idx1].value > array2[idx2].value) {
                idx2++;
            
            // Node found in both arrays
            } else {
                array3[idx3] = array1[idx1];
                idx1++;
                idx2++;
                idx3++;
                T->size++;
            }
        }
    }

    return array3;

}

AVLTreeNode *AVLNodeFromArray(AVLTreeNode array[], int first, int last) {
    // Out of bounds
    if (first > last) {
        return NULL;
    }

    // Create root node
    int mid = (first + last) / 2;
    AVLTreeNode *root = newAVLTreeNode(array[mid].key, array[mid].value);
    

    // Recursively create left child
    root->left = AVLNodeFromArray(array, first, mid-1);
    if (root->left != NULL) {
        root->left->parent = root;
    }

    // Recursively create right child
    root->right = AVLNodeFromArray(array, mid+1, last);
    if (root->right != NULL) {
        root->right->parent = root;
    }

    HeightSetter(root);

    return root;
}

// put your time complexity for ALVTreesIntersection() here
/* Analysis:
AVLTreesUnion calls StoreInOrderNode which recursively traverses a tree via in-order
traversal and stores each node to an array. The time complexity of StoreInOrderNode 
is thus O(n) where n is the number of nodes in a tree. As we are finding the intersection
between two trees, let m be the number of nodes in the first tree and n be the number
of nodes in the second tree meaning the time complexity of StoreInOrderNode for both
trees is O(m + n).

CombineInterArrays iterates through two provided arrays simultaneously and adds nodes
that appear in both arrays to a third array. As such, as CombineInterArrays is called on
the arrays created by StoreInOrderNode for T1 and T2, the time complexity is thus 
O(m + n) for CombineInterArrays.

Finally, AVLNodeFromArray recursively traverses the intersection array provided by 
CombineInterArrays. As it goes through each node in the intersection array, it is 
therefore, in the worst case where T1 and T2 are duplicates of each other, O(m + n) 
time complexity.

Putting it all together, the entire function is O(m + n) time complexity.
*/
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2) {
    //put your code here
    int idx1 = 0, idx2 = 0;
    AVLTree *interTree = newAVLTree();
    AVLTreeNode T1Array[T1->size];
    AVLTreeNode T2Array[T2->size];
    AVLTreeNode T3Array[T1->size + T2->size];

    StoreInOrderNode(T1->root, T1Array, &idx1);
    StoreInOrderNode(T2->root, T2Array, &idx2);

    memcpy(T3Array, CombineInterArrays(interTree, T1Array, T2Array, T3Array, 
                                    T1->size, T2->size), T1->size + T2->size);

    interTree->root = AVLNodeFromArray(T3Array, 0, (interTree->size)-1);

    return interTree;
}

// put the time complexity analysis for InsertNode() here
/* Analysis:
InsertNode recursively calls AttachNode. AttachNode traverses the tree, using helper
functions such as AttachLeft and AttachRight which call AttachNode after conducting O(1)
operations. Every operation in AttachNode is O(1) complexity. Since AttachNode traverses
the tree by comparing (key, value), as the tree is an AVL tree, in the worst case, where
n is the size of the tree, it will take O(log n) operations to insert the node.
*/  
int InsertNode(AVLTree *T, int k, int v) {
    //put your code here
    AVLTreeNode *newNode = newAVLTreeNode(k, v);
    if (T->root == NULL) {
        T->root = newNode;
    } else {
        // Recursively calls AttachNode. If newNode is a duplicate, return 0
        if (AttachNode(T, T->root, newNode) == 0) {
            return 0;
        }
    }
    T->size++;
    return 1;
}
// Recursively traverses through tree to attach newNode. Rotates tree to fix imbalances
// as required.
int AttachNode(AVLTree *T, AVLTreeNode *node, AVLTreeNode *newNode) {
    int flag = 1;
    // Attach node as left child if new node key is less than node key
    if (newNode->key < node->key) {
        flag = AttachLeft(T, node, newNode);
    // Attach node as right child if new node key is greater than node key
    } else if (newNode->key > node->key) {
        flag = AttachRight(T, node, newNode);
    } else {
        // Attach node as left child if new node value is less than node value
        if (newNode->value < node->value) {
            flag = AttachLeft(T, node, newNode);
        // Attach node right child if new node value is greater than node value
        } else if (newNode->value > node->value) {
            flag = AttachRight(T, node, newNode);
        // Duplicate node found
        } else {
            flag = 0;
        }
    }
    // Returns 0 when node already exists in the tree
    if (flag == 0) {
        return 0;
    }
    
    HeightSetter(node);

    // Determines if the subtree is balanced
    int balance = BalanceNum(node);

    // If the tree is left heavy:
    if (balance > 1) {
        if (newNode->key < node->left->key) {
            node = RotateRight(node);
        } else if (newNode->key > node->left->key) {
            node->left = RotateLeft(node->left);
            node = RotateRight(node);
        } else {
            if (newNode->value < node->left->value) {
                node = RotateRight(node);
            } else if (newNode->value > node->left->value) {
                node->left = RotateLeft(node->left);
                node = RotateRight(node);
            }
        }
    // If the tree is right heavy:
    } else if (balance < -1) {
        if (newNode->key > node->right->key) {
            node = RotateLeft(node);
        } else if (newNode->key < node->right->key) {
            node->right = RotateRight(node->right);
            node = RotateLeft(node);
        } else {
            if (newNode->value > node->right->value) {
                node = RotateLeft(node);
            } else if (newNode->value < node->right->value) {
                node->right = RotateRight(node->right);
                node = RotateLeft(node);
            }
        }
    }
    T->root = node;
    return 1;
}
// Recursively attaches a node as the left child
int AttachLeft(AVLTree *T, AVLTreeNode *node, AVLTreeNode *newNode) {
    if (node->left != NULL) {
        return AttachNode(T, node->left, newNode);
    } else {
        node->left = newNode;
        newNode->parent = node;
        return 1;
    }
}
// Recursively attaches a node as the right child
int AttachRight(AVLTree *T, AVLTreeNode *node, AVLTreeNode *newNode) {
    if (node->right != NULL) {
        return AttachNode(T, node->right, newNode);
    } else {
        node->right = newNode;
        newNode->parent = node;
        return 1;
    }
}

int Max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int Height(AVLTreeNode *node) {
    if (node == NULL) {
        return -1;
    }
    return node->height;
}

void HeightSetter(AVLTreeNode *node) {
    node->height = 1 + Max(Height(node->left), Height(node->right));
}

int BalanceNum(AVLTreeNode *node) {
    if (node == NULL) {
        return 0;
    }
    return Height(node->left) - Height(node->right);
}

AVLTreeNode *RotateLeft(AVLTreeNode *nodeX) {
    AVLTreeNode *nodeY, *nodeZ;
    
    nodeY = nodeX->right;
    nodeZ = nodeY->left;
    nodeY->left = nodeX;
    nodeX->right = nodeZ;
    nodeY->parent = nodeX->parent;
    
    if (nodeX->parent != NULL && nodeX->parent->left == nodeX) {
        nodeX->parent->left = nodeY;
    } else if (nodeX->parent != NULL && nodeX->parent->right == nodeX) {
        nodeX->parent->right = nodeY;
    }
    
    nodeX->parent = nodeY;
    

    HeightSetter(nodeX);
    HeightSetter(nodeY);

    return nodeY;
}

AVLTreeNode *RotateRight(AVLTreeNode *nodeY) {
    AVLTreeNode *nodeX, *nodeZ;
    
    nodeX = nodeY->left;
    nodeZ = nodeX->right;
    nodeX->right = nodeY;
    nodeY->left = nodeZ;
    nodeX->parent = nodeY->parent;

    if (nodeY->parent != NULL && nodeY->parent->left == nodeY) {
        nodeY->parent->left = nodeX;
    } else if (nodeY->parent != NULL && nodeY->parent->right == nodeY) {
        nodeY->parent->right = nodeX;
    }

    nodeY->parent = nodeX;

    HeightSetter(nodeY);
    HeightSetter(nodeX);

    return nodeX;
}

// put your time complexity for DeleteNode() here
/* Analysis:
DeleteNode calls search which is an O(log n) operation to locate the node to be deleted.
It then makes O(1) operations to realign parent/child relationships. It then iteratively
traverses the tree upwards to rotate the tree as required. As DeleteNode runs on an
AVLTree, if n is the size of the tree, then deleting a node is O(log n) time complexity.
*/
int DeleteNode(AVLTree *T, int k, int v) {
    // put your code here
    AVLTreeNode *parent, *lowest, *leftChild, *rightChild;
    AVLTreeNode *node = Search(T, k, v);

    // Node to delete doesn't exist
    if (node == NULL) {
        return 0;
    }

    parent = node->parent;

    // realign parents and children to ensure tree integrity and delete node
    // node has no children
    if (node->left == NULL && node->right == NULL) {
        ChildChecker(parent, node, NULL);
        free(node);
        node = parent;
    // node has one child
    
    } else if ((node->left != NULL && node->right == NULL)
            ||(node->right != NULL && node->left == NULL)) {
        if (node->left != NULL) {
            ChildChecker(parent, node, node->left);
            node = node->left;
            free(node->parent);
            node->parent = parent;
        } else if (node->right != NULL) {
            ChildChecker(parent, node, node->right);
            node = node->right;
            free(node->parent);
            node->parent = parent;
        }
    // node has two children
    } else {
        lowest = MinNode(node->right);
        rightChild = node->right;
        leftChild = node->left;

        ChildChecker(lowest->parent, lowest, NULL);
        lowest->right = rightChild;
        lowest->left = leftChild;
        rightChild->parent = lowest;
        leftChild->parent = lowest;
        ChildChecker(parent, node, lowest);

        free(node);
        lowest->parent = parent;
        node = MinNode(lowest->right);
    }

    // rotate tree as reqd while going up the tree
    while (node != NULL) {
        HeightSetter(node);

        int balance = BalanceNum(node);

        if (balance > 1) {
            if (BalanceNum(node->left) >= 0) {
                RotateRight(node);
            } else {
                node->left = RotateLeft(node->left);
                RotateRight(node);
            }
        } else if (balance < -1) {
            if (BalanceNum(node->right) <= 0) {
                RotateLeft(node);
            } else {
                node->right = RotateRight(node->right);
                RotateLeft(node);
            }
        }
        T->root = node;
        node = node->parent;
    }
    T->size--;
    
    return 1;
}
// Reassigns parent child from nodeFrom to nodeTo
void ChildChecker(AVLTreeNode *parent, AVLTreeNode *nodeFrom,
    AVLTreeNode *nodeTo) {
    if (parent != NULL) {
        if (parent->left == nodeFrom) {
            parent->left = nodeTo;
        } else {
            parent->right = nodeTo;
        }
    }
}

// Finds the smallest (key, value) node in a tree
AVLTreeNode *MinNode(AVLTreeNode *node) {
    if (node == NULL || node->left == NULL) {
        return node;
    }
    return MinNode(node->left);
}

// put your time complexity analysis for Search() here
/* Analysis:
Finds a node in a tree by comparing (key, value) of the desired node and current node.
Since the tree is an AVL tree, if n is the size of the tree, the time complexity is 
O(log n).
*/
AVLTreeNode *Search(AVLTree *T, int k, int v) {
    // put your code here
    AVLTreeNode *currentNode = T->root;
    while (currentNode != NULL) {
        // Found node to be found
        if (currentNode->key == k && currentNode->value == v) {
            return currentNode;
        }
        // Node to be found has smaller key
        if (currentNode->key > k) {
            currentNode = currentNode->left;

        // Node to be found has bigger key
        } else if (currentNode->key < k) {
            currentNode = currentNode->right;

        // Node to be found has same key as current node
        } else {
            // Node to be found has smaller value
            if (currentNode->value > v) {
                currentNode = currentNode->left;
            // Node to be found has bigger value
            } else {
                currentNode = currentNode->right;
            }
        }
    }
    return NULL;
}

// put your time complexity analysis for freeAVLTree() here
/* Analysis:
FreeAVLTree calls FreeAVLNode which runs recursively to free every node in a tree. Since
FreeAVLNode goes through every single node in a tree, if there are n nodes in a tree,
the time complexity is O(n).
*/
void FreeAVLTree(AVLTree *T) {
    // put your code here
    FreeAVLNode(T->root);
    free(T);
}
// Recursively frees every node in a tree
void FreeAVLNode(AVLTreeNode *node) {
    if (node->left != NULL) {
        FreeAVLNode(node->left);
    }
    if (node->right != NULL) {
        FreeAVLNode(node->right);
    }
    free(node);
}

// put your time complexity analysis for PrintAVLTree() here
/* Analysis:
PrintAVLTree prints the in-order traversal of T by calling PrintAVLNode which
recursively runs to print out the (key, value) and height of the left child node, 
current node and right child node. Since it prints out every node, if there are n nodes,
the time complexity is O(n).
*/
void PrintAVLTree(AVLTree *T) {
    // put your code here
    PrintAVLNode(T->root);
}

void PrintAVLNode(AVLTreeNode *node) {
    if (node->left != NULL) {
        PrintAVLNode(node->left);
    }
    printf("(%d, %d), %d\n", node->key, node->value, node->height);
    if (node->right != NULL) {
        PrintAVLNode(node->right);
    }
}

int main() { //sample main for testing
    int i,j;
    AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
    AVLTreeNode *node1;
    
    tree1=CreateAVLTree("stdin");
    PrintAVLTree(tree1);
    FreeAVLTree(tree1);
    //you need to create the text file file1.txt
    // to store a set of items without duplicate items
    tree2=CreateAVLTree("File1.txt"); 
    PrintAVLTree(tree2);
    tree3=CloneAVLTree(tree2);
    PrintAVLTree(tree3);
    FreeAVLTree(tree2);
    FreeAVLTree(tree3);
    //Create tree4 
    tree4=newAVLTree();
    j=InsertNode(tree4, 10, 10);
    for (i=0; i<15; i++)
    {
    j=InsertNode(tree4, i, i);
    if (j==0) printf("(%d, %d) already exists\n", i, i);
    }
    PrintAVLTree(tree4);
    node1=Search(tree4,20,20);
    if (node1!=NULL)
        printf("key= %d value= %d\n",node1->key,node1->value);
    else 
        printf("Key 20 does not exist\n");
    
    for (i=17; i>0; i--)
    {
        j=DeleteNode(tree4, i, i);
        if (j==0) 
        printf("Key %d does not exist\n",i);  
        PrintAVLTree(tree4);
    }
    FreeAVLTree(tree4);
    //Create tree5
    tree5=newAVLTree();
    j=InsertNode(tree5, 6, 25);
    j=InsertNode(tree5, 6, 10);
    j=InsertNode(tree5, 6, 12);
    j=InsertNode(tree5, 6, 20);
    j=InsertNode(tree5, 9, 25);
    j=InsertNode(tree5, 10, 25);
    PrintAVLTree(tree5);
    //Create tree6
    tree6=newAVLTree();
    j=InsertNode(tree6, 6, 25);
    j=InsertNode(tree6, 5, 10);
    j=InsertNode(tree6, 6, 12);
    j=InsertNode(tree6, 6, 20);
    j=InsertNode(tree6, 8, 35);
    j=InsertNode(tree6, 10, 25);
    PrintAVLTree(tree6);
    tree7=AVLTreesIntersection(tree5, tree6);
    tree8=AVLTreesUnion(tree5,tree6);
    PrintAVLTree(tree7);
    PrintAVLTree(tree8);
    return 0; 
}