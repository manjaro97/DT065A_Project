import datetime
import logging

import asyncio

import aiocoap.resource as resource
import aiocoap

import random

HOST = '127.0.0.1'
PORT = 5683

class GetTemp(resource.Resource):
    
    async def render_get(self, request):

        payload = str(int(random.gauss(20, 2))).encode('utf-8')
        print("\n\n")
        print(datetime.datetime.now())
        print("\n\n")
        return aiocoap.Message(payload=payload)

class GetHum(resource.Resource):
    
    async def render_get(self, request):

        payload = str(int(random.gauss(40, 5))).encode('utf-8')
        return aiocoap.Message(payload=payload)

# logging setup

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)

async def main():
    # Resource tree creation
    root = resource.Site()

    root.add_resource(['temperature'], GetTemp())
    root.add_resource(['humidity'], GetHum())

    await aiocoap.Context.create_server_context(bind=(HOST,PORT),site=root)

    # Run forever
    await asyncio.get_running_loop().create_future()

if __name__ == "__main__":
    asyncio.run(main())