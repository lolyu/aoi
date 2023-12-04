# function template
* function template provides a generic way to define functions that could be called with multiple types.
    * a template is a pattern that the compiler uses to generate a family of classes or functions
* a function template by itself is not a function definition.
    * a template must be instantiated, the template arguments must be determined so that the compiler could generate an actual function definition

## explicit specialization
* explicit specialization provides a specialized function definition other than the function template for a special type
* if there are multiple function definitions: non-template functions, function template and function template specializations:
    * non-template functions > function template specialization > function template

## explicit instantiation
* explicit instantiation is used to explicitly instantiate a function template.
* why we need explicit instantiation?
    * 

## references
* https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
