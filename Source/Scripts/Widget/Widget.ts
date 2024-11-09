import { Guid } from "guid-typescript";
import { WidgetCollector } from "./WidgetCollector";

export class Widget {
    private __id__: string
    filename: string

    constructor() {
        this.__id__ = Guid.raw()
        WidgetCollector.addWidget(this)
    }

    get id(): string {
        return this.__id__
    }
}