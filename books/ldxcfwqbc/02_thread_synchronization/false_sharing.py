import threading
import random
import functools
import time


NUMS_LEN = 10000


def get_odds_from_each_row(i):
    global nums
    global results
    start = time.perf_counter()
    for j in range(len(nums[i])):
        if nums[i][j] % 2 == 1:
            results[i] += 1
    end = time.perf_counter()
    # print("thread id: ", threading.get_ident(), ", time used: ", end - start)
    times[i] = end - start


def get_odds_from_each_row_1(i):
    global nums
    global results
    start = time.perf_counter()
    count = 0
    for j in range(len(nums[i])):
        if nums[i][j] % 2 == 1:
            count += 1
    results[i] = count
    end = time.perf_counter()
    # print("thread id: ", threading.get_ident(), ", time used: ", end - start)
    times[i] = end - start


if __name__ == "__main__":
    print("build large matrix")
    nums = []
    for i in range(NUMS_LEN):
        nums.append([])
        for j in range(NUMS_LEN):
            nums[i].append(random.randint(1, 10))
    results = [0] * NUMS_LEN
    times = [0] * NUMS_LEN

    threads = [threading.Thread(target=functools.partial(get_odds_from_each_row, i)) for i in range(NUMS_LEN)]
    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()

    print("Total time: ", sum(times))

    results = [0] * NUMS_LEN
    times = [0] * NUMS_LEN
    threads = [threading.Thread(target=functools.partial(get_odds_from_each_row_1, i)) for i in range(NUMS_LEN)]
    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()

    print("Total time: ", sum(times))
