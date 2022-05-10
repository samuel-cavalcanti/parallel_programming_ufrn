#include "../question_3/node.hpp"
#include <iostream>
#include <numeric>
#include <assert.h>

bool DEBUG = false;

std::vector<Node *> create_new_tree_bitwise(std::vector<Node *> nodes)
{

    auto size = nodes.size();

    if (size >> 1 == 1)
    {
        auto left = nodes[0];
        auto right = nodes[1];
        receive_value(left, right);
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

    return create_new_tree_bitwise(new_nodes);
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

    auto root = *create_new_tree_bitwise(nodes)[0];

    auto value = compute_node(root);
    auto expected = accumulate(data.begin(), data.end(), 0); // sum = 95

    if (DEBUG)
    {

        std::cout << "value " << value << " expected " << expected << std::endl;
    }

    assert(value == expected);

    std::cout << "Test question 4 success" << std::endl;

    return 0;
}
