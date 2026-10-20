/**
* @file getint.h
* @brief getint.c のヘッダー
* @author yuji katsuta
* @date 2022.05.26
*/
//#define LINUX
// Linux -> gcc -DLINUX -Wall -o testgint.exe testgint.c getint.c
// H8 ----> make PROG=h8sample OBJSA=getint.o 
// H8 ----> make PROG=h8sample MINICOM=1 OBJSA=getint.o 
/* H8 */
#pragma once
#ifndef LINUX
// h8/reg3068.hやmes2.hがインクルードガードをしていないため、
// ここにガードを付け加えました（田辺）
#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include<h8/reg3067.h>
#include<mes2.h>	
#endif //of #ifndef H8_H
#else
/* unix */
#include <stdio.h>
#include <stdlib.h>
#endif
/* LF */
#ifndef MINICOM 
#define LF "\n"
#else
#define LF "\r"
#endif
/* */

/**
* @brief 文字列をintに変換する関数
* @param [in] str 変換する文字列
* @param [out] geti int
* @retval 0 complete
* @retval 1 error
* @details 
*
* 文字列が整数であれば整数に変換し、
* *geti に代入し、complete:0を返す
* 文字列が整数でない場合は、
* error:1を返す
*/
int s2int(char *str, int *geti);
/**
* @brief argvをintに変換する関数
* @param [in] argc argvの個数
* @param [in] argv オプション文字列
* @param [in] str compare 文字列
* @param [out] geti int
* @retval 0 complete
* @retval 1 error
* @details 
*
* strに続く文字列が整数であれば整数に変換し、
* *geti に代入し、complete:0を返す
* strと一致しない場合と、文字列が整数でない場合は、
* error:1を返す
*/
int getint(int argc, char **argv, char *str, int *geti);

#ifdef KATOWN
#define KATEXT
#else
#define KATEXT extern
#endif

/**
* @brief デバッグ用変数 0:通常、1:degn1prinf ON 
*/
KATEXT int debgnflag; //debgnflag=1 -> debgn1printf==printf

#undef KATEXT
#undef KATOWN

#define debgn1printf if (debgnflag&1) printf

#define KATCOMP 0
#define KATERR  1

