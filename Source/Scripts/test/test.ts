import { MultiEvent } from "../Core/MultiEvent";

type NonFunctionPropertyNames<T> = { [K in keyof T]: T[K] extends Function ? never : K; }[keyof T];
type NonFunctionProperties<T> = Pick<T, NonFunctionPropertyNames<T>>;
type WidgetProps<T> = Partial<NonFunctionProperties<T>> & Required<Pick<NonFunctionProperties<base>, "filename">>;

class base {
    filename: string

    constructor() {
        this.filename
    }

    setFilename(filename: string) {
        this.filename = filename
    }
}

class test extends base {
    a: string
    b: number
    c?: boolean
    d: string[]
    onclicked: MultiEvent<() => void> = new MultiEvent<() => void>()

    constructor(init: WidgetProps<test>) {
        super()
        Object.assign(this, init)
    }

    setA(a: string) {
        this.a = a
    }
}

const t = new test({
    a: 'a',
    b: 1,
    // d: ['a', 'b'],
    c: true,
    d: [],
    filename: __filename,
})
t.onclicked.on(() => console.log('clicked'))


console.log(JSON.stringify(t))