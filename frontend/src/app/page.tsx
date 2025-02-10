"use client";

import { useEffect, useRef, createContext, RefObject } from "react";
import BrightnessControls from "./components/BrightnessControls";
import DuskfallButton from "./components/DuskfallButton";
import PowerButton from "./components/PowerButton";
import SocketHandler from "./lib/socketHandler";
import SunriseControls from "./components/SunriseControls";

export const SocketContext = createContext<RefObject<SocketHandler | null> | null>(null)
const createSocket = () => new WebSocket("ws://192.168.1.105:5021")

export default function Page() {

	let socketHandlerRef = useRef<SocketHandler>(null)
	const refreshSocket = () => {
		console.log("Trying to refresh socket")
		if (socketHandlerRef?.current) {
			console.log("Refreshing socket...")
			socketHandlerRef.current.socket = createSocket() // Set the SocketHandler's socket to a new one
			console.log("Socket refreshed!")
		}
	}

	useEffect(() => {
		let socket = createSocket()
		socket.addEventListener("close", refreshSocket) // Recreate socket upon failure
		socketHandlerRef.current = new SocketHandler(socket)
	}, [])


	return (
		<main className="control-panel">
			<SocketContext.Provider value={socketHandlerRef}>
				<PowerButton />
				<BrightnessControls />
				<DuskfallButton />
				<SunriseControls />
			</SocketContext.Provider>
		</main>
	)
}