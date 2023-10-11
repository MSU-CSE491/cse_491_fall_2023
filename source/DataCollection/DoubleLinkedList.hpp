#pragma once

#include <iostream>
#include <memory>

namespace DataCollection {

    /// @brief A node to hold data for a doubly linked list
    /// @tparam T Type of data this node holds
    template <typename T>
    class Node {
    public:
        T value;
        std::shared_ptr<Node<T>> next;
        std::shared_ptr<Node<T>> prev;

        Node(const T& val) : value(val), next(nullptr), prev(nullptr) { }
    };

    /// @brief Implementation of a doubly linked list
    /// @tparam T Type of the data this list will hold
    template<typename T>
    class DoubleLinkedList {
        private:
            int size = 0;
            std::shared_ptr<Node<T>> head;
            std::shared_ptr<Node<T>> tail;

        public:
            DoubleLinkedList(): head(nullptr), tail(nullptr) { }

            /// @brief Get the size of this list
            /// @return Size of this list as an int
            int getSize() const {
                return size;
            }

            /// @brief Get the head node for this list
            /// @return Shared pointer to the head node
            std::shared_ptr<Node<T>> getHead() const {
                return head;
            }

            /// @brief Get the tail node for this list
            /// @return Shared pointer to the tail node
            std::shared_ptr<Node<T>> getTail() const {
                return tail;
            }

            /// @brief Create a new node by value and add it to the back of the list
            /// @param val Value for the new node to add
            void add_node(const T& val) {
                auto node = std::make_shared<Node<T>>(val);

                if (tail == nullptr) {
                    head = node;
                    tail = node;
                } else {
                    node->prev = tail;
                    tail->next = node;
                    tail = node;
                }

                size++;
            }

            /// @brief Remove a node from the back of the list
            /// @return A shared pointer to the removed node else nullptr
            std::shared_ptr<Node<T>> remove_node() {
                if (tail != nullptr) {
                    std::shared_ptr<Node<T>> removedNode = tail;
        
                    if (size == 1) {
                        head = nullptr;
                        tail = nullptr; 
                    } else {
                        tail = tail->prev;
                        tail->next = nullptr;
                    }

                    size--;

                    return removedNode;
                }

                return nullptr;
            }
    };


}