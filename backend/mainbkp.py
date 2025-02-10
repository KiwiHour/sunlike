from tapo import ApiClient
from dotenv import load_dotenv
from sunlike import Sunlike, get_device
from api import SunlikeAPI

import asyncio, os, threading


        
        

def start_asyncio_sunlike_loop(loop, task):
    asyncio.set_event_loop(loop)
    loop.run_until_complete(task())

async def main():
    load_dotenv()
    client = ApiClient(
        os.getenv("TAPO_USERNAME"),
        os.getenv("TAPO_PASSWORD"))
    device = await get_device(client)
    sunlike = Sunlike(device, client)
    sunlike_api = SunlikeAPI(sunlike)
    
    await sunlike.sync_config()
    
    print(f"Version - {sunlike.version}")
    print("")
    await sunlike.print_config()
    
    sunlike_loop = asyncio.new_event_loop()
    asyncio.set_event_loop(sunlike_loop)
    
    threading.Thread(target=start_asyncio_sunlike_loop, args=(sunlike_loop, sunlike.main_loop,), daemon=True).start()
    
    sunlike_api.run()
    
    # Syncs the actual values on the bulb to the class attributes
    
    #await sunlike.set_as_defaults()

    # await sunlike.device.set().brightness(75).color_temperature(2500).send(sunlike.device)
    # time.sleep(3)
    # await sunlike.device.on()

    
    #time.sleep(10)
    #await sunlike.linear_gradient_set_config(time_m=0.3, end_brightness=1)
    #await sunlike.device.off()
    
    
    # TODO: use asyncio.gather instead of the .set() stuff, can then use the sunlike methods
    
    
        

if __name__ == "__main__":
    asyncio.run(main())
