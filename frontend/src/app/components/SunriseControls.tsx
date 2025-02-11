import { useState } from "react"
import { getSunriseTime, sunriseTimeStore } from "../stores"

export default function SunriseControls() {

	let { sunriseTime, setSunriseTime } = sunriseTimeStore()

	const padTime = (val: number) => val.toString().padStart(2, "0")

	return <>
		<input type="number" min={0} max={23} defaultValue={padTime(sunriseTime[0])} onChange={(e) => { }} />
		<input type="number" min={0} max={59} defaultValue={padTime(sunriseTime[1])} onChange={(e) => { }} />
	</>
}