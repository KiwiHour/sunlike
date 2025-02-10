from tapo import ApiClient
from dotenv import load_dotenv
from sunlike import Sunlike, get_device
from config_manager import ConfigManager
import websocket

import asyncio, os, threading
        

def start_asyncio_sunlike_loop(loop, task):
    asyncio.set_event_loop(loop)
    loop.run_until_complete(task())

async def main():
    load_dotenv()
    config_manager = ConfigManager("sunlike.config")
    client = ApiClient(
        os.getenv("TAPO_USERNAME"),
        os.getenv("TAPO_PASSWORD"))
    device = await get_device(client)
    sunlike = Sunlike(device, client, config_manager)
    
    
    await sunlike.sync_config()
    
    print(f"Version - {sunlike.version}")
    print("")
    await sunlike.print_config()
    
    sunlike_loop = asyncio.new_event_loop()
    asyncio.set_event_loop(sunlike_loop)
    
    threading.Thread(target=start_asyncio_sunlike_loop, args=(sunlike_loop, sunlike.main_loop,), daemon=True).start()
    await websocket.start_socket(sunlike)
    
    
    
        

if __name__ == "__main__":
    asyncio.run(main())
