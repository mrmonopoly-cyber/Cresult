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

#define CRESULT_TEMPLATE(T_OK, T_ERR)                                                             \
struct{                                                                                           \
  _Bool _ok;                                                                                      \
  union{                                                                                          \
    T_OK _ok_val;                                                                                 \
    T_ERR _err_val;                                                                               \
  }value;                                                                                         \
}

#if defined(CRESULT_C99)
#define CRESULT_NEW_OK(T, ok_val)       ((T) {._ok = true, .value._ok_val = ok_val})
#define CRESULT_NEW_ERR(T, err_val)     ((T) {._ok = false, .value._err_val= err_val})
#define CRESULT_FULL_MATCH(self, res_val, ok_exp, err_exp)                                        \
  do{                                                                                             \
    const __typeof__((self)) c_self = (self);                                                     \
    if(c_self._ok){                                                                               \
      __typeof__(c_self.value._ok_val) res_val = c_self.value._ok_val;                            \
      do{ok_exp}while(0);                                                                         \
    }else{                                                                                        \
      __typeof__(c_self.value._err_val) res_val = c_self.value._err_val;                          \
      do{err_exp}while(0);                                                                        \
    }                                                                                             \
  }while(0)

#define CRESULT_OK_MATCH(self, ok_val, ok_exp)                                                    \
  do{                                                                                             \
    const __typeof__((self)) c_self = (self);                                                     \
    if(c_self._ok){                                                                               \
      __typeof__(c_self.value._ok_val) ok_val = c_self.value._ok_val;                             \
      do{ok_exp}while(0);                                                                         \
    }                                                                                             \
  }while(0)

#define CRESULT_ERR_MATCH(self, err_val, ok_exp)                                                  \
  do{                                                                                             \
    const __typeof__((self)) c_self = (self);                                                     \
    if(!c_self._ok){                                                                              \
      __typeof__(c_self.value._err_val) err_val = c_self.value._err_val;                          \
      do{ok_exp}while(0);                                                                         \
    }                                                                                             \
  }while(0)
#endif

#define CRESULT_SET_OK(self, ok_val)                                                              \
  do{                                                                                             \
    __typeof__((self)) *p_self = &(self);                                                         \
    p_self->_ok=true;                                                                             \
    p_self->value._ok_val = (ok_val);                                                             \
  }while(0)

#define CRESULT_SET_ERR(self, err_val)                                                            \
  do{                                                                                             \
    __typeof__((self)) *p_self = &(self);                                                         \
    p_self->_ok=false;                                                                            \
    p_self->value._err_val = (err_val);                                                           \
  }while(0)

#define CRESULT_IS_OK(self)             ((self)._ok)
#define CRESULT_IS_ERR(self)            (!(self)._ok)

#define CRESULT_OK_VAL(self)            ((self).value._ok_val)
#define CRESULT_ERR_VAL(self)           ((self).value._err_val)

#ifdef CRESULT_NO_PREFIX
#define TEMPLATE(T_OK, T_ERR)                           CRESULT_TEMPLATE(T_OK, T_ERR)

#define SET_OK(self, ok_val)                            CRESULT_SET_OK(self, ok_val)
#define SET_ERR(self, err_val)                          CRESULT_SET_ERR(self, err_val)

#define IS_OK(self)                                     CRESULT_IS_OK(self)
#define IS_ERR(self)                                    CRESULT_IS_ERR(self)

#define OK_VAL(self)                                    CRESULT_OK_VAL(self)
#define ERR_VAL(self)                                   CRESULT_ERR_VAL(self)

#if defined(CRESULT_C99)
#define NEW_OK(T, ok_val)                               CRESULT_NEW_OK(T, ok_val)
#define NEW_ERR(T, err_val)                             CRESULT_NEW_ERR(T, err_val)

#define OK_MATCH(self, ok_val, ok_exp)                  CRESULT_OK_MATCH(self, ok_val, ok_exp)
#define ERR_MATCH(self, err_val, ok_exp)                CRESULT_ERR_MATCH(self, err_val, ok_exp)
#define FULL_MATCH(self, res_val, ok_exp, err_exp) \
  CRESULT_FULL_MATCH(self, res_val, ok_exp, err_exp)
#endif

#endif /* CRESULT_NO_PREFIX */
