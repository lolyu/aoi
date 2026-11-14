# ownership

* goal of rust:
    * fundation of rust is to ensure the program will always have defined behavior
    * prevent undefined behavior at compile-time instead of runtime

## ownership basics

### ownership as a discipline for memory safety
* rust goals:
  * primary: ensure program has no UB (undefined behavior)
  * secondary: ensure rust can prevent UB at compile time

### rust model of memory

* rules:
  1. variables live in the stack
  2. boxes live in the heap
  3. box deallocation principle: If a variable owns a box, when Rust deallocates the variable’s frame, then Rust deallocates the box’s heap memory.
  4. moved heap data principle: if a variable x moves ownership of heap data to another variable y, then x cannot be used after the move.
  5. clone avoids moves


```rust
use std::string::String;

fn add_world(s: String) -> String {
    s + " world"
}

fn main() {
    let s0 = String::from("hello");
    let s1 = add_world(s0);
    println!("{}", s0);             // UB, the ownership of the string is moved from s0 to s1
}
```

#### ownership
* Ownership is primarily a discipline of heap management:
  * All heap data must be owned by exactly one variable.
  * Rust deallocates heap data once its owner goes out of scope.
  * Ownership can be transferred by moves, which happen on assignments and function calls.
  * Heap data can only be accessed through its current owner, not a previous owner.

## references and borrowing

* problem: variables can no longer be used as the ownership moves
* rust has two types of references:
  * mutable references
  * immutable references


```rust
use std::string::String;

fn greet0(s0: String, s1: String) {
    println!("Hello, {} and {}!", s0, s1);
}

fn greet1(s0: &String, s1: &String) {
    println!("Hi, {} and {}!", s0, s1);
}

fn main() {
    let s0 = String::from("Alice");
    let s1 = String::from("Bob");
    greet0(s0, s1);
    // s0 and s1 are no longer valid here because
    // they have been moved into the greet function

    let s0 = String::from("Alice");
    let s1 = String::from("Bob");
    greet1(&s0, &s1);
    // s0 and s1 are still valid here because
    // they were only borrowed by greet1
    println!("s0 is still: {}", s0);
    println!("s1 is still: {}", s1);
}
```

* **rust references are non-owning pointers**

* deferencing a pointer acesses its data
```rust
fn main() {
    let mut x: Box<i32> = Box::new(0);      // x is a reference
    let a: i32 = *x;                        // a is assigned with dereferencing x
    *x += 1;
    println!("{a}");                        // 0
    println!("{}", *x);                     // 1

    let r1: &Box<i32> = &x;                 // r1 points to x
    let b: i32 = **r1;
    println!("{b}");                        // 1

    let r2: &i32 = &*x;                     // r2 points to the heap value
    let c: i32 = *r2;
    println!("{c}");                        // 1
}
```

### mut

```rust
let mut v = vec![1, 2, 3];
let <mut> r = &<mut> v;
```

* the diff between `mut` before `r` and `mut` before `v`:

||what it's about|permission|what it allows|requirement|
|-|-|-|-|-|
|`mut` before `r`|the reference `r`| `W` on `r` | putting a different address in the box: `r = &v0;` | none |
|`&mut` before `v`|the vector at the address| `W` on `*r` | changing the vector through `r`: `r.push(5)` | `v` must be mutable `let mut`, and `v` cannot be used while `r` is in use |


### borrow checker

```rust
fn main() {
    let mut v: Vec<i32> = vec![1, 2, 3];
    let num: &i32 = &v[2];
    v.push(4);
    println!("Third element is {}", *num);
}
```

* build error:
```
$ cargo run
   Compiling references v0.1.0 (/home/lolv/workspace/repo/vpp/rust/projects/references)
error[E0502]: cannot borrow `v` as mutable because it is also borrowed as immutable
 --> src/main.rs:4:5
  |
3 |     let num: &i32 = &v[2];
  |                      - immutable borrow occurs here
4 |     v.push(4);
  |     ^^^^^^^^^ mutable borrow occurs here
5 |     println!("Third element is {}", *num);
  |                                     ---- immutable borrow later used here

For more information about this error, try `rustc --explain E0502`.
error: could not compile `references` (bin "references") due to 1 previous error
```

### borrow concept

* a **borrow** uses a value through a reference instead of taking ownership of it
  * `&v`: shared (immutable) borrow, read-only; many can exist at the same time
  * `&mut v`: mutable borrow, read-write; nothing else can access `v` meanwhile
* the owner stays the owner the whole time
  * the reference is the pointer; the borrow is the loan behind it
  * the borrow lasts from where the reference is created to its **last use**, not to the end of the scope
* this fixes the move problem above: `greet0(s0, s1)` moves the strings, `greet1(&s0, &s1)` only borrows them

* permissions during a borrow (R/W/O: see rust permission below)
  * `&v`: `v` keeps R, loses W and O; the data through the reference (`*r`) gets R
  * `&mut v`: `v` loses R, W and O; `*r` gets R and W
  * `*r` never gets O
  * after the reference's last use, `v` gets its permissions back

```rust
fn main() {
    let mut v = vec![1, 2, 3];
    let n = v.len();                    // borrows &v, ends right away
    v.push(4);                          // borrows &mut v, ends right away
    for x in &v {                       // borrows &v for the loop
        print!("{x} ");
    }
    println!("{n} {:?}", v);            // 1 2 3 4 3 [1, 2, 3, 4]
    let w = v;                          // ok: nothing is borrowed any more
    println!("{:?}", w);                // [1, 2, 3, 4]
}
```

* reborrow: passing a `&mut` to a function doesn't move it; the compiler borrows the data again for the call (`&mut *r`)
  * so `r` can be passed again, while `let r2 = r;` would move `r`

```rust
fn add(v: &mut Vec<i32>, x: i32) {
    v.push(x);
}

fn main() {
    let mut v = vec![1];
    let r = &mut v;
    add(r, 2);                          // reborrows: &mut *r
    add(r, 3);                          // r is still usable
    println!("{:?}", v);                // [1, 2, 3]
}
```


```rust
fn total(v: &Vec<i32>) -> i32 {
    v.iter().sum()                      // a shared borrow can only read
}

fn add_one(v: &mut Vec<i32>) {
    for x in v.iter_mut() {
        *x += 1;                        // a mutable borrow can also write
    }
}

fn main() {
    // shared borrow: many readers, nobody writes
    let mut v = vec![1, 2, 3];
    let r1 = &v;                        // v keeps R, loses W O; *r1: R
    let r2 = &v;                        // a second shared borrow is fine
    println!("{:?} {:?} {:?}", r1, r2, v); // [1, 2, 3] [1, 2, 3] [1, 2, 3]
    println!("{}", total(&v));          // 6
    // r1.push(4);                      // E0596: can't write through a &
    // v.push(4);                       // E0502: v has no W while r1 is in use
    println!("{}", r1.len());           // 3, the last use of r1
    v.push(4);                          // v has R W O again
    println!("{:?}", v);                // [1, 2, 3, 4]

    // mutable borrow: one writer, nobody else even reads
    let r = &mut v;                     // v loses R W O; *r: R W
    r.push(5);
    add_one(r);                         // reborrow: r is still usable after
    // println!("{:?}", v);             // E0502: v can't be read while r is in use
    // let r3 = &mut v;                 // E0499: only one &mut at a time
    println!("{:?}", r);                // [2, 3, 4, 5, 6], the last use of r
    println!("{:?}", v);                // [2, 3, 4, 5, 6], v has R W O again
}
```

### rust permission

* rust has three kinds of data permissions
  * read: data can be copied to another location
  * write: data can be mutated
  * own: data can be moved/dropped

* a variable has `RO` permission of its data
* a `mut` variable has `RWO` permission of its data



