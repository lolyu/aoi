// class A doesn't break the ODR, it seems ODR doesn't apply to class names
class A
{
    int member;
};

// break the ODR
// int func()
// {
//     return 0;
// }
