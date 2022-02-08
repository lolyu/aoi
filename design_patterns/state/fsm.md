# finite state machine
* can be one of a finite number of states
* can change from one state to another in response to some inputs(`transition`)
* key components: `(Q, q0, ∑, O, δ, λ)`
  * `Q`: finite set of states
      * a state is a description of the status of a system that is waiting to execute a transition.
      * a transition is a set of actions to be executed when an event is received.
  * `∑`: inputs
  * `q0`: initial state
  * `O`: output alphabet
  * `δ(Q, ∑)`: transition function
  * `λ`: output function

## FSM types
### acceptor
* produces binary output, indicating whether or not the received input is accepted.
* the input is a sequence of symbols.
* there is no actions.

### transducers
* two types of transducers
  * moore machine
  * mealy machine


#### moore machine
```
A mealy machine is defined as a machine in theory of computation whose output values are determined by both its current state and current inputs. In this machine atmost one transition is possible. 
It has 6 tuples: (Q, q0, ∑, O, δ, λ) 
Q is finite set of states 
q0 is the initial state 
∑ is the input alphabet 
O is the output alphabet 
δ is transition function which maps Q×∑ → Q 
λ is the output function which maps Q×∑→ O 
```


#### mealy machine
```
A moore machine is defined as a machine in theory of computation whose output values are determined only by its current state. 
It has also 6 tuples: (Q, q0, ∑, O, δ, λ) 
Q is finite set of states 
q0 is the initial state 
∑ is the input alphabet 
O is the output alphabet 
δ is transition function which maps Q×∑ → Q 
λ is the output function which maps Q → O 
```



## references
* https://en.wikipedia.org/wiki/Moore_machine
* https://en.wikipedia.org/wiki/Mealy_machine
* https://en.wikipedia.org/wiki/Finite-state_machine
* https://inst.eecs.berkeley.edu/~cs61c/fa06/labs/10/PH-B10.pdf