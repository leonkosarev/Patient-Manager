#ifndef PATIENTMANAGER_SUMTREE_H
#define PATIENTMANAGER_SUMTREE_H

#include "SumTreeNode.h"
#include <new>
#include <stdexcept>
#include <iostream>

// Generic balanced binary search tree. A key may appear more than once.
template <class T>
class SumTree {
    int size = 0;
    SumTreeNode<T>* root = nullptr;

public:
    class SumTreeIterator{
        SumTreeNode<T>* current= nullptr;
    public:
        explicit SumTreeIterator(SumTreeNode<T>* curr = nullptr) : current(curr){}

        SumTreeIterator& operator++(){
            if (!current){
                throw std::out_of_range("Out Of Range");
            } else{
                current = current->nextInOrder();
            }
            return *this;
        }

        bool operator==(const SumTreeIterator& other){
            return current == other.current;
        }

        bool operator!=(const SumTreeIterator& other){
            return !(this->operator==(other));
        }

        const T& operator*(){
            if(!current){
                throw std::out_of_range("Out Of Range");
            }
            return *current->key;
        }

        //Functions for testing:
        void printIterator(std::ostream& os){
            assert(current);
            os << *current->key <<  " | " << current->value << std::endl;
        }
    };

    //Function for testing:
    bool findPrint(T& key, std::ostream& os){
        auto it = find(key);
        if(it!=end()){
            it.printIterator(os);
            return true;
        } else{
            return false;
        }
    }

    SumTreeIterator begin(){
        return SumTreeIterator(root->findMin());
    }

    SumTreeIterator end(){
        return SumTreeIterator();
    }

    ~SumTree(){
        if (root){
            delete root;
        }
    }

    SumTreeIterator find(const T& key){
        if(isEmpty()){
            return SumTreeIterator();
        }
        SumTreeNode<T>* target= root->find(key);
        return SumTreeIterator(target);
    }

    void insert(const T& key){
        SumTreeNode<T> * target = root ? root->find(key) : nullptr;
        if(target){
            target->setValue(target->value+1); // If the key already exists, increment it's quantity.
            return;
        }
        SumTreeNode<T>* node = nullptr;
        node = new SumTreeNode<T>(key);
        if (!root){
            root = node;
            ++size;
        } else{
            if (node!=root->insert(node)){
                delete node;
            } else{
                ++size;
                if (root->parent){
                    root=root->findRoot();
                }
            }
        }
    }

    void remove(const T& key){
        if(!isEmpty()){
            SumTreeNode<T>* target = root->find(key);
            if(target) {
                target->setValue(target->value - 1); // Decrement quantity. Remove if reaches zero.
                if (target->value <= 0) {
                    root = target->remove();
                    --size;
                    if (root && root->parent) {
                        root = root->parent;
                    }
                }
            }
        }
    }

    int countInRange(const T& lower, const T& upper){
        return root->countLessThan(upper)-root->countLessThan(lower);
    }

    bool isEmpty() const {
        return size==0;
    }
};
#endif
