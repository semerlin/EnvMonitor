#ifndef _TRACE_H_
  #define _TRACE_H_

//调试输出总开关
#define __ENABLE_TRACE

#ifdef __cplusplus
extern "C"{
#endif

  
#ifdef __ENABLE_TRACE
  /** 
   * @brief extern function, used to output message.
   *        if you want to use log system, you need to implement this function 
  * @param level: log level.1:error 2:warn 3:message
   * @param msg: message information 
   */
  extern void Log(unsigned char level, const char *msg);
  
  #define Trace_Msg(p)   (Log(3, (const char *)p))
  #define Trace_Warn(p)  (Log(2, (const char *)p))
  #define Trace_Error(p) (Log(1, (const char *)p)) 
#else
  #define Trace_Msg(p)   ((void)0)
  #define Trace_Warn(p)  ((void)0)
  #define Trace_Error(p) ((void)0)
#endif


#ifdef __cplusplus
}
#endif

#endif


