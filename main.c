#include <stdio.h>

#define CRESULT_NO_PREFIX
#include "Cresult.h"


typedef TEMPLATE(int, char*) CResult_int_char;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
CResult_int_char foo(int i){
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

int main()
{
  CResult_int_char res = foo(2);

  if (IS_OK(res)){
    printf("res: %d\n", OK_VAL(res));
  }else{
    printf("res: %s\n", ERR_VAL(res));
  }

  res = foo(5);

  if (IS_OK(res)){
    printf("res: %d\n", OK_VAL(res));
  }else{
    printf("res: %s\n", ERR_VAL(res));
  }

  return 0;
}
