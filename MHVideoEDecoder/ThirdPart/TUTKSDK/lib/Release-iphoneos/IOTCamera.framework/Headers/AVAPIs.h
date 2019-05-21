/*! \file AVAPIs.h
This file describes all the APIs of the AV module in IOTC platform.
AV module is a kind of data communication modules in IOTC platform to provide
fluent streaming Audio / Video data from AV servers to AV clients in
unidirectional way.

\copyright Copyright (c) 2010 by Throughtek Co., Ltd. All Rights Reserved.

Revision Table

Version     | Name             |Date           |Description
------------|------------------|---------------|-------------------
3.1.10.5    |Terry Liu         |2018-12-17     |o Mark avServStart, avRecvFrameData as deprecated
3.1.10.5    |Terry Liu         |2018-12-17     |- Remove AVAPI2.h (remove all AV2 APIs)
 */


#ifndef _AVAPIs_H_
#define _AVAPIs_H_

#include "IOTCAPIs.h"

/* ============================================================================
 * Platform Dependant Macro Definition
 * ============================================================================
 */


#ifdef _WIN32
    /** @cond */
    #ifdef IOTC_STATIC_LIB
    #define AVAPI_API
    #elif defined AVAPI_EXPORTS
    #define AVAPI_API __declspec(dllexport)
    #else
    #define AVAPI_API __declspec(dllimport)
    #endif // #ifdef P2PAPI_EXPORTS
    /** @endcond */
#else // #ifdef _WIN32
	#define AVAPI_API
#endif //#ifdef _WIN32

#if defined(__GNUC__) || defined(__clang__)
    #define AVAPI_API_DEPRECATED __attribute__((deprecated)) 
    #elif defined(_MSC_VER)
    #ifdef IOTC_STATIC_LIB
    #define AVAPI_API_DEPRECATED __declspec(deprecated)
        #elif defined P2PAPI_EXPORTS
    #define AVAPI_API_DEPRECATED __declspec(deprecated, dllexport)
    #else
    #define AVAPI_API_DEPRECATED __declspec(deprecated, dllimport)
    #endif
#else
    #define AVAPI_API_DEPRECATED
#endif

#ifndef _WIN32
#define __stdcall
#endif // #ifndef _WIN32

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ============================================================================
 * Generic Macro Definition
 * ============================================================================
 */

/** The maximum size of AV IO Control data to be sent, in unit of byte. */
#define AV_MAX_IOCTRL_DATA_SIZE					1024

/** The maximum size of audio data to be sent, in unit of byte. */
#define AV_MAX_AUDIO_DATA_SIZE					1280

/** The maximum length of identity, exclude the Null character '\0', in unit of byte.
 *  If DTLS security mode is enabled, or auth type is set to AV_AUTH_TOKEN,
 *  the the max lenth of account or identity should not exceed this value.
 */
#define MAX_IDENTITY_LENGTH                     119

/** The maximum length of identity description, exclude the Null character '\0', in unit of byte. */
#define MAX_IDENTITY_DESCRIPTION_LENGTH         127

/** The maximum length of token, exclude the Null character character '\0', in unit of byte. */
#define MAX_TOKEN_LENGTH                        1024

/** The maximum length of account, include the Null character '\0', in unit of byte. 
 *  If security mode is AV_SECURITY_SIMPLE, or avClientStart/avClientStart2 is used,
 *  the the max lenth of account should not exceed this value.
 */
#define NEW_MAXSIZE_VIEWACC                     257

/** The maximum length of password, include the Null character '\0', in unit of byte. */
#define NEW_MAXSIZE_VIEWPWD                     257
#define OLD_MAXSIZE_VIEWACC                     16
#define OLD_MAXSIZE_VIEWPWD                     16

/* ============================================================================
 * Error Code Declaration
 * ============================================================================
 */
/** The function is performed successfully. */
#define		AV_ER_NoERROR						 0

/** The passed-in arguments for the function are incorrect */
#define		AV_ER_INVALID_ARG					-20000

/** The buffer to receive is too small to store */
#define		AV_ER_BUFPARA_MAXSIZE_INSUFF		-20001

/** The number of AV channels has reached maximum.
 * The maximum number of AV channels is determined by the passed-in
 * argument of avInitialize() */
#define		AV_ER_EXCEED_MAX_CHANNEL			-20002

/** Insufficient memory for allocation */
#define		AV_ER_MEM_INSUFF					-20003

/** AV fails to create threads. Please check if OS has ability to create threads for AV. */
#define		AV_ER_FAIL_CREATE_THREAD			-20004

/** A warning error code to indicate that the sending queue of video frame of an AV server
 * is almost full, probably caused by slow handling of an AV client or network
 * issue. Please note that this is just a warning, the video frame is actually
 * put in the queue. */
#define 	AV_ER_EXCEED_MAX_ALARM				-20005

/** The frame to be sent exceeds the currently remaining video frame buffer.
 * The maximum of video frame buffer is controlled by avServSetResendSize() */
#define		AV_ER_EXCEED_MAX_SIZE				-20006

/** The specified AV server has no response */
#define		AV_ER_SERV_NO_RESPONSE				-20007

/** An AV client does not call avClientStart() yet */
#define		AV_ER_CLIENT_NO_AVLOGIN				-20008

/** The client fails in authentication due to incorrect view account or password */
#define		AV_ER_WRONG_VIEWACCorPWD			-20009

/** The IOTC session of specified AV channel is not valid */
#define		AV_ER_INVALID_SID					-20010

/** The specified timeout has expired during some operation */
#define		AV_ER_TIMEOUT						-20011

/** The data is not ready for receiving yet. */
#define		AV_ER_DATA_NOREADY					-20012

/** Some parts of a frame are lost during receiving */
#define		AV_ER_INCOMPLETE_FRAME				-20013

/** The whole frame is lost during receiving */
#define		AV_ER_LOSED_THIS_FRAME				-20014

/** The remote site already closes the IOTC session.
 * Please call IOTC_Session_Close() to release local IOTC session resource */
#define		AV_ER_SESSION_CLOSE_BY_REMOTE		-20015

/** This IOTC session is disconnected because remote site has no any response
 * after a specified timeout expires. */
#define		AV_ER_REMOTE_TIMEOUT_DISCONNECT		-20016

/** The AV server API process is terminated by avServExit() or avServStop()*/
#define		AV_ER_SERVER_EXIT		    		-20017

/** The AV client API process is terminated by avClientExit() or avClientStop()*/
#define		AV_ER_CLIENT_EXIT		    		-20018

/** AV module has not been initialized */
#define		AV_ER_NOT_INITIALIZED	    		-20019

/** By design, an AV client cannot send frame and audio data to an AV server */
#define		AV_ER_CLIENT_NOT_SUPPORT	   		-20020

/** The AV channel of specified AV channel ID is already in sending IO control process */
#define		AV_ER_SENDIOCTRL_ALREADY_CALLED	   	-20021

/** The sending IO control process is terminated by avSendIOCtrlExit() */
#define		AV_ER_SENDIOCTRL_EXIT		    	-20022

/** The UID is a lite UID */
#define		AV_ER_NO_PERMISSION                     -20023

/** The length of password is wrong */
#define		AV_ER_WRONG_ACCPWD_LENGTH				-20024

/** IOTC session has been closed */
#define		AV_ER_IOTC_SESSION_CLOSED				-20025

/** IOTC is deinitialized */
#define		AV_ER_IOTC_DEINITIALIZED				-20026

/** IOTC channel is used by other av channel */
#define		AV_ER_IOTC_CHANNEL_IN_USED				-20027

/** AV channel is waiting key frame */
#define		AV_ER_WAIT_KEY_FRAME				    -20028

/** The AV channel of specified AV channel ID is already in reset buffer process */
#define		AV_ER_CLEANBUF_ALREADY_CALLED	   	    -20029

/** IOTC UDP/TCP socket send queue is full. */
#define 	AV_ER_SOCKET_QUEUE_FULL					-20030

/** AV module is already initialized. It is not necessary to re-initialize. */
#define     AV_ER_ALREADY_INITIALIZED               -20031

/** Dynamic Adaptive Streaming over AVAPI notified program to clean buffer */
#define		AV_ER_DASA_CLEAN_BUFFER                 -20032

/** Function is not support, need to using correct AV Server and AV Client  */
#define		AV_ER_NOT_SUPPORT                       -20033

/** AV fails to initialize DTLS module. */
#define     AV_ER_FAIL_INITIALIZE_DTLS              -20034

/** AV fails to create channel for DTLS connection. */
#define     AV_ER_FAIL_CREATE_DTLS                  -20035

/** The AV channel of specified AV channel ID is already in request process */
#define		AV_ER_REQUEST_ALREADY_CALLED	   	    -20036

/** Function is not supported on remote side. */
#define		AV_ER_REMOTE_NOT_SUPPORT                -20037

/** The token to be sent exceeds MAX_TOKEN_LENGTH. */
#define		AV_ER_TOKEN_EXCEED_MAX_SIZE             -20038

/** The server fail because client not support DTLS */
#define     AV_ER_REMOTE_NOT_SUPPORT_DTLS           -20039

/** The server fail in authentication due to incorrect password/token with DTLS security mode enabled*/
#define     AV_ER_DTLS_WRONG_PWD					-20040

/** The server fail in authentication due to the passowrd/token auth function return value is smaller than 0*/
#define     AV_ER_DTLS_AUTH_FAIL					-20041

/** The SDK Version not support DTLS */
#define     AV_ER_SDK_NOT_SUPPORT_DTLS              -21334

/* ============================================================================
 * Enumeration Declaration
 * ============================================================================
 */

struct st_AvStatus
{
    unsigned int AvVersion;
    unsigned short RoundTripTime;
    unsigned short LostRate;
    unsigned short BandWidth;
    unsigned int MinRTT;
    unsigned int LastBw;
    unsigned int LastRtt;
    unsigned int LastCwnd;
    unsigned int InFlight;
};


typedef struct AvIdentity {
    unsigned int length;
    char identity[MAX_IDENTITY_LENGTH + 1];
} AvIdentity;

/**
 * \details The types for AV IO control. AV IO control are used	for AV servers
 *			and AV clients to communicate control message with each other.
 *			The types contains system defined types and user defined ones.
 *			The range of system defined	types is from 0x00 to 0xFF, while
 *			user defined ones is starting from 0x100.
 */

typedef enum
{
	/// Used by AV clients to tell AV servers to change the time interval
	/// of sending packets by avSendFrameData() when AV clients are too slow
	/// to handle frame data.
	IOTYPE_INNER_SND_DATA_DELAY = 0xFF,

    /// Used by AV servers to tell AV clients statistical data
    IOTYPE_INNER_STATISTICAL_DATA = 0x01,

    /// Used by AV clients to tell AV servers statistical data
    IOTYPE_INNER_STATISTICAL_CLIENT_DATA = 0x02,

	/// The starting enum value of user defined IO types. It is suggested
	/// to assign this to the first one of user defined types, like <br>
	/// enum IOTYPE_USER_DEFINED { <br>
	/// IOTYPE_USER_PLAY = IOTYPE_USER_DEFINED_START, <br>
	/// IOTYPE_USER_STOP <br>
	/// }
	IOTYPE_USER_DEFINED_START = 0x100
} AVIOCtrlType;

typedef enum _AV_RESET_TARGET
{
	RESET_VIDEO = 0,
	RESET_AUDIO,
	RESET_ALL
} AV_RESET_TARGET;

typedef enum _AV_DASA_LEVEL_
{
    AV_DASA_LEVEL_QUALITY_HIGH          = 0,
    AV_DASA_LEVEL_QUALITY_BTWHIGHNORMAL = 1,
    AV_DASA_LEVEL_QUALITY_NORMAL        = 2,
    AV_DASA_LEVEL_QUALITY_BTWNORMALLOW  = 3,
    AV_DASA_LEVEL_QUALITY_LOW           = 4
} AV_DASA_LEVEL;

typedef enum AvSecurityMode
{
	AV_SECURITY_SIMPLE,
	AV_SECURITY_DTLS,

    /// Only can be used by avClientStartEx,
    /// client will detect the security mode
    /// of av server and connect it
    AV_SECURITY_AUTO
} AvSecurityMode;

typedef enum AvAuthType
{
	AV_AUTH_PASSWORD,
	AV_AUTH_TOKEN
} AvAuthType;

/* ============================================================================
 * Type Definition
 * ============================================================================
 */

/**
 * \details The prototype of authentication function used for an AV server.
 *			The authentication function is set when an AV server starts by avServStart2().
 *			The AV server will call back authentication function when an AV
 *			client wants to connect with szViewAccount and szViewAccount
 * \param szViewAccount [in] The view account provided by AV clients for authentication
 * \param szViewPassword [in] The view password provided by AV clients for authentication
 *
 * \return true if authentication passes and false if fails.
 *
 */
typedef int(__stdcall *authFn)(const char *szViewAccount, const char *szViewPassword);

/**
 * \details The prototype of account/password authentication function used for an AV server.
 *          The authentication function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avPasswordAuthFn function when an AV
 *          client wants to connect with account and password
 * \param account [in] The account provided by AV clients for authentication
 * \param password [out] The buffer that server need to fill in the password string for authentication
 * \param password_buf_size [in] The size of password buffer
 *
 * \return 0 if account/password authentication passes.
 *
 */
typedef int(__stdcall *avPasswordAuthFn)(const char *account, char *password, unsigned int password_buf_size);

/**
 * \details The prototype of identity/token authentication function used for an AV server.
 *          The authentication function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avTokenAuthFn function when an AV
 *          client wants to connect with identity and token
 *
 * \param identity [in] The identity provided by AV clients for authentication
 * \param token [out] The buffer that AV server need to fill in the token string for authentication
 * \param token_buf_size [in] The size of token buffer
 *
 * \return 0 if identity/token authentication passes.
 *
 */
typedef int(__stdcall *avTokenAuthFn)(const char *identity, char *token, unsigned int token_buf_size);

/**
 * \details The prototype of token request function used for an AV server.
 *          The token request function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avTokenRequestFn function when an AV client use
 *          avClientRequestTokenWithIdentity() to request a token with a specific identity
 *
 * \param av_index [in] The channel ID of the AV channel who request the token
 * \param identity [in] The identity provided by AV clients for request token
 * \param identity_description [in] The identity description provided by AV clients that describes the identity
 * \param token [out] The buffer that AV server need to fill in the token string 
 * \param token_buf_size [in] The buffer size provided by AV client who called #avClientRequestTokenWithIdentity
 *
 * \return The status_code #avClientRequestTokenWithIdentity() receives.
 *
 * \see avClientRequestTokenWithIdentity()
 *
 */
typedef int(__stdcall *avTokenRequestFn)(int av_index,
                                         const char *identity,
                                         const char *identity_description,
                                         char *token, unsigned int token_buf_size);

/**
 * \details The prototype of token delete function used for an AV server.
 *          The token delete function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avTokenDeleteFn function when an AV client use
 *          avClientRequestDeleteTokenWithIdentity() to delete a token with a specific identity
 *
 * \param av_index [in] The channel ID of the AV channel who request to delete token
 * \param identity [in] The identity provided by AV clients for delete token
 *
 * \return 0 if delete token success.
 *
 * \see avClientRequestDeleteTokenWithIdentity()
 *
 */
typedef int(__stdcall *avTokenDeleteFn)(int av_index, const char *identity);

/**
 * \details The prototype of send identity array function used for an AV server.
 *          The send identity array function must be called in avIdentityArrayRequestFn
 *          call back function to send identity array to AV client
 *
 * \param av_index [in] The channel ID of the target AV channel
 * \param identities [in] The identity array to be sent
 * \param identity_count [in] The identity count to be sent
 * \param status_code [in] The status_code #avClientRequestIdentityArray() receives.
 *
 * \return #AV_ER_NoERROR if send identity array successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG AV index is invalid or non-zero identity_count with NULL identities
 *          - #AV_ER_NOT_INITIALIZED
 *
 * \see avIdentityArrayRequestFn
 *
 */
typedef int(__stdcall *avServSendIdentityArray)(int av_index,
                                      const AvIdentity *identities,
                                      unsigned int identity_count,
                                      int status_code);

/**
 * \details The prototype of identity array request function used for an AV server.
 *          The identity array request function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avIdentityArrayRequestFn function when an AV client use
 *          avClientRequestIdentityArray() to request the identity array stored in AV server
 *
 * \param av_index [in] The channel ID of the AV channel who request the identity array
 * \param send_identity_array [in] The function pointer of avServSendIdentityArray function, used for send identity array to AV client.
 *
 * \see avClientRequestIdentityArray(), avServSendIdentityArray
 *
 */
typedef void(__stdcall *avIdentityArrayRequestFn)(int av_index, avServSendIdentityArray send_identity_array);

/**
 * \details The prototype of send ability function used for an AV server.
 *          The send ability function must be called in avAbilityRequestFn
 *          call back function to send ability to AV client
 *
 * \param av_index [in] The channel ID of the target AV channel
 * \param ability [in] The ability buffer to be sent
 * \param ability_size [in] The size of ability
 *
 * \return #AV_ER_NoERROR if valid ablility
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG AV index is invalid or ability is NULL
 *          - #AV_ER_NOT_INITIALIZED
 *
 * \see avAbilityRequestFn()
 *
 */
typedef int (__stdcall *avServSendAbility)(int av_index, const unsigned char *ability, unsigned int ability_size);

/**
 * \details The prototype of ability request function used for an AV server.
 *          The ability request function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avAbilityRequestFn function when an AV client use
 *          avClientRequestServerAbility() to request the ability of AV server
 *
 * \param av_index [in] The channel ID of the AV channel who request the AV server ability
 * \param send_ability [in] The function pointer of avServSendAbility function, used for send server ability to AV client.
 *
 * \see avClientRequestServerAbility()
 *
 */
typedef void(__stdcall *avAbilityRequestFn)(int av_index, avServSendAbility send_ability);

/**
 * \details The prototype of change password request function used for an AV server.
 *          The change password function is set when an AV server starts by avServStartEx().
 *          The AV server will call back avChangePasswordRequestFn function when an AV client use
 *          avClientRequestChangeServerPassword() to request change password.
 *          This function is only supported with IOTC authkey enabled, AV server should verify 
 *          the correctness of account and old password provide by AV client, 
 *          once the account/password authentication succeed and the new password is applied, 
 *          AV server should save the new IOTC auth key, and this new IOTC auth key must be applied 
 *          in the next call of IOTC_Device_LoginEx()
 *
 * \param av_index [in] The channel ID of the AV channel who request change password
 * \param account [in] The account provide by AV client
 * \param old_password [in] The old password provide by AV client
 * \param new_password [in] The new password provide by AV client
 * \param new_iotc_authkey [in] The new IOTC auth key provide by AV module
 *
 * \return 0 if change password success, then AV module will apply new IOTC auth key to IOTC login process 
 *           immediately, and login to server with this new IOTC key until the AV server restart whole proccess.
 *
 * \see avClientRequestChangeServerPassword()
 *
 */
typedef int(__stdcall *avChangePasswordRequestFn)(
    int av_index,
    const char *account,
    const char *old_password,
    const char *new_password,
    const char *new_iotc_authkey);

/**
 * \details The input configuration of avServStartEx
 *
 * \param cb [in] The check byte of this structure.
 * \param iotc_session_id [in] The session ID of the IOTC session to create AV server
 * \param iotc_channel_id [in] The channel ID of the channel to create AV server
 * \param timeout_sec     [in] The timeout for this function in unit of second.
 *                             Specify it as 0 will make AV server start process wait
 *                             until an AV client connects successfully.
 * \param server_type     [in] The user-defined service type. An AV client will get
 *                             this value when it invokes avClientStart() successfully.
 * \param resend          [in] Enabling the Resend mechanism or not
 * \param security_mode   [in] Security mode with options
 * \param password_auth   [in] The function pointer of password authentication function
 * \param token_auth      [in] The function pointer of token authentication function
 * \param token_request   [in] The function pointer will be called if avClientRequestTokenWithIdentity
 *                             is called in client side to request a token
 * \param token_delete    [in] The function pointer will be called if avClientRequestDeleteTokenWithIdentity
 *                             is called in client side to delete a token
 * \param identity_array_request  [in] The function pointer will be called if avClientRequestIdentityArray
 *                                     is called in client side to request the identity list
 * \param ability_request [in] The function pointer will be called if avClientRequestServerAbility
 *                             is called in client side to request server ability
 * \param change_password_request [in] The function pointer will be called if avClientRequestChangeServerPassword
 *                                     is called in client side to request server change password
 *
 */
typedef struct AVServStartInConfig {
    unsigned int cb;
    unsigned int iotc_session_id;
    unsigned char iotc_channel_id;
    unsigned int timeout_sec;
    unsigned int server_type;
    int resend;
    AvSecurityMode security_mode;
    avPasswordAuthFn password_auth;
    avTokenAuthFn token_auth;
    avTokenRequestFn token_request;
    avTokenDeleteFn token_delete;
    avIdentityArrayRequestFn identity_array_request;
    avAbilityRequestFn ability_request;
    avChangePasswordRequestFn change_password_request;
} AVServStartInConfig;


typedef const AVServStartInConfig * LPCAVSERV_START_IN_CONFIG;


/**
 * \details The output configuration of avServStartEx
 *
 * \param cb                   [in] The check byte of this structure.
 * \param resend               [out] Resend mechanism is enable or not
 * \param two_way_streaming    [out] two way audio/video streaming mechanism is enabling or not
 *                                   1: enabled, 0: disabled
 * \param auth_type            [out] The connection is authenticated with password or token
 * \param account_or_identity  [out] The client identity or account name
 */

typedef struct AVServStartOutConfig {
    unsigned int cb;
    int resend;
    int two_way_streaming;
    AvAuthType auth_type;
    char account_or_identity[256];
} AVServStartOutConfig;

typedef AVServStartOutConfig * LPAVSERV_START_OUT_CONFIG;
/**
 * \details The input configuration of avClientStartEx
 *
 * \param cb                  [in] The check byte of this structur.
 * \param iotc_session_id     [in] The session ID of the IOTC session to create AV server
 * \param iotc_channel_id     [in] The channel ID of the channel to create AV server

 * \param timeout_sec         [in] The timeout for this function in unit of second.
 *                                 Specify it as 0 will make AV server start process wait
 *                                 until an AV client connects successfully.
 * \param account_or_identity [in] The predefined view account
 * \param password_or_token   [in] The predefined view password, the maximum length of token is 1024
 * \param resend              [in] Enabling the Resend mechanism or not
 * \param security_mode       [in] Enabling the DTLS mechanism  or not
 * \param auth_type           [in] Choose the authentication mode for av connection
 */
typedef struct AVClientStartInConfig {
    unsigned int cb;
    unsigned int iotc_session_id;
    unsigned char iotc_channel_id;
    unsigned int timeout_sec;
    const char* account_or_identity;
    const char* password_or_token;
    int resend;
    AvSecurityMode security_mode;
    AvAuthType auth_type;
} AVClientStartInConfig;


typedef const AVClientStartInConfig * LPCAVCLIENT_START_IN_CONFIG;
/**
 * \details The output configuration of avClientStartEx
 *
 * \param cb                [in] The check byte of this structure.
 * \param server_type       [out] The user-defined service type set when an AV server
 *                                starts. Can be NULL.
 * \param resend            [out] Resend mechanism is enabling or not
 * \param two_way_streaming [out] two way audio/video streaming mechanism is enabling or not
*/
typedef struct AVClientStartOutConfig {
    unsigned int cb;
    unsigned int server_type;
    int resend;
    int two_way_streaming;
} AVClientStartOutConfig;


typedef AVClientStartOutConfig * LPAVCLIENT_START_OUT_CONFIG;

/* ============================================================================
 * Function Declaration
 * ============================================================================
 */

/**
 * \brief Get the version of AV module
 *
 * \details This function returns the version of AV module.
 *
 * \return The version of AV module from high byte to low byte, for example,
 *			0x01020304 means the version is 1.2.3.4
 *
 * \see IOTC_Get_Version(), RDT_GetRDTApiVer()
 */
AVAPI_API int avGetAVApiVer(void);

/**
 * \brief Reset the Video or Audio buffer of AV server
 *
 * \details This function returns AV_ER_NoERROR if no error
 *
 * \param avIndex [in] The target avIndex
 * \param eTarget [in] Reset type could be RESET_VDEIO, RESET_AUDIO, RESET_ALL
 * \param Timeout_ms [in] The timeout value in ms to execute this function, set value 0 to wait forever
 *
 * \return This function returns AV_ER_NoERROR if no error
 * \return Error code if return value < 0
 *          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5000 millisecond
 *
 */
AVAPI_API int avServResetBuffer(int avIndex, AV_RESET_TARGET eTarget, unsigned int Timeout_ms);

/**
 * \brief Initialize AV module
 *
 * \details This function is used by AV servers or AV clients to initialize AV
 *			module and shall be called before any AV module related function
 *			is invoked.
 *
 * \param nMaxChannelNum [in] The max number of AV channels. If it is
 *			specified less than 1, AV will set max number of AV channels as 1.
 *
 * \return The actual maximum number of AV channels to be set.
 * \return Error code if return value < 0
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *
 */
AVAPI_API int avInitialize(int nMaxChannelNum);

/**
 * \brief Deinitialize AV module
 *
 * \details This function will deinitialize AV module.
 *
 * \return #AV_ER_NoERROR if deinitialize successfully
 * \return Error code if return value < 0
 *			- #AV_ER_NOT_INITIALIZED the AV module is not initialized yet
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) AV module shall be deinitialized before IOTC module is
 *				deinitialized.
 */
AVAPI_API int avDeInitialize(void);

/**
 * \brief Start an AV server
 *
 * \deprecated since 3.1.9.0
 *
 * \details Start an AV server with predefined view account and password.
 *			Any AV client wanting to connect with this AV server shall
 *			provide matched view account and password.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to start AV server
 * \param cszViewAccount [in] The predefined view account
 * \param cszViewPassword [in] The predefined view password
 * \param nTimeout [in] The timeout for this function in unit of second
 *						Specify it as 0 will make AV server start process wait
 *						until an AV client connects successfully.
 * \param nServType [in] The user-defined service type. An AV client will get
 *						this value when it invokes avClientStart() successfully.
 * \param nIOTCChannelID [in] The channel ID of the channel to start AV server
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
                and IOTC channel ID.
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_INVALID_SID The IOTC session ID is not valid
 *			- #AV_ER_SERVER_EXIT Users stop this function with avServExit() in another thread
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				AV start is performed completely
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *				please check if the IOTC channel is used correctly
 *
 * \see avServStart2(), avServExit(), avServStop()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *			  (3) The IOTC channel of specified channel ID will be turned on automatically
 *				  by avServStart()
 *            (4) This API has been deprecated and might be removed in the next version, please replace it with avServStart2
 *
 */
AVAPI_API_DEPRECATED int avServStart(int nIOTCSessionID, const char *cszViewAccount, const char *cszViewPassword, unsigned int nTimeout, unsigned int nServType, unsigned char nIOTCChannelID);

/**
 * \brief Start an AV server
 *
 * \details Start an AV server with user defined authentication function.
 *			Any AV client wanting to connect with this AV server shall
 *			pass the authentication with view account and password.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to create AV server
 * \param pfxAuthFn [in] The function pointer to an authentication function
 * \param nTimeout [in] The timeout for this function in unit of second.
 *						Specify it as 0 will make AV server start process wait
 *						until an AV client connects successfully.
 * \param nServType [in] The user-defined service type. An AV client will get
 *						this value when it invokes avClientStart() successfully.
 * \param nIOTCChannelID [in] The channel ID of the channel to create AV server
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
                and IOTC channel ID.
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_INVALID_SID The IOTC session ID is not valid
 *			- #AV_ER_SERVER_EXIT Users stop this function with avServExit() in another thread
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				AV start is performed completely
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *				please check if the IOTC channel is used correctly
 *
 * \see avServStart(), avServExit(), avServStop()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *			  (3) The IOTC channel of specified channel ID will be turned on automatically
 *				  by avServStart2()
 *
 */
AVAPI_API int avServStart2(int nIOTCSessionID, authFn pfxAuthFn, unsigned int nTimeout, unsigned int nServType, unsigned char nIOTCChannelID);

/**
 * \brief Start an AV server
 *
 * \details Start an AV re-send supported server with user defined authentication function.
 *			Any AV client wanting to connect with this AV server shall
 *			pass the authentication with view account and password. Whether the re-send mechanism
 *          is enabled or not depends on AV client settings and will set the result into
 *          pnResend parameter.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to create AV server
 * \param pfxAuthFn [in] The function pointer to an authentication function
 * \param nTimeout [in] The timeout for this function in unit of second.
 *						Specify it as 0 will make AV server start process wait
 *						until an AV client connects successfully.
 * \param nServType [in] The user-defined service type. An AV client will get
 *                       this value when it invokes avClientStart() successfully.
 * \param nIOTCChannelID [in] The channel ID of the channel to create AV server
 *
 * \param pnResend [out] Set the re-send is enabled or not.
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
 *               and IOTC channel ID.
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_INVALID_SID The IOTC session ID is not valid
 *			- #AV_ER_SERVER_EXIT Users stop this function with avServExit() in another thread
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				AV start is performed completely
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *				please check if the IOTC channel is used correctly
 *
 * \see avServStart(), avServStart2(), avServExit(), avServStop()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *			  (3) The IOTC channel of specified channel ID will be turned on automatically
 *			 	  by avServStart3()
 *
 */
AVAPI_API int avServStart3(int nIOTCSessionID, authFn pfxAuthFn, unsigned int nTimeout, unsigned int nServType, unsigned char nIOTCChannelID, int *pnResend);

/**
 * \brief Used by an AV server exit avServStart() or avServStart2() process
 *
 * \details Since avServStart() and avServStart2() are block processes and
 *			that means the caller has to wait for AV start or specified timeout
 *			expires	before these two functions return. In some cases,
 *			caller may want	to exit AV start process immediately by this
 *			function in another thread.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to exit AV start process
 * \param nIOTCChannelID [in] The channel ID of the channel to exit AV start process
 *
 */
AVAPI_API void avServExit(int nIOTCSessionID, unsigned char nIOTCChannelID);

/**
 * \brief Stop an AV server
 *
 * \details An AV server stop AV channel by this function if this channel is
 *			no longer required.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be stopped
 *
 * \attention (1) This API is a blocking function.<br><br>
 */
AVAPI_API void avServStop(int nAVChannelID);

/**
 * \brief Set re-send buffer size.
 *
 * \details Use this API to set the re-send buffer size if re-send mechanism is enabled.
 *          Default re-send buffer size is 256KB and recommend size is 1 second data.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be set.
 * \param nSize [in] The size of re-send buffer, in unit of kilo-byte.
 *
 */
AVAPI_API void avServSetResendSize(int nAVChannelID, unsigned int nSize);

/**
* \brief Get re-send buffer size.
*
* \details Use this API to get the re-send buffer size if re-send mechanism is enabled.
*
* \param nAVChannelID [in] The channel ID of the AV channel to be set.
* \param pnSize [out] The size of re-send buffer, in unit of kilo-byte.
*
* \return #AV_ER_NoERROR if sending successfully
* \return Error code if return value < 0
*			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
*			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
*/
AVAPI_API int avServGetResendSize(int avIndex, unsigned int *pnSize);

/**
* \brief Get frame count of re-send buffer.
*
* \details Use this API to get the frame count of re-send buffer if re-send mechanism is enabled.
*
* \param nAVChannelID [in] The channel ID of the AV channel to be set.
* \param pnCount [out] The frame count of re-send buffer.
*
* \return #AV_ER_NoERROR if sending successfully
* \return Error code if return value < 0
*			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
*			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
*			- #AV_ER_NOT_SUPPORT New re-send protocal not support this function
*/
AVAPI_API int avServGetResendFrmCount(int avIndex, unsigned int *pnCount);

/**
 * \brief An AV server sends frame data to an AV client
 *
 * \details An AV server uses this function to send frame data to AV client
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be sent
 * \param cabFrameData [in] The frame data to be sent
 * \param nFrameDataSize [in] The size of the frame data
 * \param cabFrameInfo [in] The video frame information to be sent
 * \param nFrameInfoSize [in] The size of the video frame information
 *
 * \return #AV_ER_NoERROR if sending successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *			- #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to send frame data
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *			- #AV_ER_EXCEED_MAX_SIZE The frame data and frame info to be sent exceeds
 *				currently remaining video frame buffer. The max size of video frame
 *				buffer is determined by avServSetResendSize()
 *			- #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_DASA_CLEAN_BUFFER This error returned when DASA is enabled, means this AV channel need to do clean buffer
 *
 * \see avSendAudioData()
 *
 */
AVAPI_API int avSendFrameData(int nAVChannelID, const char *cabFrameData, int nFrameDataSize,
                              const void *cabFrameInfo, int nFrameInfoSize);

/**
 * \brief An AV server sends audio data to an AV client
 *
 * \details An AV server uses this function to send audio data to AV client
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be sent
 * \param cabAudioData [in] The audio data to be sent
 * \param nAudioDataSize [in] The size of the audio data
 * \param cabFrameInfo [in] The audio frame information to be sent
 * \param nFrameInfoSize [in] The size of the audio frame information
 *
 * \return #AV_ER_NoERROR if sending successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *			- #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to send frame data
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *			- #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_EXCEED_MAX_SIZE The audio data and frame info to be sent exceeds
 *				#AV_MAX_AUDIO_DATA_SIZE
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 * \see avSendFrameData()
 *
 */
AVAPI_API int avSendAudioData(int nAVChannelID, const char *cabAudioData, int nAudioDataSize,
                              const void *cabFrameInfo, int nFrameInfoSize);

/**
 * \brief Set interval of sending video data in AV server
 *
 * \details An AV server how to send video data to AV client.
 *			It determined the delay time at regular interval between how many
 *			packets. The default value of nPacketNum is 1 and nDelayMs is 0,
 *			so the default behavior is to send frame packets without any delay.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be sent
 * \param nPacketNum [in] How many number of packet as a regular interval
 * \param nDelayMs [in] Delay time in unit of million-second
 *
 * \return #AV_ER_NoERROR if set successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid
 *          - #AV_ER_NOT_SUPPORT New re-send protocal not support this function
 *
 *
 */
AVAPI_API int avServSetDelayInterval(int nAVChannelID, unsigned short nPacketNum, unsigned short nDelayMs);

/**
 * \brief Start an AV client
 *
 * \details Start an AV client by providing view account and password.
 *			It shall pass the authentication of the AV server before receiving
 *			AV data.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to start AV client
 * \param cszViewAccount [in] The view account for authentication
 * \param cszViewPassword [in] The view password for authentication
 * \param nTimeout [in] The timeout for this function in unit of second
 *						Specify it as 0 will make this AV client try connection
 *						once and this process will exit immediately.
 * \param pnServType [out] The user-defined service type set when an AV server
 *						starts. Can be NULL.
 * \param nIOTCChannelID [in] The channel ID of the channel to start AV client
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
 *              and IOTC channel ID.
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #AV_ER_SERV_NO_RESPONSE The AV server has no response
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_INVALID_SID The IOTC session ID is not valid
 *			- #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				AV start is performed completely
 *			- #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
 *				to incorrect view account or password
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *				please check if the IOTC channel is used correctly
 *
 * \see avClientStop(), avClientExit()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *			  (3) The IOTC channel of specified channel ID will be turned on automatically
 *				  by avClientStart().
 *
 */
AVAPI_API int avClientStart(int nIOTCSessionID, const char *cszViewAccount, const char *cszViewPassword,
                            unsigned int nTimeout, unsigned int *pnServType, unsigned char nIOTCChannelID);

/**
 * \brief Start an AV client
 *
 * \details Start an AV re-send supported client by providing view account and password.
 *			It shall pass the authentication of the AV server before receiving
 *			AV data. Whether the re-send mechanism is enabled or not depends on AV server settings
 *          and will set the result into pnResend parameter.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to start AV client
 * \param cszViewAccount [in] The view account for authentication
 * \param cszViewPassword [in] The view password for authentication
 * \param nTimeout [in] The timeout for this function in unit of second
 *						Specify it as 0 will make this AV client try connection
 *						once and this process will exit immediately.
 * \param pnServType [out] The user-defined service type set when an AV server
 *						starts. Can be NULL.
 * \param nIOTCChannelID [in] The channel ID of the channel to start AV client
 * \param pnResend [out] The re-send is enabled or not.
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
                and IOTC channel ID.
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #AV_ER_SERV_NO_RESPONSE The AV server has no response
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_INVALID_SID The IOTC session ID is not valid
 *			- #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				AV start is performed completely
 *			- #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
 *				to incorrect view account or password
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *				please check if the IOTC channel is used correctly
 *
 * \see avClientStop(), avClientExit()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *			  (3) The IOTC channel of specified channel ID will be turned on automatically
 *				  by avClientStart2().<br><br>
 *            (4) If AV client uses avClientStart2() to enable AV re-send mechanism, it has
 *                to use avRecvFrameData2() to receive video data.
 *
 */
AVAPI_API int avClientStart2(int nIOTCSessionID, const char *cszViewAccount, const char *cszViewPassword,
                             unsigned int nTimeout, unsigned int *pnServType, unsigned char nIOTCChannelID, int *pnResend);

/**
 * \brief Used by an AV client exit avClientStart() process
 *
 * \details Since avClientStart() is a block process and
 *			that means the caller has to wait for AV start or specified timeout
 *			expires	before these two functions return. In some cases,
 *			caller may want	to exit AV start process immediately by this
 *			function in another thread.
 *
 * \param nIOTCSessionID [in] The session ID of the IOTC session to exit AV start process
 * \param nIOTCChannelID [in] The channel ID of the channel to exit AV start process
 *
 */
AVAPI_API void avClientExit(int nIOTCSessionID, unsigned char nIOTCChannelID);

/**
 * \brief Stop an AV client
 *
 * \details An AV client stop AV channel by this function if this channel is
 *			no longer required.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be stopped
 *
 * \attention (1) This API is a blocking function.<br><br>
 */
AVAPI_API void avClientStop(int nAVChannelID);

/**
 * \brief An AV client receives frame data from an AV server
 *
 * \details An AV client uses this function to receive frame data from AV server
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be received
 * \param abFrameData [out] The frame data to be received
 * \param nFrameDataMaxSize [in] The max size of the frame data
 * \param abFrameInfo [out] The video frame information to be received
 * \param nFrameInfoMaxSize [in] The max size of the video frame information
 * \param pnFrameIdx [out] The index of current receiving video frame
 *
 * \return The actual length of received result stored in abFrameData if successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_DATA_NOREADY The data is not ready for receiving yet.
 *			- #AV_ER_LOSED_THIS_FRAME The whole frame is lost during receiving
 *			- #AV_ER_BUFPARA_MAXSIZE_INSUFF The frame to be received exceeds
 *				the size of abFrameData, i.e. nFrameDataMaxSize
 *			- #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_INCOMPLETE_FRAME Some parts of a frame are lost during receiving
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 * \see avRecvAudioData()
 *
 * \attention (1) This API has been deprecated and might be removed in the next version, please replace it with avRecvFrameData2
 *
 */
AVAPI_API_DEPRECATED int avRecvFrameData(int nAVChannelID, char *abFrameData, int nFrameDataMaxSize,
                              char *abFrameInfo, int nFrameInfoMaxSize, unsigned int *pnFrameIdx);

/**
 * \brief A new version AV client receives frame data from an AV server
 *
 * \details An AV client uses this function to receive frame data from AV server
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be received
 * \param abFrameData [out] The frame data to be received
 * \param nFrameDataMaxSize [in] The max size of the frame data
 * \param pnActualFrameSize [in] The actual size of frame data to be received, maybe less than expected size
 * \param pnExpectedFrameSize [in] The size of frame data expect to be received that sent from av server
 * \param abFrameInfo [out] The video frame information to be received
 * \param nFrameInfoMaxSize [in] The max size of the video frame information
 * \param pnActualFrameInfoSize [in] The actual size of the video frame information to be received
 * \param pnFrameIdx [out] The index of current receiving video frame
 *
 * \return The actual length of received result stored in abFrameData if successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_DATA_NOREADY The data is not ready for receiving yet.
 *			- #AV_ER_LOSED_THIS_FRAME The whole frame is lost during receiving
 *			- #AV_ER_BUFPARA_MAXSIZE_INSUFF The frame to be received exceeds
 *				the size of abFrameData, i.e. nFrameDataMaxSize
 *			- #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_INCOMPLETE_FRAME Some parts of a frame are lost during receiving
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 * \see avRecvAudioData()
 *
 */
AVAPI_API int avRecvFrameData2(int nAVChannelID, char *abFrameData, int nFrameDataMaxSize, int *pnActualFrameSize,
                               int *pnExpectedFrameSize, char *abFrameInfo, int nFrameInfoMaxSize,
                               int *pnActualFrameInfoSize, unsigned int *pnFrameIdx);

/**
 * \brief An AV client receives audio data from an AV server
 *
 * \details An AV client uses this function to receive audio data from AV server
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be received
 * \param abAudioData [out] The audio data to be received
 * \param nAudioDataMaxSize [in] The max size of the audio data
 * \param abFrameInfo [out] The audio frame information to be received
 * \param nFrameInfoMaxSize [in] The max size of the audio frame information
 * \param pnFrameIdx [out] The index of current receiving audio frame
 *
 * \return The actual length of received result stored in abAudioData if successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_DATA_NOREADY The data is not ready for receiving yet.
 *			- #AV_ER_LOSED_THIS_FRAME The whole frame is lost during receiving
 *			- #AV_ER_BUFPARA_MAXSIZE_INSUFF The data and frame info to be received
 *				exceeds	the size of abAudioData and abFrameInfo, respectively.
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 * \see avRecvFrameData()
 *
 */
AVAPI_API int avRecvAudioData(int nAVChannelID, char *abAudioData, int nAudioDataMaxSize,
                              char *abFrameInfo, int nFrameInfoMaxSize, unsigned int *pnFrameIdx);

/**
 * \brief Get the frame count of audio buffer remaining in the queue
 *
 * \details An AV client uses this function to get the frame count of audio buffer
 *			that is still remaining in the receiving queue, then determine
 *			whether it is a right time to invoke avRecvAudioData().
 *			Keeping audio buffer in audio queue instead of receiving immediately
 *			can greatly improve the discontinuous audio issues. However,
 *			it is not good to receive audio in a very long time since audio
 *			buffer will overflow and audio data will be lost.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to check audio buffer
 *
 * \return The frame count of audio buffer
 */
AVAPI_API int avCheckAudioBuf(int nAVChannelID);

/**
 * \brief Set the maximum video frame buffer used in AV client
 *
 * \details AV client sets the maximum video frame buffer by this function.
 *			The size of video frame buffer will affect the streaming fluency.
 *			The default size of video frame buffer is 1MB.
 *
 * \param nMaxBufSize The maximum video frame buffer, in unit of kilo-byte
 *
 */
AVAPI_API void avClientSetMaxBufSize(unsigned int nMaxBufSize);

/**
 * \brief Clean the video buffer both in client and device, and clean the audio buffer of the client.
 *
 * \details A client with multiple device connection application should call
 *			this function to clean AV buffer while switch to another devices.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to clean buffer
 *
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 *
 * \attention (1) This API is a blocking function.<br><br>
 *
 */
AVAPI_API int avClientCleanBuf(int nAVChannelID);

/**
 * \brief Clean the video buffer(both in client and device)
 *        When new re-send protocal is using(avNewServStart & avNewClientStart), this function will clean both video and audio buffer
 *
 * \details A client with multiple device connection application should call
 *			this function to clean video buffer while switch to another devices.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to clean buffer
 *
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *
 * \attention (1) This API is a blocking function.<br><br>
 *
 */
AVAPI_API int avClientCleanVideoBuf(int nAVChannelID);

/**
* \brief Clean the local video and audio buffer of the client
*
* \details This function is used to clean the video and audio buffer that the client
*			has already received
*
* \param nAVChannelID [in] The channel ID of the AV channel to clean buffer
*
* \return Error code if return value < 0
*			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
*/
AVAPI_API int avClientCleanLocalBuf(int nAVChannelID);

/**
* \brief Clean the local video buffer of the client
*
* \details This function is used to clean the video buffer that the client
*			has already received
*
* \param nAVChannelID [in] The channel ID of the AV channel to clean buffer
*
* \return Error code if return value < 0
*			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
*           - #AV_ER_NOT_SUPPORT New re-send protocal not support this function
*/
AVAPI_API int avClientCleanLocalVideoBuf(int nAVChannelID);
/**
 * \brief Clean the local audio buffer of the client
 *
 * \details A client with multiple device connection application should call
 *			this function to clean audio buffer while switch listen to speaker.
 *
 * \param nAVChannelID [in] The channel ID of the audio channel to clean buffer
 *
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *          - #AV_ER_NOT_SUPPORT New re-send protocal not support this function
 */
AVAPI_API int avClientCleanAudioBuf(int nAVChannelID);

/**
 * \brief Send AV IO control
 *
 * \details This function is used by AV servers or AV clients to send a
 *			AV IO control.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to send IO control
 * \param nIOCtrlType [in] The type of IO control
 * \param cabIOCtrlData [in] The buffer of IO control data
 * \param nIOCtrlDataSize [in] The length of IO control data
 *
 * \return #AV_ER_NoERROR if sending successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid
 *			- #AV_ER_SENDIOCTRL_ALREADY_CALLED This AV channel is already in sending
 *				IO control process
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_SENDIOCTRL_EXIT avSendIOCtrlExit() is called before this
 *				function is returned
 *			- #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds
 *				#AV_MAX_IOCTRL_DATA_SIZE
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
 *          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 *
 * \see avRecvIOCtrl(), avSendIOCtrlExit()
 *
 * \attention This API is a blocking function and it will wait until
 *			  having acknowledgment from the receiver.
 */
AVAPI_API int avSendIOCtrl(int nAVChannelID, unsigned int nIOCtrlType, const char *cabIOCtrlData, int nIOCtrlDataSize);

/**
 * \brief Receive AV IO control
 *
 * \details This function is used by AV servers or AV clients to receive a
 *			AV IO control.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to receive IO control
 * \param pnIOCtrlType [out] The type of received IO control
 * \param abIOCtrlData [out] The buffer of received IO control data
 * \param nIOCtrlMaxDataSize [in] The max length of buffer of received IO control data
 * \param nTimeout [in] The timeout for this function in unit of million-second, give 0 means return immediately
 *
 * \return The actual length of received result stored in abIOCtrlData if
 *			receiving IO control successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid or IO control type
 *				/ data is null
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_DATA_NOREADY The IO control is not ready for receiving yet.
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				receiving IO control process is performed completely
 *			- #AV_ER_BUFPARA_MAXSIZE_INSUFF The IO control data to be received exceeds
 *				the size of abIOCtrlData, i.e. nIOCtrlMaxDataSize
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SERVER_EXIT avServStop() is called before this function is returned
 *          - #AV_ER_CLIENT_EXIT avClientStop() is called before this function is returned
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 1000 millisecond
 *
 * \see avSendIOCtrl()
 *
 */
AVAPI_API int avRecvIOCtrl(int nAVChannelID, unsigned int *pnIOCtrlType, char *abIOCtrlData, int nIOCtrlMaxDataSize, unsigned int nTimeout);

/**
 * \brief Used by an AV server or an AV client to exit sending IO control process
 *
 * \details Since avSendIOCtrl() process is a block process and that means
 *			the caller has to wait for the acknowledgment from the receipt
 *			before avSendIOCtrl() returns. In some cases, caller may want
 *			to exit sending immediately by this function in another thread.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel in sending IO control
 *
 * \return #AV_ER_NoERROR if sending successfully
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV channel ID is not valid
 */
AVAPI_API int avSendIOCtrlExit(int nAVChannelID);

/**
 * \brief Used by an AV server, in order to know the usage rate of resend buffer
 *
 * \details AV server calls this function to know the usage rate of resend buffer.
 *          It will return a float value between 0 and 1.
 *          0 means that resend buffer is empty, 1 means that resend buffer is full.
 *          The others mean usage rate.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel
 *
 * \return Usage rate of resend buffer, if return value >= 0.
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid
 */
AVAPI_API float avResendBufUsageRate(int nAVChannelID);

/**
 * \brief Used by an AV client, in order to know the usage rate of receive buffer
 *
 * \details AV client calls this function to know the usage rate of receive buffer.
 *          It will return a float value between 0 and 1.
 *          0 means that receive buffer is empty, 1 means that receive buffer is full.
 *          The others mean usage rate.
 *
 * \param nAVChannelID [in] The channel ID of the AV channel
 *
 * \return Usage rate of receive buffer, if return value >= 0.
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid
 */
AVAPI_API float avClientRecvBufUsageRate(int nAVChannelID);

/**
 * \brief Set path of log file
 *
 * \details Set the absolute path of log file
 *
 * \param path [in] The path of log file, NULL = disable Log
 *
 * \param nMaxSize [in] The maximum size of log file in Bytes, 0 = unlimit
 *
 */
AVAPI_API void AV_Set_Log_Path(char *path, int nMaxSize);

/**
 * \brief Set Attribute of log file
 *
 * \param logAttr [in] See #LogAttr
 *
 * \return AV_ER_NoERROR on success.
 * \return The value < 0
 *			- AV_ER_INVALID_ARG   Invalid input argument.
 */
AVAPI_API int AV_Set_Log_Attr(LogAttr logAttr);

/**
 * \brief Set Audio Lost Condition
 *
 * \param lostCondition [in] maximum audio frame number different using in check audio lost
 *
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid
 *          - #AV_ER_NOT_SUPPORT New re-send protocal not support this function
 *
 */
AVAPI_API int avSetAudioLostCondition(int nAVChannelID, int lostCondition);

/**
 * \brief Set Dynamic Adaptive Streaming over AVAPI (DASA) Parameter
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be enabled DASA
 * \param nEnable [in] Set 1 to enable DASA, 0 to disable DASA
 * \param nCleanBufferCondition [in] Set clean buffer condition in secs, must bigger than 3 secs and smaller than 9 secs
 * \param nCleanBufferRatio [in] Set clean buffer ratio, must bigger than 50 percent and smaller than 100 percent
 * \param nAdjustmentKeepTime [in] Set adjustment AV_DASA_LEVEL keep time, must bigger than 5 secs
 * \param nIncreaseQualityCond [in] Set accumulation network stable time for increasing AV_DASA_LEVEL, must bigger than 3 secs
 * \param nDecreaseRatio [in] Set decreasing AV_DASA_LEVEL ratio, set nDecreaseRatio smaller will trigger decreasing more offen (1~100).
 * \param nInitLevel [in] Set initial quality level
 *
 * \return #AV_ER_NoERROR if setting successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or not support resend
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI_API int avDASASetting(int nAVChannelID, int nEnable, int nCleanBufferCondition, int nCleanBufferRatio, int nAdjustmentKeepTime, int nIncreaseQualityCond, int nDecreaseRatio, AV_DASA_LEVEL nInitLevel);

/**
 * \brief Dynamic Adaptive Streaming over AVAPI Status Check
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be checked
 *
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or not support resend
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *
 * \return #AV_DASA_LEVEL
 *   AV_DASA_LEVEL_DOCLEANBUFFER  : Do Clean Buffer
 *	AV_DASA_LEVEL_QUALITY_HIGH   : Set Video Quality High
 *	AV_DASA_LEVEL_QUALITY_NORMAL : Set Video Quality Normal
 *	AV_DASA_LEVEL_QUALITY_LOW    : Set Video Quality Low
 */
AVAPI_API int avDASACheck(int nAVChannelID);

/**
 * \brief Dynamic Adaptive Streaming over AVAPI Status Reset
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be reset
 * \param nLevel [in] Set reset quality level
 *
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or not support resend
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *
 * \return #AV_ER_NoERROR if setting successfully
 */
AVAPI_API int avDASAReset(int nAVChannelID, AV_DASA_LEVEL nLevel);

/**
 * \brief Start an AV server support re-send and DTLS
 *
 * \details Start an AV re-send and DTLS supported server by providing input configuration and output configuration.
 *          Whether the re-send mechanism is enabled or not depends on AV client settings and will set
 *          the result into output configuration. It'll return Error if server enable DTLS but client don't.
 *
 * \param AVServerInConfig [in] a pointer to structure which store all input parameters
 * \param AVServerOutConfig [out] a pointer to structure which store all output parameters
 *  more detail see AVServStartInConfig and AVServStartOUTConfig
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
 *             and IOTC channel ID or the pointer of input parameter is NULL.
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #AV_ER_SERV_NO_RESPONSE The AV server has no response
 *			- #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *				this IOTC session
 *			- #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *				remote site has no response.
 *			- #AV_ER_INVALID_SID The IOTC session ID is not valid
 *			- #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *			- #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *				AV start is performed completely
 *			- #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
 *				to incorrect view account or password
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *				please check if the IOTC channel is used correctly
 *			- #AV_ER_REMOTE_NOT_SUPPORT_DTLS If client does not support DTLS
 *			- #AV_ER_DTLS_WRONG_ACC_PWD If client enable the security mode with DTLS and gives
 *				a wrong account name or password
 *			- #AV_ER_REMOTE_NOT_SUPPORT_DTLS The server fail because client not support DTLS
 *			- #AV_ER_DTLS_WRONG_PWD The server fail in authentication due to incorrect
 *				password with DTLS security mode eanbled
 *			- #AV_ER_DTLS_AUTH_FAIL The server fail in authentication due to the pfxExAuthFn
 *				return value is smaller than 0
 *
 * \see avServStart2(), avServStart3(), avServExit(), avServStop()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *			  (3) The IOTC channel of specified channel ID will be turned on automatically
 *			 	  by avNewServStart()
 *
 */

AVAPI_API int avServStartEx(LPCAVSERV_START_IN_CONFIG AVServerInConfig, LPAVSERV_START_OUT_CONFIG AVServerOutConfig);

/**
 * \brief Start an AV client
 *
 * \details Start an AV re-send and DTLS supported client by providing input configuration and output configuration.
 *            It shall pass the authentication of the AV server before receiving
 *            AV data. Whether the re-send mechanism is enabled or not depends on AV server settings
 *            and will set the result into pnResend parameter in LPAVSERV_START_OUT_CONFIG.
 *            It'll return Error if client enable DTLS but server don't.
 * \param AVClientInConfig [in] a pointer to structure which store all input parameters
 * \param AVClientOutConfig [out] a pointer to structure which store all output parameters
 *  more detail see AVClientStartInConfig and AVClientStartOUTConfig
 *
 * \return AV channel ID if return value >= 0
 * \return Error code if return value < 0
 *            - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID
                and IOTC channel ID.
 *            - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *            - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *            - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *            - #AV_ER_SERV_NO_RESPONSE The AV server has no response
 *            - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *                this IOTC session
 *            - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because
 *                remote site has no response.
 *            - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *            - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *            - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before
 *                AV start is performed completely
 *            - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
 *                to incorrect view account or password
 *            - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *            - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel,
 *                please check if the IOTC channel is used correctly
 *
 * \see avClientStop(), avClientExit()
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *            (3) The IOTC channel of specified channel ID will be turned on automatically
 *                by avClientStartEx().<br><br>
 *            (4) If AV client enable AV re-send mechanism, it has
 *                to use avRecvFrameData2() to receive video data.
 *
 */
AVAPI_API int avClientStartEx(LPCAVCLIENT_START_IN_CONFIG AVClientInConfig, LPAVCLIENT_START_OUT_CONFIG AVClientOutConfig);

/**
 * \brief Get status of the connected AV channel
 *
 * \details An AV server may use this function to get information of the connected AV channel
 *
 * \param nAVChannelID [in] The channel ID of the AV channel to be checked
 * \param psAvStatus [out] The connection info of specified AV channel
 *
 * \return #AV_ER_NoERROR if getting the information successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or the input pointer is NULL
 *          - #AV_ER_NOT_SUPPORT The AV server does not support new re-send protocal
 *          - #AV_ER_CLIENT_NOT_SUPPORT Not supported for AV client
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *
 */
AVAPI_API int avStatusCheck(int nAVChannelID, struct st_AvStatus *psAvStatus);

/**
 * \brief Get token from AV server by identity
 *
 * \details This function is used by AV client to request a token from AV server with a specific identity
 *
 * \param av_index [in] The channel ID of the AV channel to send token request
 * \param identity [in] The identity to be sent
 * \param identity_description [in] The description of identity to be sent
 * \param token [out] The buffer of the token to be received
 * \param token_buff_size [in] The max size of token buffer
 * \param status_code [out] The status code returned from AV client
 * \param timeout_sec [in] The timeout for this function in unit of second, blocks forever if 0
 *
 * \return The token length received from #avTokenRequestFn()
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG
 *          - #AV_ER_NOT_INITIALIZED
 *          - #AV_ER_BUFPARA_MAXSIZE_INSUFF The size of token buffer is smaller than the size of token sent from AV server
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The remote site has no response
 *          - #AV_ER_CLIENT_EXIT avClientStop() called in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by timeout_sec expires before
 *              request token process is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_REMOTE_NOT_SUPPORT Server does not support token request
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *
 * \see avTokenRequestFn
 *
 */
AVAPI_API int avClientRequestTokenWithIdentity(int av_index,
                                               const char *identity,
                                               const char *identity_description,
                                               char *token, unsigned int token_buff_size,
                                               int *status_code,
                                               unsigned int timeout_sec);

/**
 * \brief Delete token of AV server by identity
 *
 * \details This function is used by AV client to delete token of AV server with a specific identity
 *
 * \param av_index [in] The channel ID of the AV channel to send delete token request
 * \param identity [in] The identity to be sent
 * \param status_code [out] The status code returned from AV client
 * \param timeout_sec [in] The timeout for this function in unit of second, blocks forever if 0
 *
 * \return #AV_ER_NoERROR if delete token successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG
 *          - #AV_ER_NOT_INITIALIZED
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The remote site has no response
 *          - #AV_ER_CLIENT_EXIT avClientStop() called in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by timeout_sec expires before
 *              delete token process is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_REMOTE_NOT_SUPPORT Server does not support delete token request
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *
 * \see avTokenDeleteFn
 *
 */
AVAPI_API int avClientRequestDeleteTokenWithIdentity(int av_index,
                                            const char *identity,
                                            int *status_code,
                                            unsigned int timeout_sec);

/**
 * \brief AV Client get identity list from AV server
 *
 * \details This function is used by AV client to request identity list from AV server
 *
 * \param av_index [in] The channel ID of the AV channel to send idenetity list request
 * \param identities [out] The buffer of the identity list to be received
 * \param identity_count [out] The identity conut of identity list
 * \param status_code [out] The status code returned from AV client
 * \param timeout_sec [in] The timeout for this function in unit of second, blocks forever if 0
 *
 * \return #AV_ER_NoERROR if get identity list successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG
 *          - #AV_ER_NOT_INITIALIZED
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The remote site has no response
 *          - #AV_ER_CLIENT_EXIT avClientStop() called in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by timeout_sec expires before
 *              request identity list process is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_REMOTE_NOT_SUPPORT Server does not support identity list request
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *
 * \see avServSendIdentityArray, avIdentityArrayRequestFn
 *
 */
AVAPI_API int avClientRequestIdentityArray(int av_index,
                                           AvIdentity **identities,
                                           unsigned int *identity_count,
                                           int *status_code,
                                           unsigned int timeout_sec);

/**
 * \brief AV Client free the identity list buffer
 *
 * \details This function is used by AV client to free the memory allocated by AV module.
 *          Once the AV client use avClientRequestIdentityArray() to request identity list from AV server,
 *          AV module will allocate a memory block to store the identity list.
 *          After a success call of avClientRequestIdentityArray(), AV client must use this function
 *          to free the memory, otherwise the memory leak will occur
 *          
 * \param identities [in] The identity list buffer allocated by AV module to be free
 *
 * \return #AV_ER_NoERROR if memory free successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG identities is NULL
 *
 * \see avClientRequestIdentityArray()
 *
 */
AVAPI_API int avClientFreeIdentityArray(AvIdentity *identities);

/**
 * \brief AV Client request ability from server
 *
 * \details This function is used by AV client to request ability from AV server
 *
 * \param av_index [in] The channel ID of the AV channel to send ability request
 * \param ability [out] The buffer of the ability to be received
 * \param ability_size [out] The size of ability to be received
 * \param timeout_sec [in] The timeout for this function in unit of second, blocks forever if 0
 *
 * \return #AV_ER_NoERROR if getting the server ability successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV index is invalid or ability is NULL or ability_size is NULL
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The remote site has no response
 *          - #AV_ER_CLIENT_EXIT avClientStop() called in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by timeout_sec expires before
 *              request server ability process is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_REMOTE_NOT_SUPPORT Server does not support ability request
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *
 * \see avServSendAbility, avAbilityRequestFn
 *
 */
AVAPI_API int avClientRequestServerAbility(int av_index,
        unsigned char **ability, unsigned int *ability_size, unsigned int timeout_sec);

/**
 * \brief AV Client free the ability buffer
 *
 * \details This function is used by AV client to free the memory allocated by AV module.
 *          Once the AV client use avClientRequestServerAbility() to request ability from AV server,
 *          AV module will allocate a memory block to store the server ability.
 *          After a success call of avClientRequestServerAbility(), AV client must use this function
 *          to free the memory, otherwise the memory leak will occur
 *
 * \param ability [in] The server ability buffer allocated by AV module to be free
 *
 * \return #AV_ER_NoERROR if memory free successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG ability is NULL
 *
 * \see avClientRequestServerAbility()
 *
 */
AVAPI_API int avClientFreeServerAbility(unsigned char *ability);

/**
 * \brief AV Client request change server password
 *
 * \details This function is used by AV client to request AV server change password.
 *          If AV server change password successfully, IOTC new authkey would force apply on AV server,
 *          then AV client should use new IOTC auth key for the next IOTC connection, and use 
 *          new password for AV Connection, otherwise the connection will fail.
 *
 * \param av_index [in] The channel ID of the AV channel to send change password request
 * \param account [in] The account to be sent
 * \param old_password [in] The old password to be sent
 * \param new_password [in] The new password to be sent
 * \param new_iotc_authkey [out] The buffer of new IOTC auth key to be received
 * \param new_iotc_authkey_buff_size [in] The size of new IOTC auth key buffer
 * \param new_iotc_authkey_actual_length [out] The actual length of new IOTC auth key to be received
 * \param timeout_sec [in] The timeout for this function in unit of second, blocks forever if 0
 *
 * \return #AV_ER_NoERROR if getting the server ability successfully
 * \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV index is invalid or ability is NULL or ability_length is NULL
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes
 *              this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The remote site has no response
 *          - #AV_ER_CLIENT_EXIT avClientStop() called in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by timeout_sec expires before
 *              change password request process is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_REMOTE_NOT_SUPPORT Server does not support change password request
 *			- #AV_ER_NOT_SUPPORT Client connect without authkey does not support this function
 *
 * \attention (1) This API is a blocking function.<br><br>
 *            (2) Recommended value of timeout: > 5 secs
 *
 * \see avChangePasswordRequestFn
 *
 */
AVAPI_API int avClientRequestChangeServerPassword(
    int av_index,
    const char *account,
    const char *old_password,
    const char *new_password,
    char *new_iotc_authkey,
    unsigned int new_iotc_authkey_buff_size,
    unsigned int *new_iotc_authkey_actual_length,
    unsigned int timeout_sec);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _AVAPIs_H_ */
