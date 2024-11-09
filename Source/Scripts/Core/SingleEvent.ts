import { EventEntity } from "./EventEntity";

export class SingleEvent<T extends (...args: any[]) => any> extends EventEntity<T> {
    constructor(private readonly defaultReturnValue?: ReturnType<T>) {
        super()
    }

    execute(...args: Parameters<T>): ReturnType<T> {
        const listeners = this.eventEmitter.listeners(this.eventName())
        if (listeners.length > 0)
            return listeners[listeners.length - 1](args);
        return this.defaultReturnValue
    }

    isBound() {
        return this.listenerCount() > 0
    }
}