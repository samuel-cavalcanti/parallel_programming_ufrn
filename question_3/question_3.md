3. Try to write pseudo-code for the tree-structured global sum illustrated in
Figure 1.1. Assume the number of cores is a power of two (1, 2, 4, 8, . . . ).


Para criar a árvore, foi considerado que o vetor principal já foi igualmente espaçado entre as _p_ threads, usando o algoritmo da questão 1.


## Representação de um Nó

Neste caso foi representado o nó, como uma estrutura que possui um vetor de vizinhos e outro ponteiro para um vetor de inteiros, na prática,
o ponteiro para um vetor de inteiros, seria usando o design pattern chamado **Future**, ou um **Option\<Future\>**. 

Também foi criado dois construtores um construtor que representa,
a inicialização do Nó por meio dos seus vizinhos a esquerda e direita,
usando na criação de nós intermediários  da árvore, e a inicialização
do Nó por meio do seu dado, os nós inicializados por meio dos dados
abstrai os **núcleos** ou as **threads** que estão sendo usadas para
executar o algoritmo de alto custo computacional. 

```c++
class Node
{
public:
    std::vector<Node *> neighborhoods;
    std::vector<int> *data;
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
};

```
## Função recursiva que cria a Árvore

Para criar a Árvore foi feita uma função recursiva que
a partir do nível mais baixo da árvore cria a raiz, ou seja,
a partir um vetor com _p_ Nós ,a função vai sendo chamada recursivamente,
onde a cada chamada vai-se criando um nível acima da árvore, até
que se atinja a raiz, onde a cada nível o número de nós é dividido
por 2. **Caso o número de nós inicial não for divisível pro 2, o algoritmo não funciona**

```c++
std::vector<Node *> create_tree(std::vector<Node *> nodes)
{

    auto new_nodes = std::vector<Node *>();

    if (nodes.size() == 2) // Caso BASE
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
    // passando o próximo nível até chegar em 2
    return create_tree(new_nodes);
}
```