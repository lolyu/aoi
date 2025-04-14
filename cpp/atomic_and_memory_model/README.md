# CPP atomic

## questions

### why memory barrier is needed?
* cache coherence protocol:
    * snooping protocols: MESI protocol
    * directory-based protocols
* for MESI protocol, when the CPU performs the store operation, the CPU must wait until it receives invalidate acknowledge from other CPUs after sending the invalidate message.

![image](https://github.com/user-attachments/assets/6e698d5d-6437-4bb1-98b1-dd486a036603)

* the stall is not neccessary -> store buffer and invalidate queue are introduced:
    * store operation can be stored in the store buffer while waiting for the invalidate message.
    * invalidate acknowledge will be instantly replied before the CPU actually processes the invalidate.
  


## reference
* https://en.wikipedia.org/wiki/MESI_protocol
* https://www-old.cs.utah.edu/~aburtsev/cs5460/lectures/lecture13-memory-ordering/lecture13-memory-barriers.pdf
* https://www.alibabacloud.com/blog/memory-model-and-synchronization-primitive---part-1-memory-barrier_597460
* https://www.alibabacloud.com/blog/memory-model-and-synchronization-primitive---part-2-memory-model_597461
* https://herbsutter.com/2013/02/11/atomic-weapons-the-c-memory-model-and-modern-hardware/
* https://en.wikipedia.org/wiki/Memory_coherence
* https://www.kernel.org/doc/Documentation/memory-barriers.txt
* https://preshing.com/20120913/acquire-and-release-semantics/
* https://developer.arm.com/documentation/102336/0100/Load-Acquire-and-Store-Release-instructions
* https://www.cs.rochester.edu/u/sandhya/csc258/seminars/2015/java_c++_memory_models.pdf
