import { setBrightness, setPowerState, setSunriseTime } from "../stores";

type Response = [string, string]

export default class SocketHandler {

	private delimiter = "/"

	constructor(public socket: WebSocket) {

		this.socket.addEventListener("message", this.handleEvent)

		this.socket.addEventListener("open", _ => {
			socket.send("full-inform/na")
			socket.send("config-inform/na")
		})

	}

	private handleEvent(event: MessageEvent) {
		let [path, data] = event.data.split("/") as [string, string]

		if (path == "inform-brightness") {
			let brightness = parseInt(data)
			setBrightness(brightness)
		}
		else if (path == "inform-power-state") {
			let powerState = data
			if (!["on", "off"].includes(powerState)) return
			setPowerState(data as "on" | "off")
		}
		else if (path == "inform-sunrise-time") {
			let [sunriseHour, sunriseMinute] = data.split(":")
			setSunriseTime([parseInt(sunriseHour), parseInt(sunriseMinute)])
		}
	}

	public sendMessage(path: string, data: string) {
		this.socket.send(`${path}${this.delimiter}${data}`)
	}
}