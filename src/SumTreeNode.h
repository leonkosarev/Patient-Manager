#ifndef PATIENTMANAGER_SUMTREENODE_H
#define PATIENTMANAGER_SUMTREENODE_H

#include <algorithm>
#include <assert.h>

template <class T> class SumTree; // forward declaration

template <class T>
class SumTreeNode {
    int height = 0;
    SumTreeNode * left = nullptr;
    SumTreeNode * right = nullptr;
    SumTreeNode * parent = nullptr;
    T * key;
    int value = 1; // Represents the quantity of the key.
    int subtreeSum; // Sum of all quantities in the subtree. Used for efficient countInRange.

    explicit SumTreeNode(const T& key, SumTreeNode * parent = nullptr, int sum = 1)
    : key(new T(key)), parent(parent), subtreeSum(sum) {}

    ~SumTreeNode(){
        if (left){
            delete left;
        }
        if (right){
            delete right;
        }
        delete key;
    }

    static int getHeight(SumTreeNode * node){
        return node ? node->height : -1;
    }

    static int getBalance(SumTreeNode * node){
        SumTreeNode * s;
        return node ? node->getHeight(node->left) - node->getHeight(node->right) : 0;
    }

    static int getSum(SumTreeNode * node){
        return node ? node->subtreeSum : 0;
    }

    int getBalance(){
        return getBalance(this);
    }

    void setRight(SumTreeNode* other){
        right = other;
        if(other){
            other->parent = this;
        }
    }

    void setLeft(SumTreeNode* other){
        left=other;
        if(other){
            other->parent=this;
        }
    }
    
    // LL, RR , RL , LR - Similar to an AVL tree.
    
    void LL(){
        SumTreeNode* curr = this;
        SumTreeNode* par = parent;
        SumTreeNode* l = curr->left;
        SumTreeNode* r = l->right;

        curr->setLeft(r);
        l->setRight(curr);
        l->parent = par;
        if (par){
            if (par->left==curr){
                par->left=l;
            } else{
                par->right =l;
            }
        }
        curr->fixHeight();
        curr->fixSum();
        l->fixHeight();
        l->fixSum();
    }

    void RR() {
        SumTreeNode* curr = this;
        SumTreeNode* par = this->parent;
        SumTreeNode* r = curr->right;
        SumTreeNode* l = r->left;

        curr->setRight(l);
        r->setLeft(curr);
        r->parent = par;
        if (par) {
            if (par->right == curr)
                par->right = r;
            else
                par->left = r;
        }
        curr->fixHeight();
        curr->fixSum();
        r->fixHeight();
        r->fixSum();
    }

    void LR()  {
        left->RR();
        LL();
    }
    void RL()  {
        right->LL();
        RR();
    }

    void fixHeight(){
        height = std::max(getHeight(left),getHeight(right)) + 1;
    }

    void fixSum(){
        subtreeSum = value;
        if(left){
            subtreeSum += left->subtreeSum;
        }
        if(right){
            subtreeSum += right->subtreeSum;
        }
    }

    SumTreeNode* insert(SumTreeNode *node){
        if (*key == *(node->key)){
            return this;
        }
        if(*(node->key) < *key){
            if(!left){
                setLeft(node);
                left->fixUpwards();
            } else{
                left->insert(node);
            }
        } else {
            if(!right){
                setRight(node);
                right->fixUpwards();
            } else{
                right->insert(node);
            }
        }
        return node;
    }

    SumTreeNode* find(const T& val){
        if (*key == val){
            return this;
        }
        if(val < *key){
            if(!left){
                return nullptr;
            }
            return left->find(val);
        } else{
            if(!right){
                return nullptr;
            }
            return right->find(val);
        }
    }

    void setValue(int val){
        value = val;
        fixUpwards();
    }

    SumTreeNode* remove(){
        SumTreeNode* root = nullptr;
        if(parent || left || right){
            if (left && right){
                swapWithSuccessor();
                root= findRoot();
            }
            if (parent){
                bool isLeftChild = (parent->left == this);
                if (!left && !right){
                    isLeftChild ? parent->left= nullptr : parent->right= nullptr;
                }
                if (left && !right){
                    isLeftChild ? parent->left = left : parent->right = left;
                    left->parent = parent;
                }
                if(right && !left){
                    isLeftChild ? parent->left = right : parent->right=right;
                    right->parent = parent;
                }
                root = findRoot();
                parent->fixUpwards();
            } else{
                if((left && !right)){
                    left->parent = nullptr;
                    root = left;
                    left->fixNode();
                }
                if (right && !left){
                    right->parent = nullptr;
                    root = right;
                    right->fixNode();
                }
            }
        }
        left = right = nullptr;
        delete this;
        return root;
    }

    void fixNode(){
        fixHeight();
        int balance = getBalance();
        if(balance>1){
            if (left->getBalance() >=0){
                LL();
            } else{
                LR();
            }
        }
        if(balance<-1){
            if (right->getBalance() <= 0){
                RR();
            } else{
                RL();
            }
        }
        fixSum();
    }

    void fixUpwards(){
        SumTreeNode* next = parent;
        fixNode();
        if(next){
            next->fixUpwards();
        }
    }
    
    // Find the minimum node in the subtree.
    SumTreeNode* findMin(){
        SumTreeNode<T>* target = this;
        while (target){
            if (!target->left){
                break;
            }
            target=target->left;
        }
        return target;
    }

    void swapWithSuccessor(){
        if (!right){
            return;
        }
        SumTreeNode* succ= right->findMin();
        SumTreeNode* oldLeft = left;
        SumTreeNode* oldRight = right;
        SumTreeNode* oldParent = parent;

        setRight(succ->right);
        setLeft(nullptr);

        succ->setLeft(oldLeft);

        if(succ == oldRight){
            succ->setRight(this);
        } else{
            succ->setRight(oldRight);
            succ->parent->setLeft(this);
        }

        succ->parent = oldParent;
        if (oldParent){
            oldParent->left == this ? oldParent->left = succ
                                    : oldParent->right = succ;
        }
    }

    SumTreeNode* findRoot(){
        return parent ? parent->findRoot() : this;
    }
    
    // Returns the total quantity of keys less than val.
    int countLessThan(const T& val){ 
        SumTreeNode* current = this;
        int sum = 0;
        while (current){
            while (current && *current->key > val){
                current = current->left;
            }
            if (current && *current->key <= val){
                sum += current->subtreeSum;
            }
            while (current && *current->key <= val){
                current = current->right;
            }
            if (current && *current->key > val){
                sum -= current->subtreeSum;
            }
        }
        return sum;
    }
    
    // Find the next node in order
    SumTreeNode* nextInOrder(){ 
        SumTreeNode* target = this;
        if(right){
            target = right;
            while(target->left){
                target = target->left;
            }
        } else{
            while (target->parent && target == target->parent->right){
                target = target->parent;
            }
            target = target->parent;
        }
        return target;
    }

    friend SumTree<T>;
};
#endif
