//
//  GLog.h
//
//
//  Created by Gavin Chang on 2015/4/7.
//  Copyright (c) 2014年 WarRoom. All rights reserved.
//

#ifndef _TUTK_GLog_h
#define _TUTK_GLog_h

#ifdef DEBUG

  #ifdef LOGTOFILE
	//#import "iOSLogEngine.h"
	//轉印GLog內容至檔案
	#define GLog(cond,printf_exp) ((cond)?(GLogToFile printf_exp),1:0)

	#define GLogREL(cond,printf_exp) ((cond)?(GLogToFile printf_exp),1:0)
  #else
	#define GLog(cond,printf_exp) ((cond)?(NSLog printf_exp),1:0)

	#define GLogREL(cond,printf_exp) ((cond)?(NSLog printf_exp),1:0)
  #endif

#else

  #ifdef LOGTOFILE
	//#import "iOSLogEngine.h"
	#define GLog(cond,printf_exp) ((cond)?(GLogToFile printf_exp),1:0)

	#define GLogREL(cond,printf_exp) ((cond)?(GLogToFile printf_exp),1:0)
  #else
	#define GLog(cond,printf_exp)

	#define GLogREL(cond,printf_exp) ((cond)?(NSLog printf_exp),1:0)
  #endif

#endif

#endif
