let instance_count = 0;

class Test {
    constructor() {
        this.name = "Test Instance" + instance_count++;
    }
}

let a = [];

for (let i = 0; i < 1000; i++) {
    a.push(new Test());
}

console.log(a);