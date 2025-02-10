import { useContext, useEffect, useState } from "react"
import { powerStateStore } from "../stores"
import { SocketContext } from "../page"

export default function PowerButton() {

	const POWER_BUTTON_DIAMETER = 100
	const socketRef = useContext(SocketContext)

	function getPowerButtonSVGPath() {
		return `power-button-${powerState}.svg`
	}

	function togglePowerState() {
		setPowerState(powerState == "on" ? "off" : "on")
	}

	let { powerState, setPowerState } = powerStateStore()
	let powerButtonSVGPath = getPowerButtonSVGPath()

	useEffect(() => {
		powerButtonSVGPath = getPowerButtonSVGPath();
		socketRef?.current?.sendMessage("set-power-state", powerState)

	}, [powerState])

	return (
		<div>
			<img
				src={powerButtonSVGPath} alt="Power Button"
				width={POWER_BUTTON_DIAMETER} height="auto"
				style={{ cursor: "pointer", borderRadius: `${Math.round(POWER_BUTTON_DIAMETER / 2)}px` }}
				onClick={togglePowerState}
			/>
		</div >
	)
}