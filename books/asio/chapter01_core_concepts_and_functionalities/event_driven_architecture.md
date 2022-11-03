# event driven architecture

![image](https://user-images.githubusercontent.com/35479537/199648185-d10b9af1-b1a1-48d0-8735-f81f0cec6434.png)

## blocking vs nonblocking, synchronous vs asynchronous
* blocking and synchronous mean the same thing: you call the API, it hangs up the thread until it has some kind of answer and returns it to you.
* non-blocking means that if an answer can't be returned rapidly, the API returns immediately with an error and does nothing else. So there must be some related way to query whether the API is ready to be called (that is, to simulate a wait in an efficient way, to avoid manual polling in a tight loop).
* asynchronous means that the API always returns immediately, having started a "background" effort to fulfil your request, so there must be some related way to obtain the result.

## reactor pattern
* the reactor pattern allows event-driven applications to demutiplex and dispatch synchronously and serially service requests that are received simultaneously from one or more clients
* non-blocking synchronous I/O strategy
    * it is nonblocking if the event handler is executed not in the same threads as reactor.

### structure and participants
![image](https://user-images.githubusercontent.com/35479537/199648831-37682897-f627-47d9-b187-a7b7a448419a.png)

* participants:
    * `handle`
        * handles are provided by the operating system to identify event sources
    * `event handler`
        * defines an interface with a set of hook methods that represents the dispatching operations available to process events
    * `concrete event handler`
        * the event handler for a particular service, implements the hook methods
    * `reactor`
        * specifies an interface to register/remove event handlers and handles
        * run the event loop to react to each event by demultiplexing it from the handle to the event handler and dispatching the proper hook method
    * `synchronous event demultiplexer`
        * provided by the operating system to wait for one or more events to occur on a set of handles. 

![image](https://user-images.githubusercontent.com/35479537/199652433-5ba75774-2800-4581-b059-2d71c8c94019.png)


## proactor pattern
* the proactor pattern allows event-driven applications to demultiplex and dispatch service requests in an efficient asynchronous way.
* non-blocking asynchronous I/O strategy

### structure and participants
![image](https://user-images.githubusercontent.com/35479537/199652998-7a8a3e7b-7607-4f43-acf0-d34d70cd287e.png)

* participants:
    * `handle`
        * handles are provided by the operating system to identify event sources
    * `completion Handler`
        * defines an interface with a set of hook methods for the operations available to process results of asynchronous operations
    * `concrete completion handler`
        * specializes the completion handler for a particular service, and implements the hook methods
    * `proactor`
        * provides the application’s event loop, demultiplexes completion events to the related completion handers, and dispatches hook methods to process the results
    * `asynchronous event demultiplexer`
        * function that blocks awaiting completion events to be added to a completion queue, and returns them to the caller
    * `completion event queue`
        * buffers completion events while they are waiting to be demultiplexed to the respective completion handlers
    * `asynchronous operations`
        * represent potentially long-duration operations that are used to service on behalf of application
    * `asynchronous operation processor`
        * executes asynchronous operations invoked on handles, generates the respective completion event, and queues it
    * `initiator`
        * entity local to the application, initiates an asynchronous operation, registers a completion handler and a Proactor with an asynchronous operation processor, which notifies it when operations completes

![image](https://user-images.githubusercontent.com/35479537/199655634-ab31a05a-3c30-4ad3-b605-46b0c2c21e18.png)


## boost implementation with proactor pattern
![image](https://user-images.githubusercontent.com/35479537/199639966-82e2d6de-1a3b-4ced-b47d-9d9f81edb978.png)

* terms:
    * initiator
        * application code that starts asynchronous operations
    * asynchronous operation processor
        * executes asynchronous operations and queues completion events over completion event queue
    * completion event queue
        * when asynchronous operation processor finishes an asynchronous operation, one completion event will be pushed to this queue
    * asynchronous even demultiplexer
        * blocks waiting for events queued in completion event queue, and return a completion event to its caller
    * proactor
        * calls the asynchronous even demultiplexer to dequeue events and dispatch the completion handler associated with this event
        * this abstraction is represented by `io_service` class 


## references
* http://www.diranieh.com/DP/POSA_Reactor.htm
* http://didawiki.cli.di.unipi.it/lib/exe/fetch.php/magistraleinformatica/tdp/tpd_reactor_proactor.pdf
