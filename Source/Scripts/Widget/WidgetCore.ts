import { Widget } from "./Widget";
// @ts-ignore
import * as UE from "ue";
import { WidgetCollector } from "./WidgetCollector";


export class WidgetCore {


    static setWidgetToWindow(widget: Widget) {
        // let widgetProxy = UE.ScriptWidgetFunction.CreateWidgetProxy(widget);
        UE.ScriptWidgetFunction.SetWidgetToWindow(widget);
    }

    static findWidget(id: string) {
        return WidgetCollector.getWidget(id)
    }
}
