/*
コンパイル： gcc -Wall -DLINUX -o main100 getint.c main.c
（実行ファイルをファイル名main100で生成する。）
サンプル実行： ./main100 -a10 -b20 -b0xff 
→
argc: 4
argv[0]: "./main100"
argv[1]: "-a10"
argv[2]: "-b20"
argv[3]: "-b0xff"
*/

#include<stdio.h>
#include "getint.h"

int main(int argc, char ** argv){
    int i;
    int val = 0; //getint呼び出し時に代入された値
    int ret = 0; //getintの返却値
    /* main関数の仮引数argcおよびargvの中身を確認しよう */
    printf("argc = %d\n", argc);
    for(i = 0; i < argc; i++){
        printf("%d: %s\n", i, argv[i]);
    }


    /* getint.h内のgetint関数のコメントと実行結果を比較して、仕様を理解しよう */
    // オプション「-a」の値を調べる。
    val = 0;
    ret = getint(argc, argv, "-a", &val);
    printf("test1: ret=%d, val=%d\n", ret, val);

    // オプション「-b」の値を調べる。
    // なぜこのような出力になるのか？
    val = 0;
    ret = getint(argc, argv, "-b", &val);
    printf("test2: ret=%d, val=%d\n", ret, val);

    // オプション「./main」の値を調べる。
    // これで良いのか？
    val = 0;
    ret = getint(argc, argv, "./main", &val);
    printf("test3: ret=%d, val=%d\n", ret, val);

    return 0;
}