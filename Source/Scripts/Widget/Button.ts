import { Widget } from "./Widget";

export class Button extends Widget {
    text: string;
    onClick: () => void;

    constructor() {
        super();
    }

    setText(text: string) {
        this.text = text;
    }

}