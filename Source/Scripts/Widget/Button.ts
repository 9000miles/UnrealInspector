import { Widget } from "./Widget";
import { EHAlign, EVAlign, TAttribute, WidgetProps } from "./IType";
import { MultiEvent } from "../Core/MultiEvent";
import * as UE from "ue";

export class Button extends Widget {
    text: TAttribute<string>
    isFocusable: boolean
    content: Widget;
    hAlign: EHAlign
    vAlign: EVAlign

    onClick: MultiEvent<() => void>;

    // content: Widget | () => Widget;

    constructor(props: WidgetProps<Button>) {
        super();
    }

    setText(text: string) {
        this.text = text;
    }

    setContent(content: Widget) {
        this.content = content;
    }

    /**
     * 鼠标移动
     * @param geometry
     * @param mouseEvent
     * @virtual
     */
    OnMouseMove(geometry: UE.Geometry, mouseEvent: UE.PointerEvent) {
        console.log("OnMouseMove")
    }
}