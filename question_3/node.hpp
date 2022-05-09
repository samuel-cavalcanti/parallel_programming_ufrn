

#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

class Node
{
public:
    Node(Node *left, Node *right)
    {
        this->neighborhoods.push_back(left);
        this->neighborhoods.push_back(right);
        this->data = nullptr;
    }
    Node(std::vector<int> *data)
    {
        this->data = data;
    }

    ~Node()
    {
        delete this->data;
    }

    std::vector<Node *> neighborhoods;
    std::vector<int> *data;
};

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

#endif // NODE_HPP