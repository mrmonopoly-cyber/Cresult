#pragma once

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
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

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define CRESULT_NEW_OK(T, ok_val)       ((T) {._ok = true, .value._ok_val = ok_val})
#define CRESULT_NEW_ERR(T, err_val)     ((T) {._ok = false, .value._err_val= err_val})
#endif

#define CRESULT_SET_OK(self, ok_val)                                                              \
  do{                                                                                             \
    (self)._ok=true;                                                                              \
    (self).value._ok_val = (ok_val);                                                              \
  }while(0)

#define CRESULT_SET_ERR(self, err_val)                                                            \
  do{                                                                                             \
    (self)._ok=false;                                                                             \
    (self).value._err_val = (err_val);                                                            \
  }while(0)

#define CRESULT_IS_OK(self)             ((self)._ok)
#define CRESULT_IS_ERR(self)            (!(self)._ok)

#define CRESULT_OK_VAL(self)            ((self).value._ok_val)
#define CRESULT_ERR_VAL(self)           ((self).value._err_val)

#ifdef CRESULT_NO_PREFIX
#define TEMPLATE(T_OK, T_ERR)           CRESULT_TEMPLATE(T_OK, T_ERR)

#define NEW_OK(T, ok_val)               CRESULT_NEW_OK(T, ok_val)
#define NEW_ERR(T, err_val)             CRESULT_NEW_ERR(T, err_val)

#define SET_OK(self, ok_val)            CRESULT_SET_OK(self, ok_val)
#define SET_ERR(self, err_val)          CRESULT_SET_ERR(self, err_val)

#define IS_OK(self)                     CRESULT_IS_OK(self)
#define IS_ERR(self)                    CRESULT_IS_ERR(self)

#define OK_VAL(self)                    CRESULT_OK_VAL(self)
#define ERR_VAL(self)                   CRESULT_ERR_VAL(self)

#endif /* CRESULT_NO_PREFIX */
