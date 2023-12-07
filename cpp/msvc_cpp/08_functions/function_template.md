# function template
* function template provides a generic way to define functions that could be called with multiple types.
    * a template is a pattern that the compiler uses to generate a family of classes or functions
* a function template by itself is not a function definition.
    * a template must be instantiated, the template arguments must be determined so that the compiler could generate an actual function definition
    * the implicit instantiation happens at the compile-time, when the compiler sees the template function call and instantiate the template with the arguments' types.
        * **the compiler needs to see both the template definition and the specific types used to fill the template**
        * so the template definition is always put inside the header file
        * when a `.cpp` file that uses the template wants to instantiate it, it could have both the template definition(in the header) and the specific type(provided by itself)
## explicit specialization
* explicit specialization provides a specialized function definition other than the function template for a special type
* if there are multiple function definitions: non-template functions, function template and function template specializations:
    * non-template functions > function template specialization > function template

## explicit instantiation
* explicit instantiation is used to explicitly instantiate a function template.
* why we need explicit instantiation?
    * explicit instantiation allows putting the template declaration in the header and template definition in the source file.
    * in this way, any other code that wants to use this template is limited to use the already-instanted template.
        * not allowed to instantiate new template, because the template definition is not visible to other source file in the compile-time.

## references
* https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
