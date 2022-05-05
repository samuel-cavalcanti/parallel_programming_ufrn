
#include <iostream>
#include <vector>
#include "node.hpp"


void print_vector(std::vector<int> *data)
{
    std::cout << "data: ";

    if (data == nullptr)
    {
        std::cout << "NULL" << std::endl;
        return;
    }

    auto size = data->size();

    std::cout << "[";
    for (auto i = 0; i < size; i++)
    {
        std::cout << data->at(i) << " ";
    }
    std::cout << "]" << std::endl;
}

void print_node(Node &e)
{

    std::cout << "Node { " << std::endl;

    print_vector(e.data);

    if (e.neighborhoods.empty())
        return;

    std::cout << "neighborhood { " << std::endl;
    for (auto &node : e.neighborhoods)
    {
        print_node(*node);
    }

    std::cout << "}" << std::endl;
}