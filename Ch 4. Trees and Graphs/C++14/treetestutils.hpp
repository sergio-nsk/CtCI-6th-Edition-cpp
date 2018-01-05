#pragma once

#include <limits>
#include <algorithm>
#include <functional>
#include <queue>
#include <iostream>
#include <iomanip>
#include "tree.hpp"

namespace TestUtils
{
    template <typename T, bool WithParent, typename P>
    void printTree(const Tree<T, WithParent, P> &tree)
    {
        using NodePtr = typename Tree<T, WithParent, P>::NodePtr;

        T minValue = std::numeric_limits<T>::max(), maxValue = std::numeric_limits<T>::min();

        // Lambda function is used to hide it from externall access
        std::function<size_t (const NodePtr &)> getDepth =
        [&](const NodePtr &node) -> size_t
        {
            if (!node)
                return 0;

            auto depth = std::max(getDepth(node->getLeft()), getDepth(node->getRight())) + 1;
            minValue = std::min(minValue, node->getValue());
            maxValue = std::max(maxValue, node->getValue());
            return depth;
        };

        size_t depth = getDepth(tree.getRoot());

        // bottommost max leaf count
        size_t size = std::pow(2, depth - 1);

        // width of node, in digits
        size_t digits = (maxValue != 0 || minValue != 0) ? std::log10(std::max(maxValue, std::abs(minValue))) + 1  : 1;
        if (minValue < 0)
            ++digits;

        const std::string placeholder(digits, ' ');

        std::queue<NodePtr> queue;
        std::queue<NodePtr> childs;
        queue.push(tree.getRoot());
        std::cout << "Tree:" << std::endl;

        do
        {
            // space between nodes
            std::string space((size - 1) * (placeholder.length() + 1) + 1, ' ');

            // margin
            std::cout << std::string(space.length() / 2, ' ');;
            while (!queue.empty())
            {
                if (!queue.front())
                {
                    std::cout << placeholder << space;
                    childs.emplace();
                    childs.emplace();
                }
                else
                {
                    std::cout << std::setw(digits) << std::right << queue.front()->getValue() << space;
                    childs.push(queue.front()->getLeft());
                    childs.push(queue.front()->getRight());
                }
                queue.pop();
            }
            std::cout << std::endl;
            queue.swap(childs);
            size /= 2;
        } while (size > 0);
    }

    // The function treeFromArray from the task 4.2 helps us to fill test trees.
    template <typename T, bool WithParent = false>
    auto treeFromArray(const T *array, size_t size)
    {
        using NodePtr = typename Tree<T, WithParent>::NodePtr;

        std::function<NodePtr (const T *, NodePtr, int, int)> subtreeFromArray =
            [&subtreeFromArray](const T *array, NodePtr parent, int start, int end) -> NodePtr
            {
                if (end < start)
                    return nullptr;

                int i = (start + end) / 2;
                auto node = std::make_shared<Node<T, WithParent>>(array[i], parent);
                node->setLeft(subtreeFromArray(array, node, start, i - 1));
                node->setRight(subtreeFromArray(array, node, i + 1, end));
                return node;
            };

        Tree<T, WithParent> tree;
        tree.setRoot(subtreeFromArray(&array[0], nullptr, 0, size - 1));
        return tree;
    }

    template <typename T, bool WithParent = false>
    auto treeFromArray(std::initializer_list<T> array)
    {
        return treeFromArray<T, WithParent>(array.begin(), array.size());
    }

    template <typename T, bool WithParent = false>
    auto getSampleTree(size_t nodeCount)
    {
        std::vector<T> v(nodeCount) ;
        std::iota(std::begin(v), std::end(v), 0); // Fill with 0, 1, ..., nodeCount - 1.
        return treeFromArray<T, WithParent>(&v[0], v.size());
    }
}
