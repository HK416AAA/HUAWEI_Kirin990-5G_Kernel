
#ifndef __EAGLEYE_H__
#define __EAGLEYE_H__

/*
 * 1 其他头文件包含
 */

/*
 * 2 宏定义
 */
#ifdef __EAGLEYE__
extern void Coverity_Tainted_Set(void *pkt);
#define COVERITY_TAINTED_SET(pkt) (Coverity_Tainted_Set(pkt))
#else
#define COVERITY_TAINTED_SET(pkt)
#endif

/*
 * 3 枚举定义
 */

/*
 * 4 消息头定义
 */

/*
 * 5 消息定义
 */

/*
 * 6 STRUCT定义
 */

/*
 * 7 UNION定义
 */

/*
 * 8 OTHERS定义
 */

/*
 * 9 全局变量声明
 */

/*
 * 10 函数声明
 */

#endif /* end eagleye */

