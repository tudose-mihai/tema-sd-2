#include <iostream>


using namespace std;

int global_degree = 5;

class bNode {
    int degree;
    bool leaf;
    bNode *parent;
    bNode **children;
    int *keys;
    int stored;
public:
    bNode() {
        degree = global_degree;
        leaf = true;
        children = new bNode *[degree + 1];
        for (int i = 0; i < degree; i++)
            children[i] = nullptr;

        keys = new int[degree + 1];
        for (int i = 0; i < degree - 1; i++)
            keys[i] = 0;

        stored = 0;
        parent = nullptr;
    }

    bNode(int key) {
        degree = global_degree;
        leaf = true;
        children = new bNode *[degree];
        for (int i = 0; i < degree; i++)
            children[i] = nullptr;

        keys = new int[degree];
        for (int i = 0; i < degree - 1; i++)
            keys[i] = 0;

        keys[0] = key;
        stored = 1;
        parent = nullptr;
    }


    void print(bNode *node) {
        for (int i = 0; i < node->stored; i++)
            cout << node->keys[i] << " ";
        cout << '\n';
    }

    friend class bTree;
};

class bTree {
    bNode *root;
public:
    bTree() {
        root = nullptr;
    }

    bNode *getRoot() { return root; }

    void insertKey(int key, bNode *node) {
        bNode *temp = new bNode;
        if (root == nullptr) {
            root = temp;
            node = root;
            temp->stored = 1;
            temp->keys[0] = key;
        } else if (node->leaf == true) {
            if (node->stored == 0) {
                node->keys[0] = key;
                node->stored++;
            } else {
                int i = 0, j = node->stored;
                while (node->keys[i] < key && i < j)
                    i++;
                while (j > i) {
                    node->keys[j] = node->keys[j - 1];
                    j--;
                }
                node->keys[i] = key;
                node->stored++;
            }
        } else if (node->leaf == false) {
            if (key <= node->keys[0]) {
                node->children[0]->parent = node;
                insertKey(key, node->children[0]);
            } else {
                int i = node->stored - 1;
                while (key <= node->keys[i] && i > 0)
                    i--;
                node->children[i]->parent = node;
                insertKey(key, node->children[i + 1]);
            }
        }
        if (node->stored == global_degree) {
            splitNode(node);
        }
    }

    void splitNode(bNode *node) {
        if (node == root) {
            bNode *temp = new bNode;
            temp->leaf = false;
            node->parent = temp;
            root = temp;
        }
        if (node->leaf == true) {
            int middle = node->keys[global_degree / 2 - (1 - global_degree % 2)];
            bNode *parent = node->parent;
            parent->leaf = false;
            int i = global_degree / 2 - (1 - global_degree % 2), j = node->stored, k = i + 1, l = node->stored;
            while (i < j) {
                int x = node->keys[i], y = node->keys[i + 1];
                node->keys[i] = node->keys[i + 1];
                i++;
            }
            node->keys[i - 1] = 0;
            node->stored--;
            i = 0;
            j = parent->stored;
            bNode *newHalf = new bNode;
            while (parent->keys[i] < middle && i < j)
                i++;
            while (j > i) {
                parent->keys[j] = parent->keys[j - 1];
                parent->children[j + 1] = parent->children[j];
                j--;
            }
            parent->keys[i] = middle;
            parent->children[i] = node;
            parent->children[i + 1] = newHalf;
            newHalf->parent = parent;
            parent->stored++;
            for (i = 0; i < l - k; i++) {
                newHalf->keys[i] = node->keys[k + i - 1];
                node->keys[k + i - 1] = 0;
                node->stored--;
                newHalf->stored++;
            }
            if (parent->stored == global_degree)
                splitNode(parent);
        } else if (node->leaf == false) {
            int middle = node->keys[global_degree / 2 - (1 - global_degree % 2)];
            bNode *parent = node->parent;
            parent->leaf = false;
            int i = global_degree / 2 - (1 - global_degree % 2), j = node->stored, k = i + 1, l = node->stored;
            while (i < j) {
                node->keys[i] = node->keys[i + 1];
                i++;
            }
            node->keys[i - 1] = 0;
            node->stored--;
            i = 0;
            j = parent->stored;
            bNode *newHalf = new bNode;
            newHalf->leaf = false;
            while (parent->keys[i] < middle && i < j)
                i++;
            while (j > i) {
                parent->keys[j] = parent->keys[j - 1];
                parent->children[j + 1] = parent->children[j];
                j--;
            }
            parent->keys[i] = middle;
            parent->children[i] = node;
            parent->children[i + 1] = newHalf;
            newHalf->parent = parent;
            parent->stored++;
            for (i = 0; i < l - k; i++) {
                newHalf->keys[i] = node->keys[k + i - 1];
                node->keys[k + i - 1] = 0;
                newHalf->children[i] = node->children[k + i];
                newHalf->children[i]->parent = newHalf;
                node->children[k + i] = nullptr;
                node->stored--;
                newHalf->stored++;
            }

            newHalf->children[i] = node->children[k + i];
            newHalf->children[i]->parent = newHalf;
            node->children[k + i] = nullptr;
            if (parent->stored == global_degree)
                splitNode(parent);
        }

    }

    bNode *findSmallestInSubtree(bNode *node) {
        if (node->children[0] != nullptr)
            return findSmallestInSubtree(node->children[0]);
        else
            return node;
    }

    bNode *findLargestInSubtree(bNode *node) {
        int i = node->stored;
        if (node->children[i] != nullptr)
            return findLargestInSubtree(node->children[i]);
        else
            return node;
    }
    
    int findSuccessor(int key){
        bNode *node = root;
        int ok = 1,i;
        while (ok) // find node containing if it exists
        {
            ok = 0;
            i = 0;
            while (node->keys[i] < key && i < node->stored)
                i++;
            if (node->keys[i] == key) {
                ok = 1;
                break;
            } else if (node->children[i] != nullptr) {
                node = node->children[i];
                ok = 1;
            } else cout << "nu exista \n";
        }
        node = findSmallestInSubtree(node->children[i+1]);
        return node->keys[0];
    }
    int findPredecessor(int key){
        bNode *node = root;
        int ok = 1, i=0;
        while (ok) // find node containing if it exists
        {
            ok = 0;
            i = 0;
            while (node->keys[i] < key && i < node->stored)
                i++;
            if (node->keys[i] == key) {
                ok = 1;
                break;
            } else if (node->children[i] != nullptr) {
                node = node->children[i];
                ok = 1;
            } else cout << "nu exista \n";
        }
        node = findLargestInSubtree(node->children[i]);
        return node->keys[node->stored-1];
    }

    void printInOrder(bNode *node)
    {
        for(int i=0;i<node->stored;i++)
        {
            if(node->children[i]!= nullptr)
                printInOrder(node->children[i]);
            cout << node->keys[i] << " ";
        }
        if(node->children[node->stored]!= nullptr)
            printInOrder(node->children[node->stored]);
    }
    
    void mergeWithNeighbour(bNode *node) {
        bNode *parent = node->parent;
        int i = 0;
        while (parent->children[i] != node)
            i++;
        bNode *neighbour;
        if (parent->children[i + 1] != nullptr)
            neighbour = parent->children[i + 1]; // merge with right neighbour
        else
        {
            node = parent->children[i - 1]; // merge with left neighbour
            neighbour = parent->children[i];
        }

        if (neighbour->stored == (global_degree - 1) / 2) {
            //node keys + borrowed key + neighbour keys == global_degree - 1
            int j = node->stored;
            int borrowed = parent->keys[i];
            while (i < parent->stored) {
                parent->keys[i] = parent->keys[i + 1];
                parent->children[i] = parent->children[i + 1];
                i++;
            }
            parent->keys[i] = 0;
            parent->children[i] = nullptr;
            parent->stored--;
            node->keys[j] = borrowed;
            node->stored++;
            int k = 0;
            if (neighbour->leaf == true)
            {
                while (k < neighbour->stored) {
                    node->keys[node->stored + k] = neighbour->keys[k];
                    k++;
                }
            } else if (neighbour->leaf == false)
            {
                while (k < neighbour->stored) {
                    node->keys[node->stored + k] = neighbour->keys[k];
                    node->children[node->stored + k + 1] = neighbour->children[k];
                    k++;
                }
            }
            delete neighbour;
        }
        else {
            //node keys + borrowed key + neighbour keys > global_degree - 1
            int borrowed = parent->keys[i];
            int tempArray[global_degree*2]={0},k=0;
            for(int j=0;j<node->stored;j++)
            {
                tempArray[k]=node->keys[j];
                k++;
            }
            tempArray[k]=borrowed;
            k++;
            for(int j=0;j<neighbour->stored;j++)
            {
                tempArray[k]=neighbour->keys[j];
                k++;
            }
            parent->keys[i]=tempArray[k/2 -1];
            for(int j=0;j<node->stored;j++)
                node->keys[j]=0;
            node->stored=0;
            for(int j=0;j<k/2 -1;j++)
            {
                node->keys[j]=tempArray[j];
                node->stored++;
            }
            for(int j=0;j<neighbour->stored;j++)
                neighbour->keys[j]=0;
            neighbour->stored=0;
            for(int j=k/2;j<k;j++)
            {
                neighbour->keys[j-k/2]=tempArray[j];
                neighbour->stored++;
            }
            
        }
    }

    void removeKey(int key) {
        bNode *node = root;
        int ok = 1;
        while (ok) // find node containing if it exists
        {
            ok = 0;
            int i = 0;
            while (node->keys[i] < key && i < node->stored)
                i++;
            if (node->keys[i] == key) {
                ok = 1;
                break;
            } else if (node->children[i] != nullptr) {
                node = node->children[i];
                ok = 1;
            } else cout << "nu exista \n";
        }
        if (ok == 1)//node found
        {
            if (node->leaf == true) {
                int i = 0, j = node->stored;
                while (i < j && node->keys[i] < key)
                    i++; //find key in node
                while (i < j) {
                    node->keys[i] = node->keys[i + 1];
                    i++; //overwrite key in node
                }
                node->stored--;
                if (node->stored == (global_degree - 1) / 2 - 1)
                    mergeWithNeighbour(node); //fix underflow
            } else if (node->leaf == false) {
                int i = 0, j = node->stored;
                while (i < j && node->keys[i] < key)
                    i++; //find key in node
                bNode *donor = findLargestInSubtree(node->children[i]);
                // node containing largest value smaller than key

                int largestVal = donor->keys[donor->stored - 1];
                removeKey(largestVal);
                node->keys[i] = largestVal;
                if (donor->stored == (global_degree - 1) / 2 - 1)
                    mergeWithNeighbour(donor); //fix underflow
            }
        }

    }


    void printSubTree(bNode *node, int height) {
        cout << "leaf: " << node->leaf << " height: " << height << '\n';

        for (int i = 0; i < node->stored; i++)
            cout << node->keys[i] << " ";
        cout << '\n';

        for (int i = 0; i <= node->stored; i++)
            if (node->children[i] != nullptr)
                printSubTree(node->children[i], height + 1);
    }
};

int main() {

    bTree bArbore;
    for (int i = 1; i <= 22; i++) {
        cout << i << " :\n";
        bArbore.insertKey(i, bArbore.getRoot());
        cout << '\n';
    }
    bArbore.removeKey(11);
    bArbore.printSubTree(bArbore.getRoot(), 0);
    cout << '\n';

    bArbore.printInOrder(bArbore.getRoot());
    cout << '\n';
    cout << bArbore.findSuccessor(3);
    cout << '\n';
    cout << bArbore.findPredecessor(3);
    return 0;
}