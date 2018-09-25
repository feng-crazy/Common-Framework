#ifndef DEBUGCTRL_H
#define DEBUGCTRL_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <Qdebug>

#ifndef QT_NO_DEBUG

#define mdbg \
    printf("%s(line %d): %s()\t$$$$$$$$$$$$$$$$$$$$\n", __FILE__, __LINE__, __FUNCTION__);

#define m_debug(args) \
do { \
    printf("%s(line %d): %s():\t", __FILE__, __LINE__, __FUNCTION__); \
    printf(args); \
} while(false)

#define IF(arg) \
    if(arg) \
        printf(">>>!! ASSERT was hitted in %s(line %d) !!\n", \
                __FILE__, __LINE__); \
    if(arg)

#define debug_out_str(type, buff, len) \
for(type i = 0; i < len; i++) \
{\
    printf("0x%x ", buff[i]); \
} \
printf("\t%s:%d\n", __FILE__, __LINE__)
#else

#define mdbg
#define m_debug(args)
#define IF(arg) if(arg)
#define out_str(type, buff, len)
#endif // NDEBUG

#define m_warning(args) \
do { \
    printf("%s(line %d): %s(): ", __FILE__, __LINE__, __FUNCTION__); \
    printf(args); \
} while(false)


#endif // DEBUGCTRL_H
