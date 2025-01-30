# more about too many open file error
* This note will focus on the too many open file error, why the Linux process hits this error, and how to avoid this?

## file descriptor data structure
![image](https://github.com/user-attachments/assets/c08e1b40-8192-4d96-9231-a68aac2f7d2e)

* key structures:
    * `struct task_struct`: the Linux task, each task has a file descriptor table
    * `struct files_struct`: manages the file descriptors with overall state and metadata
    * `struct fdtable`: low-level structure that manages the mapping of file descriptors to file objects.



