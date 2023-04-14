# netlink

* summary:
    * `NetLink` represents a selectable
    * `NetMsg` provides APIs as callback for regsitered netlink messages
    * `NetDispatcher`: a singleton class to call the callback registered to the received netlink message

* class `NetLink`: selectable netlink socket, provide same interface to be compatible with swss select architecture
    * `registerGroup`
        * register with a specific protocol group
    * `dumpRequest`
        * dump existing kernel table
    * `readData`
        * called when the netlink socket is readable, call `nl_recvmsgs_default` to read from netlink socket
    * `onNetlinkMsg`
        * callback registered with the netlink socket, will be called if message is received via `nl_recvmsgs_default`
        * call `NetDispatcher::getInstance().onNetlinkMessage`
 * class `NetMsg`: represent a message handler for a specific message type
    * `onMsg`
        * virtual function to be override by child class
 * class `NetDispatcher`: a singleton class to dispatch the callback based on the message type
    * `std::map<int, NetMsg*> m_handlers`
        * mapping from message type to message handler
    * `registerMessageHandler`
    * `unregisterMessageHandler`
    * `getCallback`
    * `onNetLinkMessage`
        * dispatch the message received
        * call `nlCallback`
    * `nlCallback`
        * call the corresponding `NetMsg` obj's `onMsg`
