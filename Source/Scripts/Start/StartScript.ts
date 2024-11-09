import { Button } from "../Widget/Button";
import { WidgetCore } from "../Widget/WidgetCore";
import { EHAlign, EVAlign } from "../Widget/IType";

console.log("hello world")

const button = new Button({
    text: "hello world",
    hAlign: EHAlign.Center,
    vAlign: EVAlign.Center,
    filename: __filename
})
button.onClick.on(() => {
    console.log("clicked")
})
button.onClick.on(ssssf)

function ssssf() {
    console.log("ssssf")
}

WidgetCore.setWidgetToWindow(button)
