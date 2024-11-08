import { Guid } from "guid-typescript";
import { WidgetCollector } from "./WidgetCollector";

export class Widget {
    private __id__: string
    private __type__: string

    constructor() {
        this.__id__ = Guid.raw()
        this.__type__ = this.constructor.name
        console.log("Widget created: " + this.__id__, this.__type__)
        WidgetCollector.addWidget(this)
    }

    get id(): string {
        return this.__id__
    }

    get type(): string {
        return this.__type__
    }
}