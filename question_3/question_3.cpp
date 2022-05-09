#include "node.hpp"
#include "debug.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>

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

Node *create_new_tree(std::vector<Node *> nodes)
{

    auto size = nodes.size();

    for (auto i = 0; i < size; i += 2)
    {
        auto left = nodes[i];
        auto right = nodes[i + 1];
        left->neighborhoods.push_back(right);

        if (i != 0)
            nodes[0]->neighborhoods.push_back(left);
    }

    return nodes[0];
}

bool DEBUG = true;

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
