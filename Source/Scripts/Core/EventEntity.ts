import { EventEmitter } from 'events';
import { utils } from '../utils/utils';

type FunctionType = (...args: any[]) => void;

export abstract class EventEntity<T extends FunctionType> {
    private EventName = Symbol('Event_Name');
    protected readonly eventEmitter = new EventEmitter();
    protected readonly listenerHandles: { [key: string]: T } = {};

    protected eventName(): string {
        return this.EventName.toString();
    }

    addListener(listener: T): string {
        return this.on(listener);
    }

    on(listener: T): string {
        this.eventEmitter.addListener(this.eventName(), listener as any);

        const handle = utils.new_guid();
        this.listenerHandles[handle] = listener;
        return handle;
    }

    once(listener: T): void {
        this.eventEmitter.once(this.eventName(), listener as any);
    }

    removeListener(handle: string): void
    removeListener(listener: T): void
    removeListener(listener: T | string): void {
        this.off(listener as any);
    }

    off(handle: string): void
    off(listener: T): void
    off(listener: T | string): void {
        if (typeof listener === 'string') {
            listener = this.listenerHandles[listener];
            delete this.listenerHandles[listener as unknown as string];
        }

        this.eventEmitter.off(this.eventName(), listener as any);
    }

    removeAllListeners(): void {
        for (const handle in this.listenerHandles) {
            this.off(handle);
        }
        this.eventEmitter.removeAllListeners(this.eventName());
    }

    setMaxListeners(n: number): void {
        this.eventEmitter.setMaxListeners(n);
    }

    getMaxListeners(): number {
        return this.eventEmitter.getMaxListeners();
    }

    listeners(): T[] {
        return this.eventEmitter.listeners(this.eventName()) as T[];
    }

    rawListeners(): T[] {
        return this.eventEmitter.rawListeners(this.eventName()) as T[];
    }

    listenerCount(): number {
        return this.eventEmitter.listenerCount(this.eventName());
    }

    prependListener(listener: T): string {
        this.eventEmitter.prependListener(this.eventName(), listener as any);

        const handle = utils.new_guid();
        this.listenerHandles[handle] = listener;
        return handle;
    }

    prependOnceListener(listener: T): void {
        this.eventEmitter.prependOnceListener(this.eventName(), listener as any);
    }

    eventNames(): string[] {
        return this.eventEmitter.eventNames() as string[];
    }
}