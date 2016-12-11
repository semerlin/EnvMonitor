#include "board.h"
#include "sysdef.h"

void board_init(void)
{
}






#ifdef __DEBUG
void assert_failed(const char *file, unsigned int line)
{
}
#endif


#ifdef __ENABLE_TRACE
void Log(unsigned char level, const char *msg)
{
}
#endif