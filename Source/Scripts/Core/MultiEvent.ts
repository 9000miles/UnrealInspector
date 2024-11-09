import { EventEntity } from "./EventEntity";

export class MultiEvent<T extends (...args: any[]) => void> extends EventEntity<T> {
    broadcast(...args: Parameters<T>): boolean {
        return this.eventEmitter.emit(this.eventName(), ...args);
    }
}