import { Widget } from "./Widget";

export type TAttribute<T> = T | (() => T);

export type NonFunctionPropertyNames<T> = { [K in keyof T]: T[K] extends Function ? never : K; }[keyof T];
export type NonFunctionProperties<T> = Pick<T, NonFunctionPropertyNames<T>>;
export type WidgetProps<T> = Partial<NonFunctionProperties<T>> & Required<Pick<NonFunctionProperties<Widget>, "filename">>;

export enum EHAlign {
    Left,
    Center,
    Right
}

export enum EVAlign {
    Top,
    Center,
    Bottom
}

export class Sound {

}

