#include "node.hpp"
#include "debug.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>
bool DEBUG = false;

std::vector<Node *> create_tree_from_core_nodes(std::vector<Node *> nodes)
{
    auto size = nodes.size();

    if (size / 2 == 1)
    {
        auto left = nodes[0];
        auto right = nodes[1];
        receive_value(left, right);  // Left receive value from right
        return {left};
    }

    auto new_nodes = std::vector<Node *>{};

    for (auto i = 0; i < size; i += 2)
    {
        auto left = nodes[i]; 
        auto right = nodes[i + 1];
        receive_value(left, right); // Left receive value from right
        new_nodes.push_back(left);
    }

    return create_tree_from_core_nodes(new_nodes);
}

Node *create_new_tree(std::vector<Node *> nodes)
{
    return create_tree_from_core_nodes(nodes)[0];
}

int main(int argc, char const *argv[])
{

    std::vector<int> data = {8, 19, 7, 15, 7, 13, 12, 14};
    std::vector<Node *> nodes;
    for (auto value : data)
    {
        std::vector<int> *values = new std::vector<int>(1);
        values->at(0) = value;
        nodes.push_back(new Node(values));
    }

    auto root = *create_new_tree(nodes);
    auto expected = accumulate(data.begin(), data.end(), 0); // sum = 95
    auto predict = compute_node(root);

    if (DEBUG)
    {

        print_node(root);
        std::cout << "predict " << predict << " expected " << expected << std::endl;
    }

    assert(predict == expected);

    std::cout << "Test question 3 success" << std::endl;

    return 0;
}
