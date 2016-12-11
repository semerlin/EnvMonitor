#ifndef _ASSERT_H_
  #define _ASSERT_H_


#ifdef __DEBUG
/**
 * @brief  The assert macro is used for function's parameters check.
 * @param  expr: If expr is false, it calls assert_failed function which reports 
 *         the name of the source file and the source line number of the call 
 *         that failed. If expr is true, it returns no value.
 * @reutn None
*/
  /* external function */
  extern void assert_failed(const char *file, unsigned int line);
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((const char *)__FILE__, __LINE__))
#else
  #define assert_param(expr) ((void)0)
#endif 


#endif