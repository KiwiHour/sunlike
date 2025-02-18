import { useContext, useState } from "react"
import { getSunriseTime, sunriseTimeStore } from "../stores"
import { SocketContext } from "../page"

export default function SunriseControls() {

	const socket = useContext(SocketContext)
	let { sunriseTime, setSunriseTime } = sunriseTimeStore()

	const padTime = (val: number) => val.toString().padStart(2, "0")

	function updateSunriseTime() {
		socket?.current?.sendMessage("set-sunrise-time", sunriseTime.join(":"))
	}

	return <>
		<input type="number" min={0} max={23} defaultValue={padTime(sunriseTime[0])} onChange={(e) => { setSunriseTime([Number(e.target.value), sunriseTime[1]]) }} />
		<input type="number" min={0} max={59} defaultValue={padTime(sunriseTime[1])} onChange={(e) => { setSunriseTime([sunriseTime[0], Number(e.target.value)]) }} />
		<input type="button" value="Set sunrise time" onClick={updateSunriseTime} />
	</>
}