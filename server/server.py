import aiocoap.resource as resource
import aiocoap
import threading
import logging
import asyncio
import json

class temperatureResource(resource.ObservableResource):
    """This resource supports the GET and PUT methods and is observable.
    GET: Return current state of temperature
    PUT: Update state of temperature and notify registered observers
    """

    def __init__(self):
        super().__init__()

        self.status = 0
        self.humidity = 0
        self.has_observers = False
        self.notify_observers = False

    # Ensure observers are notify if required
    def notify_observers_check(self):
        while True:
            if self.has_observers and self.notify_observers:
                print('Notifying observers')
                self.updated_state()
                self.notify_observers = False

    # Observers change event callback
    def update_observation_count(self, count):
        if count:
            self.has_observers = True
        else:
            self.has_observers = False

    # Handles GET request or observer notify
    async def render_get(self, request):
        response = {}
        tmp = str(self.status)
        tmp2 = str(self.humidity)
        print('Return temperature state: %s' % tmp)
        response['temperature'] = tmp
        response['humidity'] = tmp2
        json_tmp = json.dumps(response)
        #payload = b'%s' % tmp.encode('ascii')
        payload = json_tmp.encode('ascii')

        return aiocoap.Message(payload=payload)

    # Handles PUT request
    async def render_put(self, request):
        #self.status =  request.payload.decode('ascii')
        msgJson = json.loads(request.payload.decode('ascii'))
        self.status = msgJson["temperature"]
        self.humidity = msgJson["humidity"]
        #hum = msgJson["humidity"]
        print('Receiving temperature %s and humidity %s' % (self.status, self.humidity) )
        self.notify_observers = True
        response = str(self.status) + ',' + str(self.humidity)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=b'%s' % response.encode("ascii"))

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.INFO) #logging.DEBUG

def main():
    # Resource tree creation
    root = resource.Site()
    #ipserver = '10.100.229.33' #'192.168.174.2'
    tempResource = temperatureResource()
    root.add_resource(['temperature'], tempResource)
    asyncio.Task(aiocoap.Context.create_server_context(root, bind=('192.168.174.2', 5683)))
    #asyncio.Task(aiocoap.Context.create_server_context(root, bind=('0.0.0.0', 5683)))

    # Spawn a daemon to notify observers when temperature status changes
    observers_notifier = threading.Thread(target=tempResource.notify_observers_check)
    observers_notifier.daemon = True
    observers_notifier.start()

    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()
    #loop = asyncio.new_event_loop()
    #asyncio.set_event_loop(loop)
    #try:
    #    asyncio.run(main(loop=loop))
    #except KeyboardInterrupt:
    #    pass