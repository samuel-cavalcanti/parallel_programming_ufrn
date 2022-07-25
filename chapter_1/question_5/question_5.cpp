#include "../question_3/node.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <assert.h>

bool DEBUG = false;

std::vector<Node *> create_new_tree_bitwise(std::vector<Node *> nodes)
{

    auto size = nodes.size();

    if (size == 2)
    {
        auto left = nodes[0];
        auto right = nodes[1];
        receive_value(left, right);
        return {left}; // Construtor C++ Moderno.
    }
    if (size == 3)
    {
        auto left = nodes[0];
        auto middle = nodes[1];
        auto right = nodes[2];
        receive_value(left, middle); // Left receive value from middle
        receive_value(left, right);  // Left receive value from right
        return {left};               // Construtor C++ Moderno.
    }

    auto new_nodes = std::vector<Node *>{};

    if (size % 2 != 0)
    {
        size = size - 1;
        new_nodes.push_back(nodes[size]);
    }

    for (auto i = 0; i < size; i += 2)
    {
        auto left = nodes[i];
        auto right = nodes[i + 1];
        receive_value(left, right); // Left receive value from right
        new_nodes.push_back(left);
    }

    return create_new_tree_bitwise(new_nodes);
}

Node *map_to_node(int value)
{
    std::vector<int> *values = new std::vector<int>(1);
    values->at(0) = value;
    return new Node(values);
}

int main(int argc, char const *argv[])
{

    std::vector<std::vector<int>> data_tests = {
        {8, 19, 7, 15, 7, 13, 12, 14},
        {8, 19, 7, 15, 7, 13, 12},
        {8, 19, 7, 15, 7, 13},
        {8, 19, 7, 15, 7},
        {8, 19, 7, 15},
        {8, 19, 7},
        {8, 19},
    };

    for (auto &data : data_tests)
    {

        std::vector<Node *> nodes(data.size());
        std::transform(data.begin(), data.end(), nodes.begin(), map_to_node);

        auto root = *create_new_tree_bitwise(nodes)[0];

        auto value = compute_node(root);
        auto expected = accumulate(data.begin(), data.end(), 0); // sum = 95

        if (DEBUG)
        {

            std::cout << "value " << value << " expected " << expected << std::endl;
        }

        assert(value == expected);
    }

    std::cout << "Test question 5 success" << std::endl;

    return 0;
}
