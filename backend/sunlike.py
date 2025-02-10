from tapo import ColorLightHandler, ApiClient
from config_manager import ConfigManager

import asyncio, time, math, threading, time, datetime, os, websockets

async def get_device(client: ApiClient):
    return await client.l530(os.getenv("TAPO_IP_ADDRESS"))

def clamp(x, minimum, maximum):
    return max(minimum, min(x, maximum))

# TODO: DELETE THIS, SHOULD BE VOID ONCE THE APP IS MADE
# Eventually once the app is made, pressing the "on" button will turn on and then set the unchanged config
# No need to poll for the light to be on when you know exactly when the button is pressed
# App will most likely just have an on/off button, with a config to change datetime certain stuff happens (along with the config values)

# TODO: fully transition to WebSocket
# TODO: Add hue and saturation controls to frontend
# TODO: Code the formulas for sunrise, sunset and duskfall and switch to them over time event based
# TODO: Add configurable sunset and sunrise start and end times that update the backend real time (allow for one time temporary changes, eg. i need to stay up late and need a quick sunset for one day)
# TODO: Add "Pause/Unpause sunset" button 
# TODO: Need some way to stop it? otherwise you get locked out from editing during a sunrise/sunset
# TODO: Make sure duskfall overrides sunset  
# TODO: See discord for rest of info

# I GET TO LEARN HOW TO MAKE A MOBILE APP (react native :pensive:)
def thread_target(loop, function):
    asyncio.set_event_loop(loop)
    loop.run_until_complete(function())

class Sunlike(object):
    def __init__(self, device: ColorLightHandler, client: ApiClient, config_manager: ConfigManager):
        self.websocket: websockets.WebSocketServerProtocol = None
        self.config_manager = config_manager
        self.client = client
        self.device = device
        self.version = "Sunlike 0.0.4a"
        self.MAX_STEPS_PER_M = 120
        self.unsynced_config_changes = False
        self.most_recent_config_type = None
        
        # --- Sunset config (your job to make sure duration doesn't go past next phase's start time) ---
        self.SUNSET_TIME_1 = (20, 00) # Light becomes slowly lower color_temp, much more yellow
        self.SUNSET_DURATION_1 = 30
        self.SUNSET_BRIGHTNESS_1 = 70
        self.SUNSET_COLOR_TEMP_1 = 4000
        self.SUNSET_HUE_1 = 30
        self.SUNSET_SATURATION_1 = 80
        
        self.SUNSET_TIME_2 = (21, 00) # Light becomes orange
        self.SUNSET_DURATION_2 = 45
        self.SUNSET_BRIGHTNESS_2 = 50
        self.SUNSET_COLOR_TEMP_2 = 3500
        self.SUNSET_HUE_2 = 30
        self.SUNSET_SATURATION_2 = 80
        
        self.SUNSET_TIME_3 = (22, 00) # Light becomes red
        self.SUNSET_DURATION_3 = 30
        self.SUNSET_BRIGHTNESS_3 = 70
        self.SUNSET_COLOR_TEMP_3 = 4000
        self.SUNSET_HUE_3 = 30
        self.SUNSET_SATURATION_3 = 80
        
        self.SUNSET_TIME_END = (22, 30) # Light slowly dims and then turns off
        self.SUNSET_DURATION_END = 15
        
        # Defaults
        self.DEFAULT_BRIGHTNESS = 100
        self.DEFAULT_COLOR_TEMP = 2900
    
    async def main_loop(self):
        while True:

            try:
                now = datetime.datetime.now()
                
                if now.hour == 9 and now.minute == 30:
                    await self.device.on() # Make sure to turn on first
                    await asyncio.sleep(0.5) # wait for unsaved changes to update
                    await self.set_config(brightness=1, color_temp=2500) # Set to lowest values, so it starts low
                    # TODO: learn how to transfer from hue to color_temp at lower brightness for wakeup
                    # That way i can start at red, stay orange mostly, bit of yellow, then go back to temp for rest of the day
                    await self.linear_gradient_set_config(time_m=60, end_brightness=100, end_color_temp=2900)
                
                if now.hour == 20 and now.minute == 00:
                    await self.linear_gradient_set_config(time_m=55, end_brightness=75, end_color_temp=2500)
                    await asyncio.sleep(30)
                    await self.shift_from_temp_to_hue()
                    
                if now.hour == 21 and now.minute == 00:
                    await self.linear_gradient_set_config(time_m=55, end_brightness=60, end_hue=25, end_saturation=60)
                
                if now.hour == 22 and now.minute == 00:
                    await self.linear_gradient_set_config(time_m=25, end_brightness=35, end_hue=20, end_saturation=60)
                    
                if now.hour == 22 and now.minute == 45:
                    await self.linear_gradient_set_config(time_m=25, end_brightness=20, end_hue=15, end_saturation=80)
                    
                await asyncio.sleep(60)
                
            # Except below the sleep, so another chance to trigger event is possible
            except Exception as ex:
                if "connect error" in str(ex):
                    # Refresh device
                    device = get_device(self.client)
                    self.device = device
                else:
                    raise ex
    
    # THIS IS BAD CODE SEE TOP TODO FOR FIX
    async def wait_until_on_and_make_changes(self):
        while True:
            if (await self.get_current_config())["device_on"]:
                set_values = {"brightness": self.brightness}
                
                if self.most_recent_config_type == "color_temp":
                    set_values["color_temp"] = self.color_temp
                elif self.most_recent_config_type == "hue_sat":
                    set_values["hue"] = self.hue
                    set_values["saturation"] = self.saturation
                else:
                    print("Shouldn't see this, ever.")
                
                await self.set_config(**set_values)
                self.unsynced_config_changes = False
                return
            else:
                time.sleep(0.25)

    async def start_duskfall(self):
        await self.linear_gradient_set_config(5, end_brightness=1)
        await self.device.off()
    
    async def sync_config(self):
        current_config = await self.get_current_config()
        self.brightness = current_config["brightness"]
        self.color_temp = current_config["color_temp"]
        self.hue = current_config["hue"]
        self.saturation = current_config["saturation"]
        
    async def get_current_config(self):
        device_info = await self.device.get_device_info_json()
        return device_info
        
    async def start_sunset_phase_1(self):
        pass
    
    async def linear_gradient_set_config(self, time_m: float, end_brightness: float = None, end_color_temp: float = None, end_hue: float = None, end_saturation: float = None):
        
        current_config = await self.get_current_config()
                
        total_steps = min(
            self.MAX_STEPS_PER_M * time_m,
            max(
                1,
                abs(current_config["brightness"]  - (end_brightness or current_config["brightness"] )),
                abs(current_config["color_temp"]  - (end_color_temp or current_config["color_temp"] )),
                abs(current_config["hue"]  - (end_hue or current_config["hue"] )),
                abs(current_config["saturation"]  - (end_saturation or current_config["saturation"] ))
            )
        )
        
        adjust_params = {}
        
        if end_brightness:
            adjust_params["brightness_delta"] = (end_brightness - current_config["brightness"]) / total_steps
        if end_color_temp:
            adjust_params["color_temp_delta"] = (end_color_temp - current_config["color_temp"]) / total_steps
        if end_hue:
            adjust_params["hue_delta"] = (end_hue - current_config["hue"]) / total_steps
        if end_saturation:
            adjust_params["saturation_delta"] = (end_saturation - current_config["saturation"]) / total_steps
            
        print(adjust_params)
        
        sleep_length = round((time_m / total_steps) * 60, 2)
        print(f"Sleep: {sleep_length}, Steps: {total_steps}")
        for _ in range(math.ceil(total_steps)):
            await self.adjust_config(**adjust_params)
            
            await asyncio.sleep(sleep_length)
            

    async def adjust_config(self, brightness_delta: float = None, color_temp_delta: float = None, hue_delta: float = None, saturation_delta: float = None):
        
        set_params = {}

        if brightness_delta:
            set_params["brightness"] = (self.brightness + brightness_delta)
        if color_temp_delta:
            set_params["color_temp"] = (self.color_temp  + color_temp_delta)
        if hue_delta:
            set_params["hue"] = (self.hue  + hue_delta)
        if saturation_delta:
            set_params["saturation"] = (self.saturation  + saturation_delta)
        
        await self.set_config(**set_params)
            
    # Current values work shifting from 2500K at 75 brightness
    # TODO: Maybe can figure out an equation to convert a range of valid color_temps to hue?
    async def shift_from_temp_to_hue(self):
        await self.set_config(brightness=86, hue=47, saturation=3)
        
    async def ws_full_inform(self):
        
        if not self.websocket:
            return
        
        try:
   
            config = await self.get_current_config()
            await self.websocket.send(f"inform-brightness/{config['brightness']}")
            await self.websocket.send(f"inform-power-state/{'on' if config['device_on'] else 'off'}")
        
        except websockets.exceptions.ConnectionClosedError:
            # If no websocket connected, then just ignore
            print("Errorful websocket closure")
            return
        except websockets.exceptions.ConnectionClosed:
            return
        except websockets.exceptions.ConnectionClosedOK:
            return

    async def set_config(self, brightness: float = None, color_temp: float = None, hue: float = None, saturation: float = None):
        
        
        
        try:  
            
            info_params = self.device.set()
            
            if brightness:
                self.brightness = brightness
                info_params = info_params.brightness(round(brightness))
            if color_temp:
                self.color_temp = color_temp
                info_params = info_params.color_temperature(round(color_temp))
                self.most_recent_config_type = "color_temp"
            if hue:
                self.hue = hue
                info_params = info_params.hue_saturation(round(hue), round(self.saturation))
                self.most_recent_config_type = "hue_sat"
            if saturation:
                self.saturation = saturation
                info_params = info_params.hue_saturation(round(self.hue), round(saturation))
                self.most_recent_config_type = "hue_sat"
            if not brightness and not color_temp and not hue and not saturation:
                return
            
            
            # TODO: See top, will be void after app
            is_off = not (await self.get_current_config())["device_on"]
            # Internal values are set to desired level, but no request is sent to over the network.
            # Once the device it turned back on, a listener will set immediatly with the correct values
            # Also check that we haven't already started a "is_on" listener
            if is_off:
                if not self.unsynced_config_changes:
                    self.unsynced_config_changes = True
                    new_loop = asyncio.new_event_loop()
                    threading.Thread(target=thread_target, args=(new_loop, self.wait_until_on_and_make_changes,), daemon=True).start()
                    

                return
                                
            # Rounding allows to keep an internal, more accurate, floating point value. Which is fitted to the int requirement at the latest possible point
            await info_params.send(self.device)
            # If not on, then I must have turned it off manually (aka not at home)
            # So then you keep it off, but change the setting

            
            await self.ws_full_inform()
            await self.print_config()
            
            

        except Exception as e:
            print("Failed to set config in 'set_config'")
            raise e
        
    async def set_as_defaults(self):
        await self.set_config(brightness=self.DEFAULT_BRIGHTNESS, color_temp=self.DEFAULT_COLOR_TEMP)
        
    async def print_config(self):
        current_config = await self.get_current_config()
        print((f"Brightness: {current_config['brightness']}\n"
              f"Temperature: {current_config['color_temp']}\n"
              f"Hue-sat: ({current_config['hue']}, {current_config['saturation']})"))