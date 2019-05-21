//
//  GLogZone.h
//  
//
//  Created by Gavin Chang on 2014/5/25.
//  Copyright (c) 2014年 WarRoom. All rights reserved.
//

#ifndef _TUTK_GLog_Zone_h
#define _TUTK_GLog_Zone_h

extern unsigned int g_dwGLogZoneSeed;


#define tAll_MSK				-1
#define tUI_MSK					(1)				//trace UI flow
#define tCtrl_MSK				(1<< 1)			//trace Control
#define tMemory_MSK				(1<< 2)			//trace Memory load
#define tPushNotify_MSK			(1<< 3)			//trace TPNS
#define tAudioDecode_MSK		(1<< 4)			//trace audio decode
#define tReStartShow_MSK		(1<< 5)			//
#define tHWDecode_Alex_MSK		(1<< 6)			//trace HWDecode decode
#define tHWDecode_MSK			(1<< 7)			//trace HWDecode decode
#define tSoap_MSK				(1<< 8)			//trace Soap
#define tHttp_MSK				(1<< 9)
#define tStartShow_MSK			(1<< 10)
#define tPinchZoom_MSK			(1<< 11)
#define tUserDefaults_MSK		(1<< 12)
#define tForeBackground_MSK		(1<< 13)
#define tTimeStamp_MSK			(1<< 14)
#define tAudioUnitPLAY_MSK		(1<< 15)
#define tAudioUnitRECORD_MSK	(1<< 16)
#define tDecode_MSK				(1<< 17)
#define tStartRecvFrame_MSK		(1<< 18)

#define tAll					(1)
#define tUI						(g_dwGLogZoneSeed & tUI_MSK)
#define tCtrl					(g_dwGLogZoneSeed & tCtrl_MSK)
#define tMemory					(g_dwGLogZoneSeed & tMemory_MSK)
#define tPushNotify				(g_dwGLogZoneSeed & tPushNotify_MSK)
#define tAudioDecode			(g_dwGLogZoneSeed & tAudioDecode_MSK)
#define tReStartShow			(g_dwGLogZoneSeed & tReStartShow_MSK)
#define tHWDecode_Alex			(g_dwGLogZoneSeed & tHWDecode_Alex_MSK)
#define tHWDecode				(g_dwGLogZoneSeed & tHWDecode_MSK)
#define tSoap					(g_dwGLogZoneSeed & tSoap_MSK)
#define tHttp					(g_dwGLogZoneSeed & tHttp_MSK)
#define tStartShow				(g_dwGLogZoneSeed & tStartShow_MSK)
#define tPinchZoom				(g_dwGLogZoneSeed & tPinchZoom_MSK)
#define tUserDefaults			(g_dwGLogZoneSeed & tUserDefaults_MSK)
#define tForeBackground			(g_dwGLogZoneSeed & tForeBackground_MSK)
#define tTimeStamp				(g_dwGLogZoneSeed & tTimeStamp_MSK)
#define tAudioUnitPLAY			(g_dwGLogZoneSeed & tAudioUnitPLAY_MSK)
#define tAudioUnitRECORD		(g_dwGLogZoneSeed & tAudioUnitRECORD_MSK)
#define tDecode					(g_dwGLogZoneSeed & tDecode_MSK)
#define tStartRecvFrame			(g_dwGLogZoneSeed & tStartRecvFrame_MSK)


#endif
