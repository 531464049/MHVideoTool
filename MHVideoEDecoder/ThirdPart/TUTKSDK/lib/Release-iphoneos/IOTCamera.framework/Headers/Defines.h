//
//  Defines.h
//  IOTCamera
//
//  Created by steven_yang on 16/12/5.
//  Copyright © 2016年 tutksz_ios. All rights reserved.
//

#ifndef Defines_h
#define Defines_h

#define CONNECTION_MODE_NONE            -1
#define CONNECTION_MODE_P2P             0
#define CONNECTION_MODE_RELAY           1
#define CONNECTION_MODE_LAN             2

/* used for display status */
#define CONNECTION_STATE_NONE               0
#define CONNECTION_STATE_CONNECTING         1
#define CONNECTION_STATE_CONNECTED          2
#define CONNECTION_STATE_DISCONNECTED       3
#define CONNECTION_STATE_UNKNOWN_DEVICE     4
#define CONNECTION_STATE_WRONG_PASSWORD     5
#define CONNECTION_STATE_TIMEOUT            6
#define CONNECTION_STATE_UNSUPPORTED        7
#define CONNECTION_STATE_CONNECT_FAILED     8
#define CONNECTION_STATE_SLEEP              -64


#define DONE                                1
#define NOTDONE                             0

#define DEF_QUEUE_NUM                       10
#define RECV_VIDEO_BUFFER_SIZE              512*1024
#define DEF_Latency_EnterDropFrame          2000

#define DEF_testData                        0
#define DEF_AUDIO_16K                       0

#define RECV_VIDEO_BUFFER_SIZE              512*1024
#define RECV_AUDIO_BUFFER_SIZE              1280
#define SPEEX_FRAME_SIZE                    160
#define MAX_IOCTRL_BUFFER_SIZE              1024
#define kQualityMonitorPeriod               60000


#endif /* Defines_h */
