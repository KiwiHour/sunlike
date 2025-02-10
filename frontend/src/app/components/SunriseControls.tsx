import { useState } from "react"
import { getSunriseTime, sunriseTimeStore } from "../stores"

export default function SunriseControls() {

	let { sunriseTime, setSunriseTime } = sunriseTimeStore()

	return <>
		<input type="number" min={0} max={23} defaultValue={sunriseTime[0]} onChange={(e) => { }} />
		<input type="number" min={0} max={59} defaultValue={sunriseTime[1]} onChange={(e) => { }} />
	</>
}