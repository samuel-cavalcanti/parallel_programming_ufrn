def new_interval(thread_index: int, p: int,  n: int) -> tuple[int, int]:
    """Calculo de indice das threads questão 1"""
    division = n // p
    rest = n % p

    if thread_index < rest:
        first = thread_index * (division + 1)
        last = first + division + 1
    else:
        first = thread_index * division + rest
        last = first + division

    return (first, last)


def input_size_to_normalize_array(n: int) -> list[tuple[int, ] | tuple[int, int]]:
    normalized_array = []
    last_index = n - 1
    for i in range(n//2):
        normalized = (i, last_index - i)
        normalized_array.append(normalized)
    if n % 2 == 1:
        normalized = (n // 2,)
        normalized_array.append(normalized)
    return normalized_array


def normalize_array_to_intervals(normalized_array, p) -> list[tuple[int, int]]:
    intervals = []
    size_normalized_array = len(normalized_array)
    for thread_index in range(p):
        interval = new_interval(thread_index, p, size_normalized_array)
        intervals.append(interval)

    return intervals


def equalize_computation(p: int, n: int) -> list[list[int]]:

    threads_computations = []

    normalized_array = input_size_to_normalize_array(n)

    size_normalized_array = len(normalized_array)

    for thread_index in range(p):
        first, last = new_interval(thread_index, p, size_normalized_array)

        thread_indices = []
        for indices in normalized_array[first:last]:
            if len(indices) == 2:
                thread_indices.append(indices[0])
                thread_indices.append(indices[1])
            else:
                thread_indices.append(indices[0])

        threads_computations.append(thread_indices)

    return threads_computations


def test_with_my_exemple():
    expected_output = [
        [0, 9, 1, 8],
        [2, 7, 3, 6],
        [4, 5]
    ]
    threads_computations = equalize_computation(p=3, n=10)

    for predict, expected in zip(threads_computations, expected_output):
        for predict_index, expected_index in zip(predict, expected):
            assert predict_index == expected_index, f'ERROR\npredict: {predict}\nexpected:{expected}'

    print('Test question 2 success')


if __name__ == '__main__':
    test_with_my_exemple()
