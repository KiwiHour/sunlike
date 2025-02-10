import { useContext } from "react"
import { SocketContext } from "../page"

export default function DuskfallButton() {

	let socket = useContext(SocketContext)

	function startDuskfall() {
		socket?.current?.sendMessage("start-duskfall", "na")
	}

	return (
		<div style={{ padding: 50 }}>
			<input type="button" value="Start Duskfall" onClick={startDuskfall} />
		</div>
	)
}