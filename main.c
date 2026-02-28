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
