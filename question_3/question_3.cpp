#include "node.hpp"
#include "debug.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>
bool DEBUG = false;


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
