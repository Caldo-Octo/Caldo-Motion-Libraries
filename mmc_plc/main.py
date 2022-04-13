import asyncio
import time
import pyads
from pymodbus.client.sync import ModbusTcpClient as ModbusClient


class PlcComms:
    def __init__(self, ams_id, plc_vars):
        self.plc_produced = {}
        self.plc_consumed = {}
        self.plc = pyads.Connection(ams_id, pyads.PORT_TC3PLC1)
        self.plc_variables = plc_vars
        self.comms_enabled = False

    def plc_read_write(self, read_write_delay):
        with self.plc:
            while self.comms_enabled:
                self.plc_produced = self.plc.read_list_by_name(self.plc_variables)
                self.plc.write_list_by_name(self.plc_consumed)
                time.sleep(read_write_delay)

    async def async_plc_comms(self, lock):
        async with lock:
            await asyncio.to_thread(self.plc_read_write, 0.1)


class MMCComms:
    def __init__(self, ip_address):
        self.mmc_produced = []
        self.mmc_consumed = []
        self.comms_enabled = False
        self.ip_address = ip_address
        self.mmc = ModbusClient(ip_address)

    def mmc_read_write(self, write_addresses):
        with self.mmc:
            while self.comms_enabled:
                # only write relevant registers
                for address in write_addresses:
                    self.mmc.write_register(address, self.mmc_consumed)
                time.sleep(0.1)
                # read all registers
                self.mmc_produced = self.mmc.read_holding_registers(1, 255)


async def main():
    pass


if __name__ == '__main__':
    plc_variables = []
    mutex_lock = asyncio.Lock()
    plc_client = PlcComms('127.0.0.1.1.1', plc_variables)
    asyncio.run(main())
