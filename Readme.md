# CResult

## Summary

Error handling in C is quite painful. Other languages like `Rust` and `C++` have better error
handling through the use of `exceptions` or improved `return types`.

The latter is particularly interesting. In fact, both `Rust` and `C++` provide their own
return value-based error handling mechanisms.
`Rust`'s `Result` type is probably the most elegant, but also `C++` with `std::expected` does a
decent job.

Since I quite enjoy this type of error handling, I decided to implement this library.
It's a template-based library (like templates in `C++`, but implemented through `C` macros) and it
gives users the tools to create and manage `Result` types similar to those in `Rust`, including
pattern matching.

The library works with every standard starting from C89 with some caveats:
In C89, some features are unavailable because they depend on language features introduced in
later standards. The library still compiles, and through `conditional guards` these features are 
excluded from compilation.

> [!WARNING]
> This library uses some `GCC Extensions` so check your compiler for that.

```c
#include <stdio.h>

#define CRESULT_NO_PREFIX
#include "Cresult.h"

typedef TEMPLATE(int, char*) CResult_int_char;
typedef TEMPLATE(int, struct H{int a;}) CResult_int_custom_c;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
static RETURN_TYPE(CResult_int_char) foo(int i){
  #define OK(val) T_OK(CResult_int_char, val)

  if (i==2) {
    return OK(1);
  }else{
    return T_ERR(CResult_int_char,"invalid value");
  }
}
#else
static RETURN_TYPE(CResult_int_char) foo(int i){
  CResult_int_char res = {0};

  if (i==2) {
    SET_OK(res, 1);
  }else{
    SET_ERR(res, "invalid value");
  }

  return res;
}
#endif

RETURN_TYPE(CResult_int_char) try_f(int j){
  CResult_int_char res = {0};

  if (j==1) SET_OK(res, 2);
  else SET_ERR(res, "try failed");

  return res;
}

RETURN_TYPE(CResult_int_char) bar(int i){
  CResult_int_char res = {0};
  int z = TRY(try_f(i));
  (void)z;

  SET_OK(res,1);

  return res;
}

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

  printf("with full match\n");
  FULL_MATCH(res, res_val, printf("res: %d\n", res_val), printf("res: %s\n", res_val));

  printf("with ok match\n");
  OK_MATCH(res, ok_val, printf("res: %d\n", ok_val));

  printf("with err match\n");
  ERR_MATCH(res, err_val, printf("res: %s\n", err_val));

  printf("call to bar(5)\n");
  FULL_MATCH(bar(5), val, printf("ok val: %d\n", val), printf("error: %s\n",val));
  printf("call to bar(1)\n");
  FULL_MATCH(bar(1), val, printf("ok val: %d\n", val), printf("error: %s\n",val));

  return 0;
}

```

## Usage

### Defining a Result

> To make the code less verbose, a namespace system has been defined.
> By defining `CRESULT_NO_PREFIX` before the include, you remove the prefix `CRESULT_`. 
> It works in the same way as `using namespace X` in `C++`.

To use a Result, first you need to define one. To do that, you specialize the template
(`#define CRESULT_TEMPLATE(T_OK, T_ERR)`) in the following way:

```c
typedef TEMPLATE(int, char*) CResult_int_char;
typedef TEMPLATE(int, struct H{int a;}) CResult_int_custom_c;
```

- T_OK : It's the type for the result in case of success
- T_ERR : It's the type for the result in case of failure

There is no real constraint on the type: they can be anything from pointers, to structs,
to primitive types.

After a Result has been designed, it can be used freely in the code.

### Returning a Result

To use it in a function you just make the function return a Result type in this way:
```c
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
RETURN(CResult_int_char) foo(int i){
  if (i == 2) {
    return NEW_OK(CResult_int_char,1);
  }else{
    return NEW_ERR(CResult_int_char, "invalid value");
  }
}
#else
CResult_int_char foo(int i){
  CResult_int_char res = {0};

  if (i == 2) {
    SET_OK(res, 1);
  }else{
    SET_ERR(res, "invalid value");
  }

  return res;
}
#endif
```

In this snippet of code, the function foo() returns a `CResult_int_char` with `Ok(1)` if i == 2 and
`Err("invalid value")` otherwise. 
The `RETURN` macro enforces that the Result value cannot be ignored by applying the
`warn_unused_result` attribute to the function's return type. (Like in Rust)

> [!IMPORTANT]
> `NEW_OK` and `NEW_ERR` require at least C99. In case this is not possible, you can do that
> manually by using `SET_OK` and `SET_ERR` as in the example above.

### Consuming a Result

To inspect and use a Result type there are different approaches. They can be split into two
categories:

- Manual
- Automatic

#### Manual

In the manual approach, the user is responsible for determining whether the Result value is an
`Ok()` or an `Err()` by using the command `IS_OK(self)`.
After that the user can access the data by using the following commands:

- for Ok values: `OK_VAL(self)`
- for Err values: `ERR_VAL(self)`

#### Automatic

In the automatic approach, the user can use a set of commands to pattern match the appropriate
value and execute different expressions based on the context. Example below:

```c
  printf("with full match\n");
  FULL_MATCH(res, res_val, printf("res: %d\n", res_val), printf("res: %s\n", res_val));

  printf("with ok match\n");
  OK_MATCH(res, ok_val, printf("res: %d\n", ok_val));

  printf("with err match\n");
  ERR_MATCH(res, err_val, printf("res: %s\n", err_val));

  RETURN_TYPE(CResult_int_char) bar(int i){
      CResult_int_char res = {0};
      int z = TRY(try_f(i));
  }
```

The `TRY` command allow the evaluation of a function and if it returned a `Err(x)` than that error
is returned by the function itself else it is stored in a lvalue like. 
This is similar to the `?` of Rust.
