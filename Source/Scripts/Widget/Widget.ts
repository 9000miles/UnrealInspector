import { Guid } from "guid-typescript";
import { WidgetCollector } from "./WidgetCollector";


/**
 * 在C++端，检测Widget对象的new，同时SNew对应的SWidget，并将SWidget对象保存在Widget对象实例中
 * C++端使用的时候，获取对应的Widget对象，再通过Widget对象获取对应的SWidget对象
 * 不然得话就需要通过id 来获取对应的SWidget对象，非常麻烦
 */

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