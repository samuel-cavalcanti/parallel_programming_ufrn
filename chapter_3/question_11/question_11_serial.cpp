#include <iostream>
#include <vector>
#include <assert.h>

std::vector<int> calculate_n_prefix_sums(std::vector<int> &v, int &n);
int prefix_sum(std::vector<int> &v, int &n);
void test_prefix_sums();

int main(int argc, char const *argv[])
{
    test_prefix_sums();

    return 0;
}

std::vector<int> calculate_n_prefix_sums(std::vector<int> &v, int &n)
{
    assert(n >= 1);

    std::vector<int> result(n);
    result[0] = v[0];

    for (int i = 1; i < n; i++)
        result[i] = result[i - 1] + v[i];

    return result;
}

void test_prefix_sums()
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto n = 1;
    auto prefixs = calculate_n_prefix_sums(v, n);

    assert(prefixs.size() == 1);
    assert(prefixs[0] == v[0]);

    n = 2;
    prefixs = calculate_n_prefix_sums(v, n);

    assert(prefixs.size() == 2);
    assert(prefixs[0] == v[0]);
    assert(prefixs[1] == v[0] + v[1]);

    n = 3;
    prefixs = calculate_n_prefix_sums(v, n);

    assert(prefixs.size() == 3);
    assert(prefixs[0] == v[0]);
    assert(prefixs[1] == v[0] + v[1]);
    assert(prefixs[2] == v[0] + v[1] + v[2]);

    std::cout << "Passed in tests" << std::endl;
}