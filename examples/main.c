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
