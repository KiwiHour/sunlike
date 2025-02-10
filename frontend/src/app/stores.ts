import { create } from "zustand";

interface BrightnessState {
	brightness: number;
	setBrightness: (value: number) => void;
}

type PowerState = "on" | "off"
interface PowerStateState {
	powerState: PowerState;
	setPowerState: (powerState: PowerState) => void;
}

export const brightnessStore = create<BrightnessState>(set => ({
	brightness: -1,
	setBrightness: (value: number) => set({ brightness: value })
}))
export const getBrightness = () => brightnessStore.getState().brightness
export const setBrightness = (value: number) => brightnessStore.getState().setBrightness(value)

export const powerStateStore = create<PowerStateState>(set => ({
	powerState: "on",
	setPowerState: (value: PowerState) => set({ powerState: value })
}))
export const getPowerState = () => powerStateStore.getState().powerState
export const setPowerState = (value: PowerState) => powerStateStore.getState().setPowerState(value)