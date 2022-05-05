#include "node.hpp"
#include "debug.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>

bool DEBUG = false;

int compute_data(std::vector<int> *data)
{
    auto total = 0;
    auto size = data->size();
    for (auto i = 0; i < size; i++)
    {
        total += data->at(i);
    }

    return total;
}
int compute_node(Node &node)
{

    int result_data = node.data == nullptr ? 0 : compute_data(node.data);

    for (auto neighborhood : node.neighborhoods)
        result_data += compute_node(*neighborhood);

    return result_data;
}

std::vector<Node *> create_tree(std::vector<Node *> nodes)
{

    auto new_nodes = std::vector<Node *>();

    if (nodes.size() <= 2)
    {
        auto left = nodes[0];
        auto right = nodes[1];
        auto middle = new Node(left, right);
        new_nodes.push_back(middle);
        return new_nodes;
    }

    auto new_size = nodes.size();
    auto last_index = new_size - 1;

    for (auto i = 0; i < last_index; i += 2)
    {
        auto left = nodes[i];
        auto right = nodes[i + 1];
        auto middle = new Node(left, right);
        new_nodes.push_back(middle);
    }

    return create_tree(new_nodes);
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

    auto last_level = create_tree(nodes);
    auto root = *last_level[0];
    auto expected =  accumulate(data.begin(), data.end(), 0); // sum = 95
    auto predict = compute_node(root);

    if (DEBUG)
    {

        print_node(root);
        std::cout << "Numeber of nodes: " << last_level.size() << std::endl;
        std::cout << "predict " << predict << " expected " << expected << std::endl;
    }

    assert(predict == expected);

    std::cout << "Test question 3 success" << std::endl;

    return 0;
}
