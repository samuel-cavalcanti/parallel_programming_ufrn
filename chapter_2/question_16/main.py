
from typing import overload
import numpy as np
from matplotlib import pyplot


def serial_time(units: np.ndarray) -> np.ndarray: return units**2


def parallel_time(units: np.ndarray, cores: np.ndarray) -> np.ndarray:
    serial = serial_time(units)
    return serial/cores + np.log2(cores)


def parallel_time_with_soft_overhead(units: np.ndarray, cores: np.ndarray):
    serial = serial_time(units)
    overhead = units*5
    return serial/cores + overhead


def parallel_time_with_hard_overhead(units: np.ndarray, cores: np.ndarray):
    serial = serial_time(units)
    overhead = serial*3
    return serial/cores + overhead


def generate_n() -> np.ndarray:
    values = np.arange(start=10, stop=330, step=10)

    assert values[0] == 10
    assert values[-1] == 320

    return values


def generate_p() -> np.ndarray:
    exponent = np.arange(8)

    values = 2**exponent

    assert values[0] == 1
    assert values[-1] == 128

    return values


def a():
    n_values = generate_n()
    p_values = generate_p()

    fixed_n_value = np.array([n_values[-1] for _ in p_values])

    simulations = []

    for p_value in p_values:
        fixed_p_values = np.array([p_value for _ in n_values])
        parallel_with_fixed_p = parallel_time(n_values, fixed_p_values)
        simulations.append((p_value, parallel_with_fixed_p))

    parallel_with_fixed_n = parallel_time(fixed_n_value, p_values)

    pyplot.plot(p_values, parallel_with_fixed_n)
    pyplot.title(
        f'Aumentando o valor de $p$, para um $n=${fixed_n_value[0]},\n possível platô em ({p_values[-2]}, {parallel_with_fixed_n[-2]})')
    pyplot.xlabel('Número de cores')
    pyplot.ylabel('Tempo de execução da aplicação em micro segundos')
    pyplot.draw()
    pyplot.savefig('16_a_n_fixed.svg')

    pyplot.figure(len(pyplot.get_fignums()) + 1)

    legends = []
    for fixed_p_value, parallel_with_fixed_p in simulations:
        pyplot.plot(n_values, parallel_with_fixed_p)
        legends.append(f'$p=${fixed_p_value}')

    pyplot.legend(legends)
    pyplot.title(
        'Aumentando o valor de $n$, para um valor fixo $p$')
    pyplot.xlabel('Número de units')
    pyplot.ylabel('Tempo de execução da aplicação em micro segundos')
    pyplot.draw()
    pyplot.savefig('16_a_p_fixed.svg')


def b():
    n_values = generate_n()
    p_values = generate_p()

    soft_overhead_simulations = []
    hard_overhead_simulations = []

    for p_value in p_values:
        fixed_p_values = np.array([p_value for _ in n_values])
        parallel_with_fixed_p = parallel_time_with_soft_overhead(
            n_values, fixed_p_values)
        soft_overhead_simulations.append((p_value, parallel_with_fixed_p))
        parallel_with_fixed_p = parallel_time_with_hard_overhead(
            n_values, fixed_p_values)
        hard_overhead_simulations.append((p_value, parallel_with_fixed_p))


    pyplot.figure(len(pyplot.get_fignums()) + 1)
    legends = []
    for fixed_p_value, parallel_with_fixed_p in soft_overhead_simulations:
        pyplot.plot(n_values, parallel_with_fixed_p)
        legends.append(f'$p=${fixed_p_value}')

    pyplot.legend(legends)
    pyplot.title(
        'Aumentando o valor de $n$, para um valor fixo $p$')
    pyplot.xlabel('Número de units')
    pyplot.ylabel('Tempo de execução da aplicação em micro segundos')
    pyplot.draw()
    pyplot.savefig('16_b_soft_overhead.svg')

    pyplot.figure(len(pyplot.get_fignums()) + 1)

    legends = []
    for fixed_p_value, parallel_with_fixed_p in hard_overhead_simulations:
        pyplot.plot(n_values, parallel_with_fixed_p)
        legends.append(f'$p=${fixed_p_value}')

    pyplot.legend(legends)
    pyplot.title(
        'Aumentando o valor de $n$, para um valor fixo $p$')
    pyplot.xlabel('Número de units')
    pyplot.ylabel('Tempo de execução da aplicação em micro segundos')
    pyplot.draw()
    pyplot.savefig('16_b_hard_overhead.svg')


if __name__ == '__main__':
    a()
    b()
