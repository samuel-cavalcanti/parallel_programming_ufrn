10. Suppose a program must execute 10¹² instructions in order to solve a particular problem. Suppose further that a single processor system can solve the problem in 10⁶ seconds (about 11.6 days). So, on average, the single processor system executes 10⁶ or a million instructions per second. Now suppose that the program has been parallelized for execution on a distributed-memory system. Suppose also that if the parallel program uses p processors, each processor will execute 10¹²  /p instructions and each processor must send 10⁹ ( p − 1) messages. Finally, suppose that there is no additional overhead in executing the
parallel program. That is, the program will complete after each processor has executed all of its instructions and sent all of its messages, and there won’t be any delays due to things such as waiting for messages.  
    1.  Suppose it takes 10⁻⁹ seconds to send a message. How long will it take the program to run with 1000 processors, if each processor is as fast as the single processor on which the serial program was run ?  

    2. Suppose it takes 10⁻³ seconds to send a message. How long will it take the program to run with 1000 processors ?


## main.py

```py
import datetime

NUMBER_OF_INSTRUCTIONS = 10**12
NUMBER_OF_MESSAGES = 10**9
AVERANGE_SECOND_PER_INSTRUCTIONS = (10**6) / NUMBER_OF_INSTRUCTIONS


def cost_time_per_instruction(instructions: int) -> float:
    return AVERANGE_SECOND_PER_INSTRUCTIONS * instructions


def number_of_instructions_per_processor(p: int) -> int:
    return NUMBER_OF_INSTRUCTIONS/p


def number_of_messagens_per_processor(p: int) -> int:
    return NUMBER_OF_MESSAGES * (p-1)


def simulate(time_per_message_in_seconds: float, processors: int):
    print(
        f'time to send a message: {time_per_message_in_seconds} processors: {processors}')
    instructions = number_of_instructions_per_processor(processors)
    number_of_messages = number_of_messagens_per_processor(processors)
    each_process_cost_in_seconds = cost_time_per_instruction(instructions)

    total_messages_in_seconds = time_per_message_in_seconds * number_of_messages

    result = total_messages_in_seconds + each_process_cost_in_seconds
    result_date = datetime.timedelta(seconds=result)

    print(f'executing instructions is {instructions}')
    print(f'spend sending messages is {total_messages_in_seconds}')

    print(f'total time in seconds: {result}')
    print(f'total time in HH:MM:SS {result_date}')


def a():

    time_per_message_in_seconds = 1e-9
    processors = 1e3
    simulate(time_per_message_in_seconds, processors)


def b():
    time_per_message_in_seconds = 1e-3
    processors = 1e3
    simulate(time_per_message_in_seconds, processors)


def main():
    print('A:')
    a()
    print('B:')
    b()


if __name__ == '__main__':
    main()
```
Executando:
```shell
python chapter_2/question_10/main.py
A:
time to send a message: 1e-09 processors: 1000.0
executing instructions is 1000000000.0
spend sending messages is 999.0000000000001
total time in seconds: 1999.0
total time in HH:MM:SS 0:33:19
B:
time to send a message: 0.001 processors: 1000.0
executing instructions is 1000000000.0
spend sending messages is 999000000.0
total time in seconds: 999001000.0
total time in HH:MM:SS 11562 days, 12:16:40
```
11562 dias são 32 anos.
