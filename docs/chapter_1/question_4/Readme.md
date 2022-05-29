# Questão 4

As an alternative to the approach outlined in the preceding problem we can use C’s bitwise operators to implement the tree-structured global sum. In order to see how this works, it helps to write down the binary (base 2) representation of each of the core ranks, and note the pairings during each stage

Semelhante ao questão 3 sendo a diferença utilizar o bitwise __<<__ para dividir
o tamanho atual da função recursiva:

```c++
std::vector<Node *> create_new_tree_bitwise(std::vector<Node *> nodes)
{

    auto size = nodes.size();

    if (size >> 1 == 1) // alteração.
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
```