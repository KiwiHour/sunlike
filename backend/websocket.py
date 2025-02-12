
import websockets, threading, asyncio
from sunlike import Sunlike

HOST = "192.168.1.105"
PORT = 5021

async def handle(path: str, data: str, websocket: websockets.WebSocketClientProtocol, sunlike: Sunlike):
        
    # TODO: request grouping (brightness=20, brightness=21, brightness=22) in a short time, most recent one used
    # Keep requests every 500ms in an array, then remove redundant requests
    
    sunlike.websocket = websocket
    if path == "full-inform":
        config = await sunlike.get_current_config()
        await websocket.send(f"inform-brightness/{config['brightness']}")
        await websocket.send(f"inform-power-state/{'on' if config['device_on'] else 'off'}")
        
    elif path == "config-inform":
        config = sunlike.config_manager.get_config()
        await websocket.send(f"inform-sunrise-time/{config['sunrise_time']}")
    
    elif path == "set-brightness":
        # TODO: validation, 1-100
        await sunlike.set_config(brightness=int(data), ws_inform=False)
    
    elif path == "set-power-state":
        if data == "on":
            await sunlike.device.on()
        elif data == "off":
            await sunlike.device.off()
    
    elif path == "start-duskfall":
        await sunlike.start_duskfall()
            
async def debounceRequests(debounceRequestList: list[tuple[str, str]], websocket: websockets.WebSocketServerProtocol, sunlike: Sunlike):
    
    # Make copy, so that debouncing can continue while this works in the background
    debounceRequestListCopy = list(debounceRequestList)
    debounceRequestList.clear() # Mutably clear the list
    
    # Keeping them seperate so I don't have to map each iteration
    filteredPaths = []
    filteredData = []
    
    # Filter the requests, go in reverse as latest index is the newest
    for path, data in debounceRequestListCopy[::-1]:
        if path in filteredPaths: continue
        filteredPaths.append(path)
        filteredData.append(data)
        
    print(f"Debounced {debounceRequestListCopy}")
    print(f"Debounced result: {list(zip(filteredPaths, filteredData))}")
    
    for path, data in zip(filteredPaths, filteredData):
        await handle(path, data, websocket, sunlike)
    

async def handler(websocket: websockets.WebSocketServerProtocol, sunlike: Sunlike):
    debounceRequestList: list[tuple[str, str]] = [] # Tuple of paths and data
    
    async for message in websocket:
        path, data = message.split("/")
        
        if len(debounceRequestList) == 0: # If nothing to debounce currently
            threading.Timer(0.5, lambda: asyncio.run(debounceRequests(debounceRequestList, websocket, sunlike))).start() # After 500ms, debounce all
            
        debounceRequestList.append((path, data))
        
        

async def start_socket(sunlike: Sunlike):
    server = await websockets.serve(lambda ws: handler(ws, sunlike), HOST, PORT)
    print(f"WebSocket server started on ws://{HOST}:{PORT}")
    await server.wait_closed()
