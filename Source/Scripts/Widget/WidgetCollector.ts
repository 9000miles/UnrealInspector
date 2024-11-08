import { Widget } from "./Widget";

export class WidgetCollector {
    static widgets_map: Map<string, Widget> = new Map();

    constructor() {
    }

    static addWidget(widget: Widget) {
        this.widgets_map.set(widget.id, widget);
    }

    static getWidget(id: string) {
        return this.widgets_map.get(id);
    }
}