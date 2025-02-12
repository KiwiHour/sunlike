import { ChangeEvent, useContext, useEffect, useRef, useState } from "react"
import { brightnessStore } from "../stores"
import { SocketContext } from "../page"

export default function BrightnessControls() {

	let { brightness, setBrightness } = brightnessStore()
	let hasRendered = useRef(false)
	const socketRef = useContext(SocketContext)

	async function handleBrightnessChange(event: ChangeEvent<HTMLInputElement>) {
		let newBrightness = Number(event.target.value)
		setBrightness(newBrightness)
		socketRef?.current?.sendMessage("set-brightness", `${event.target.value}`)
	}



	return (
		<div>
			<div style={{ display: "flex", flexDirection: "row", alignItems: "center" }}>
				<p style={{ minWidth: 25 }}>{brightness}</p>
				<input type="range"
					min={1}
					max={100}
					className="brightness-slider"
					style={{ cursor: "pointer", height: "10px" }}
					value={brightness}
					onChange={handleBrightnessChange} />
			</div>
			{/* <input type="button" value="Set Brightness" onClick={() => { socketRef?.current?.sendMessage("set-brightness", `${brightness}`) }} /> */}
		</div>
	)
}
