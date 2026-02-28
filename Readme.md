# CResult

## Summary

Error handling in C is quite a pain, other languages like `Rust` and `C++` have better error
handling though the use of `exceptions` of better `return type`.

The last one is particularly interesting, in fact both `Rust` and `C++` has their own better
return value error handling. Probably the best is `Rust` with the Result type, but also `C++`
with `std::expected` does a decent job.

Since I quite enjoy this type of error handling I decided to implement this little library.
It's a template base library (like template in C++ but though C macros) and it gives the users
the tools to create and manage `Result` types like in rust with also a pattern matching system.

The library works on every standard starting from C89 with some caveats:
In C89 some of the features are not available since they depend on features which are not
available in C89. The library still compiles and through `conditional guards` these features are 
removed from the compilation.

> [!WARNING]
> This library uses some `GCC Extensions` so check your compiler for that.

```c
#include <stdio.h>

#define CRESULT_NO_PREFIX
#include "Cresult.h"


typedef TEMPLATE(int, char*) CResult_int_char;
typedef TEMPLATE(int, struct H{int a;}) CResult_int_custom_c;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
static RETURN(CResult_int_char) foo(int i){
  if (i==2) {
    return NEW_OK(CResult_int_char,1);
  }else{
    return NEW_ERR(CResult_int_char, "invalid value");
  }
}
#else
static RETURN(CResult_int_char) foo(int i){
  CResult_int_char res = {0};

  if (i==2) {
    SET_OK(res, 1);
  }else{
    SET_ERR(res, "invalid value");
  }

  return res;
}
#endif

int main(void)
{
  CResult_int_char res;

  printf("foo(2)\n");
  res= foo(2);

  printf("manual\n");
  if (IS_OK(res)){
    printf("res: %d\n", OK_VAL(res));
  }else{
    printf("res: %s\n", ERR_VAL(res));
  }

  #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
  printf("with full match\n");
  FULL_MATCH(res, res_val, printf("res: %d\n", res_val), printf("res: %s\n", res_val));

  printf("with ok match\n");
  OK_MATCH(res, ok_val, printf("res: %d\n", ok_val));

  printf("with err match\n");
  ERR_MATCH(res, err_val, printf("res: %s\n", err_val));
  #endif


  printf("--------------------------\n");
  printf("foo(5)\n");
  res = foo(5);

  printf("manual\n");
  if (IS_OK(res)){
    printf("res: %d\n", OK_VAL(res));
  }else{
    printf("res: %s\n", ERR_VAL(res));
  }

  #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
  printf("with full match\n");
  FULL_MATCH(res, res_val, printf("res: %d\n", res_val), printf("res: %s\n", res_val));

  printf("with ok match\n");
  OK_MATCH(res, ok_val, printf("res: %d\n", ok_val));

  printf("with err match\n");
  ERR_MATCH(res, err_val, printf("res: %s\n", err_val));
  #endif

  return 0;
}
```

## Usage

### Defining a Result

> To make the code less verbose a namespace system has been defined.
> By defining `CRESULT_NO_PREFIX`before the include you get rid of that. 
> Works in the same way as `using namespace X` in `C++`.

To use a Result first you need to define one. To do that you specialize the template
(`#define CRESULT_TEMPLATE(T_OK, T_ERR)`) in the following way;

```c
typedef TEMPLATE(int, char*) CResult_int_char;
typedef TEMPLATE(int, struct H{int a;}) CResult_int_custom_c;
```

- T_OK : It's the type for the result in case of success
- T_ERR : It's the type for the result in case of failure

There is no real constraint to the type: they can be anything from pointers, to structs,
to primitive types.

After a Result has been designed it can be used freely in the code.

### Returning a Result

To use it on a function you just need to make the function return a Result type in this way:
```c
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
RETURN(CResult_int_char) foo(int i){
  if (i==2) {
    return NEW_OK(CResult_int_char,1);
  }else{
    return NEW_ERR(CResult_int_char, "invalid value");
  }
}
#else
CResult_int_char foo(int i){
  CResult_int_char res = {0};

  if (i==2) {
    SET_OK(res, 1);
  }else{
    SET_ERR(res, "invalid value");
  }

  return res;
}
#endif
```

In this snippet of code the function foo() returns a `CResult_int_char` with `Ok(1)` if i==2 and
`Err("invalid value")` otherwise. The `RETURN` macro forces the evaluation of the Result value if
setting the attribute `warn_unused_result` on the return type of the function. (Like in Rust)

> [!IMPORTANT]
> `NEW_OK` and `NEW_ERR` requires at least C99. In case this is not possible you can do that
> manually by using `SET_OK` and `SET_ERR` as in the example above.

### Consuming a Result

To inspect and use a Result type there are different approaches. They can be splitted in two
categories:

- Manual
- Automatic

#### Manual

In the manual approach the user is responsible to determine if the Result value is an
`Ok()` or an `Err()` by using the command `IS_OK(self)`.
After that the user can access the data by using the following commands:

- for Ok values: `OK_VAL(self)`
- for Err values: `ERR_VAL(self)`

> [!IMPORTANT]
> This approach works on any C standard starting from **C89**, assuming `__GNUC__`.

#### Automatic

In the automatic approach the user can use a set of command to pattern match the proper value
and execute different expressions based on the context. Example below:

```c
  #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
  printf("with full match\n");
  FULL_MATCH(res, res_val, printf("res: %d\n", res_val);, printf("res: %s\n", res_val););

  printf("with ok match\n");
  OK_MATCH(res, ok_val, printf("res: %d\n", ok_val););

  printf("with err match\n");
  ERR_MATCH(res, err_val, printf("res: %s\n", err_val););
  #endif
```

> [!IMPORTANT]
> This approach works on any C standard starting from **C99**, assuming `__GNUC__`.
