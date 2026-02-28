#pragma once

#if !defined(__GNUC__) && !defined(__clang__)
#error unsupported compiler
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define CRESULT_C99
#endif

#if defined(CRESULT_C99)
#include <stdbool.h>
#else
#define _Bool unsigned char
#define true 1
#define false 0
#endif

#define CRESULT_APPEND_RAW(x, y) x ## y
#define CRESULT_APPEND_2(x, y) CRESULT_APPEND_RAW(x,y)

/*
 * Template definition of a CResult type.
 * @param T_OK: type of the data in case of success
 * @param T_ERR: type of the data in case of failure
 *
 * example usage:
 * typedef TEMPLATE(int, char*) CResult_int_char;
 * typedef TEMPLATE(int, struct H{int a;}) CResult_int_custom_c;
 *
 */
#define CRESULT_TEMPLATE(T_OK, T_ERR)                                                             \
struct{                                                                                           \
  _Bool _ok;                                                                                      \
  union{                                                                                          \
    T_OK _ok_val;                                                                                 \
    T_ERR _err_val;                                                                               \
  }value;                                                                                         \
}

#if defined(CRESULT_C99)
/*
 * Return a CResult type ,already defined, in case of success:
 * @param T: Specialization Type of the CResult
 * @param ok_val: value of the result
 */
#define CRESULT_NEW_OK(T, ok_val)       ((T) {._ok = true, .value._ok_val = ok_val})

/*
 * Return a CResult type ,already defined, in case of failure:
 * @param T: Specialization Type of the CResult
 * @param ok_err: value of the error
 */
#define CRESULT_NEW_ERR(T, err_val)     ((T) {._ok = false, .value._err_val= err_val})

/*
 * Complete pattern match of a already defined specialized CResult:
 * @param self: instance of the specialized Cresult
 * @param res_val: name of the result/error that can be used in the user expressions
 * @param ok_exp: expression executed if self represent a successfully result
 * @param err_exp: expression executed if self represent a failure error
 */
#define CRESULT_FULL_MATCH(self, res_val, ok_exp, err_exp)                                        \
  do{                                                                                             \
    const __typeof__((self)) c_self = (self);                                                     \
    if(c_self._ok){                                                                               \
      __typeof__(c_self.value._ok_val) res_val = c_self.value._ok_val;                            \
      do{(ok_exp);}while(0);                                                                         \
    }else{                                                                                        \
      __typeof__(c_self.value._err_val) res_val = c_self.value._err_val;                          \
      do{(err_exp);}while(0);                                                                     \
    }                                                                                             \
  }while(0)

/*
 * Partial pattern match which executes if the specialized CResult represent a successful value
 * @param self: instance of the specialized Cresult
 * @param ok_val: name of the result that can be used in the user expressions
 * @param ok_exp: expression executed if self represent a result
 */
#define CRESULT_OK_MATCH(self, ok_val, ok_exp)                                                    \
  do{                                                                                             \
    const __typeof__((self)) c_self = (self);                                                     \
    if(c_self._ok){                                                                               \
      __typeof__(c_self.value._ok_val) ok_val = c_self.value._ok_val;                             \
      do{(ok_exp);}while(0);                                                                      \
    }                                                                                             \
  }while(0)

/*
 * Partial pattern match which executes if the specialized CResult represent a failed error
 * @param self: instance of the specialized Cresult
 * @param err_val: name of the error that can be used in the user expressions
 * @param err_exp: expression executed if self represent an error 
 */
#define CRESULT_ERR_MATCH(self, err_val, ok_exp)                                                  \
  do{                                                                                             \
    const __typeof__((self)) c_self = (self);                                                     \
    if(!c_self._ok){                                                                              \
      __typeof__(c_self.value._err_val) err_val = c_self.value._err_val;                          \
      do{(ok_exp);}while(0);                                                                      \
    }                                                                                             \
  }while(0)
#endif

/*
 * Set an existent specialized CResult as successful with a specific result
 * @param self: instance of the specialized Cresult
 * @param ok_val: user value to set
 */
#define CRESULT_SET_OK(self, ok_val)                                                              \
  do{                                                                                             \
    __typeof__((self)) *p_self = &(self);                                                         \
    p_self->_ok=true;                                                                             \
    p_self->value._ok_val = (ok_val);                                                             \
  }while(0)

/*
 * Set an existent specialized CResult as successful with a specific error
 * @param self: instance of the specialized Cresult
 * @param err_val: user error to set
 */
#define CRESULT_SET_ERR(self, err_val)                                                            \
  do{                                                                                             \
    __typeof__((self)) *p_self = &(self);                                                         \
    p_self->_ok=false;                                                                            \
    p_self->value._err_val = (err_val);                                                           \
  }while(0)

/*
 * Check if an existent specialized CResult has a value
 * @param self: instance of the specialized Cresult
 * RETURN: true if self has a value, false otherwise
 */
#define CRESULT_IS_OK(self)             ((self)._ok)

/*
 * Check if an existent specialized CResult has an error
 * @param self: instance of the specialized Cresult
 * RETURN: true if self has an error, false otherwise
 */
#define CRESULT_IS_ERR(self)            (!(self)._ok)

/*
 * UNCHECKED RETURN the value of an existent specialized CResult.
 * @param self: instance of the specialized Cresult
 * IF self has an error is UNDEFINED BEHAVIOUR
 */
#define CRESULT_OK_VAL(self)            ((self).value._ok_val)

/*
 * UNCHECKED RETURN the error of an existent specialized CResult.
 * @param self: instance of the specialized Cresult
 * IF self has a value is UNDEFINED BEHAVIOUR
 */
#define CRESULT_ERR_VAL(self)           ((self).value._err_val)

/*
 * Set the return type of a function to an already defined specialized CResult.
 * When set the return value of the function cannot be ignored.
 */
#define CRESULT_RETURN(self)            __attribute__((warn_unused_result)) self

/*
 * Remove the namespace guard of the libraries
 */

#ifdef CRESULT_NO_PREFIX
#define TEMPLATE(T_OK, T_ERR)                           CRESULT_TEMPLATE(T_OK, T_ERR)

#define SET_OK(self, ok_val)                            CRESULT_SET_OK(self, ok_val)
#define SET_ERR(self, err_val)                          CRESULT_SET_ERR(self, err_val)

#define IS_OK(self)                                     CRESULT_IS_OK(self)
#define IS_ERR(self)                                    CRESULT_IS_ERR(self)

#define OK_VAL(self)                                    CRESULT_OK_VAL(self)
#define ERR_VAL(self)                                   CRESULT_ERR_VAL(self)

#define RETURN(self)                                    CRESULT_RETURN(self)

#if defined(CRESULT_C99)
#define NEW_OK(T, ok_val)                               CRESULT_NEW_OK(T, ok_val)
#define NEW_ERR(T, err_val)                             CRESULT_NEW_ERR(T, err_val)

#define OK_MATCH(self, ok_val, ok_exp)                  CRESULT_OK_MATCH(self, ok_val, ok_exp)
#define ERR_MATCH(self, err_val, ok_exp)                CRESULT_ERR_MATCH(self, err_val, ok_exp)
#define FULL_MATCH(self, res_val, ok_exp, err_exp) \
  CRESULT_FULL_MATCH(self, res_val, ok_exp, err_exp)
#endif

#endif /* CRESULT_NO_PREFIX */
