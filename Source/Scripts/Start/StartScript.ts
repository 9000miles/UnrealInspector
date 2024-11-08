import { Button } from "../Widget/Button";
import { WidgetCore } from "../Widget/WidgetCore";

console.log("hello world")

const button = new Button()
button.setText("hello world")
button.onClick = () => {
    console.log("clicked")
}

WidgetCore.setWidgetToWindow(button)
