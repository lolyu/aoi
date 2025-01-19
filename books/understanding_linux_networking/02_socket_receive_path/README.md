# socket receive path

## questions
### how many buffering in the socket receive path?
* at least two:
    * the device ring buffer
    * the socket receive queue
