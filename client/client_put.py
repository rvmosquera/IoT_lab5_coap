# client_put.py
import asyncio
import json
import random
import logging

from aiocoap import *


logging.basicConfig(level=logging.INFO)

async def main():
    context = await Context.create_client_context()
    #alarm_state = random.choice([True, False])
    #payload = b"OFF"

    #if alarm_state:
    #    payload = b"ON"

    #tmp = "{\"temperature\": 46}"
    tmp = "1"
    payload = tmp.encode()
    
    request = Message(code=GET, payload=payload, uri="coap://192.168.174.2/temperature")
    #request = Message(code=PUT, payload=payload, uri="coap://192.168.1.100/light")

    response = await context.request(request).response
    #print('Result: %s\n%r'%(response.code, response.payload))
    print('Result: ', response.payload)

async def mainv2():
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri='coap://192.168.1.100/light')

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print('Result: %s\n%r'%(response.code, response.payload))

if __name__ == "__main__":
    #asyncio.get_event_loop().run_until_complete(main())
    asyncio.run(main())