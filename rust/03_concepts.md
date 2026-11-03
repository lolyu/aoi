# concepts

## variables and mutability

* **variables are immutable by default**


### const

```rust
const THREE_HOURS_IN_SECONDS: u32 = 60 * 60 * 3;
```

* `const`:
    * `const` can be declared in any scope
    * `const` can only be assigned with a constant expression

### shadowing

```rust
let mut x = 10;
{
    let x = 12;
    println!("The value of x is: {x}");
}
```

## data types

* **rust is static-type language**

### scalar types

* four scalar types:
    * int
      * i8/u8
      * i16/u16
      * i32(default)/u32
      * i64/u64
      * i128/u128
      * isize/usize
    * float
      * f32
      * f64
    * bool
    * char

* bool

```rust
let f: bool = false;
let c = 'z';
let z: char = 'Z';
```


### compound types

* two compound types:
    * tuple
    * array

#### tuple

```rust
fn main() {
    let tup: (i32, f64, char) = (500, 6.4, 'z');
    let (x, y, z) = tup;
    println!("The value of x is: {x}");
    println!("The value of y is: {y}");
    println!("The value of z is: {z}");
    println!("The first item of tuple is: {}", tup.0);
    println!("The second item of tuple is: {}", tup.1);
    println!("The third item of tuple is: {}", tup.2);

    let mut tup = (500, 6.4, 'z');
    tup.0 = 1000;
    println!("The first item of tuple is: {}", tup.0);
}
```

#### array
* **rust ALWAYS check if the index is less than the array length**

```rust
use std::io;

fn main() {
    let a = [3; 5];
    let a: [u32; 5] = [1, 2, 3, 4, 5];

    println!("please enter an array index.");

    let mut index = String::new();
    io::stdin().read_line(&mut index).expect("Failed to read line");
    let index: usize = index.trim().parse().expect("Index entered was not a number");

    let element = a[index];
    println!("The value of the element at index {index} is: {element}");
}
```

## functions

* in function signature, **MUST** declare the type of each parameter
* function bodies are made up of:
    * **statements** are instructions to perform actions with no return value
        * `let ...`
        * any expression followed by `;`
    * **expressions** evaluate to a result value
        * a value `5`, `x + 1`
        * a function/macro call
        * a block
        * `if`
        * `match`

* assignment is statement:
```rust
let y = 10; 
```

* `y` is 4
```rust
let y = {
    let x = 3;
    x + 1
};
```

* `y` is `()` - empty tuple/unit type
```rust
let y = {
    let x = 3;
    x + 1;
};
```

### function return

* the rust function is an item - declaration
* the rust function body is ablock - expression

```rust
fn five() -> i32 {
    5;
}

fn main() {
    let x = five();
    println!("{}", x);
}
```

## comments

* two formats:
    * `// comment`
    * `/* comment */`

## control flows

### if expression

```rust
fn main() {
    let number = 3;
    if number < 5 {
        println!("number is smaller than 5");
    } else if number == 5 {
        println!("number is equal to 5");
    } else {
        println!("number is larger than 5");
    }
}
```

#### use if in a let statement

* the values in the `if` or `else` must be the same type

```rust
let condition = true;
let number = if condition { 5 } else {6};
let number = if condition { 5 } else { "six" };
```


### loop
* three ways to loop:
    * `loop`
    * `while`
    * `for`


#### loop
```rust
fn main() {
    let mut counter = 0;
    let result = loop {
        counter += 1;
        if counter == 10 {
            break counter * 2;
        }
    };
    println!("The result is {result}");
}
```

* loop can be labeled so `break` or `continue` can apply to the target loop:

```rust
fn main() {
    let mut count = 0;
    'outer_loop: loop {
        println!("count = {count}");
        let mut remaining = 10;
        'inner_loop: loop {
            println!("remaining = {remaining}");
            if remaining == 9 {
                break 'inner_loop;
            }
            if count == 2 {
                break 'outer_loop;
            }
            remaining -= 1;
        }
        count += 1;
    }
}
```

#### while

```rust
fn main() {
    let a = [10, 20, 30, 40, 50];
    let mut index = 0;

    while index < 5 {
        println!("the value is: {}", a[index]);

        index += 1;
    }
}
```

#### for

```rust
fn main() {
    let a = [10, 20, 30, 40, 50];

    for element in a {
        println!("the value is: {element}");
    }
}
```
