#include <stdlib.h>

#include <string.h>
#include <ctype.h>

#define KATOWN
#include "getint.h"

int s2int(char *str, int *geti){
	int i=0;
	int a=0,b=1;
	
	if ( (str[0]=='0')&&(str[1]=='x') ) {
	  i=2;
	  for (;(isdigit(str[i]))||
			 ((str[i]>='a')&&(str[i]<='f'))||
			 ((str[i]>='A')&&(str[i]<='F'))
			 ;i++) {
		if (isdigit(str[i])) a=16*a+(str[i]-'0');
		if ((str[i]>='a')&&(str[i]<='f')) a=16*a+10+(str[i]-'a');
		if ((str[i]>='A')&&(str[i]<='F')) a=16*a+10+(str[i]-'A');
	  }
	  if (str[i]=='\0') {
			*geti=a;
			return KATCOMP;
		}
	  return KATERR;
	}
	if ( (str[0]=='0')&&(str[1]=='b') ) {
	  i=2;
	  for (;((str[i]>='0')&&(str[i]<='1'));i++) {
	    a=2*a+(str[i]-'0');
	  }
	  if (str[i]=='\0') {
			*geti=a;
			return KATCOMP;
		}
	  return KATERR;
	}
	
	if ( str[0]=='-' ) {
		b=-1;
		i=1;
	}
	if (str[i]=='\0') return 1;
	for (;isdigit(str[i]);i++) {
		a=10*a+(str[i]-'0');
	}
	*geti=a*b;
	if (str[i]=='\0') return 0;
	return KATERR;
}

int getint(int argc, char **argv, char *str, int *geti)
{
	int i;
	int ret=KATERR;
	int a;
	for (i=0;i<argc;i++) {
		if (strncmp(argv[i], str, strlen(str))==0) {
			if (s2int(argv[i]+strlen(str),&a)!=0) {
				debgn1printf("%s is not int!"LF,argv[i]+strlen(str));
			}else{
				*geti=a;
				ret=KATCOMP;
			}
		}
	}
	return ret;
}

