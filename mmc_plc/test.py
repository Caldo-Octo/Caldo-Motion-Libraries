import asyncio
import time
from concurrent.futures import ThreadPoolExecutor
from functools import partial

# _executor = ThreadPoolExecutor(1)
test_lock = asyncio.Lock()
test_var = [1, 2]


def read_write_ads(plc_var):
    update_ = 0
    while update_ < 5:
        update_ = update_+1
        plc_var[0] = update_
        print(f"from simulation: {plc_var[0]}, Lock: {test_lock.locked()}")
        time.sleep(1)


async def print_lock():
    print(test_lock.locked())
    await asyncio.sleep(0.1)


async def ads_comms(vars_list):
    async with test_lock:
        await asyncio.to_thread(read_write_ads, vars_list)


async def update(var_plc):
    async with test_lock:
        while True:
            print(f"from update function {var_plc[0]}")
            await asyncio.sleep(1)


async def main():
    await asyncio.gather(asyncio.to_thread(read_write_ads, test_var), update(test_var), print_lock())


asyncio.run(main())
'''
loop = asyncio.get_event_loop()
#asyncio.run(main())

#loop.run_until_complete(main())
#loop.close()


'''
