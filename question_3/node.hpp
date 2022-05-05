

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

#endif // NODE_HPP