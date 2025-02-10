from flask import Flask, request, jsonify
from flask_cors import CORS
from sunlike import Sunlike
import asyncio, threading

class SunlikeAPI(object):
    def __init__(self, sunlike: Sunlike):
        self.api = Flask(__name__)
        self.sunlike = sunlike
        CORS(self.api)
        
        self.host = "0.0.0.0"
        self.port = 5020
        
        @self.api.before_request
        def middleware():
            if request.method == "OPTIONS":
                return '', 200  # Allow preflight requests
            
            data = request.get_json()
            if data.get("password") != "tapobulbinmyass":
                return jsonify({"error": "Nice try dumbass"}), 401

        @self.api.route('/', methods=['POST'])
        async def main():
            return jsonify("SunlikeAPI Authenticated"), 200
        
        @self.api.route("/api/power-state", methods=['POST'])
        async def set_state():
            data = request.json
            if data["power_state"] == "on":
                await self.sunlike.device.on()
            elif data["power_state"] == "off":
                await self.sunlike.device.off()
            else:
                return jsonify(1), 222
            return jsonify(0), 200
        
        @self.api.route("/api/brightness", methods=['POST'])
        async def set_brightness():
            data = request.json
            await self.sunlike.set_config(brightness=float(data["brightness"]))
            
            return jsonify(data["brightness"]), 200
        
        @self.api.route("/api/current-config", methods=["POST"])
        async def get_current_config():
            config = await self.sunlike.get_current_config()
            
            return jsonify(config), 200
        
        @self.api.route("/api/start-duskfall", methods=['POST'])
        async def start_duskfall():
            threading.Thread(target = lambda: asyncio.run(self.sunlike.start_duskfall())).start()
            return jsonify("Duskfall started"), 200
            

    def run(self):
        self.api.run(debug=True, host=self.host, port=self.port, threaded=True)