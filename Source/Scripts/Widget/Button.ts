import { Widget } from "./Widget";
import { EHAlign, EVAlign, TAttribute, WidgetProps } from "./IType";
import { MultiEvent } from "../Core/MultiEvent";

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

}