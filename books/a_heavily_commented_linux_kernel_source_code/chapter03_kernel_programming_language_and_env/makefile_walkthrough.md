# makefile walkthrough
* a makefile has five elements:
    * explicit rules
    * implicit rules
    * variable definitions
    * directives
    * comments

## rules
```
target ...: prerequisites ...
    command
    ...
    ...
```
* the prerequisites are a series of files or other targets necessary to create the target.
* when the last modification time of one or more files in the prerequisite condition is newer than that of the target file, the command of the rule will be executed.
* `phony target`: a target that has no prerequisites, just specifies operations
    * `clean` target
* by default, make will start from the first target in the Makefile (not including targets starting with '.'). This first goal is called the default goal of the Makefile. The ultimate goal is to make an effort to try to update the target.
