/** @file moal_eth_ioctl.h
 *
 * @brief This file contains definition for private IOCTL call.
 *  
 * Copyright (C) 2012, Marvell International Ltd.  
 *
 * This software file (the "File") is distributed by Marvell International 
 * Ltd. under the terms of the GNU General Public License Version 2, June 1991 
 * (the "License").  You may use, redistribute and/or modify this File in 
 * accordance with the terms and conditions of the License, a copy of which 
 * is available by writing to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
 * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE 
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about 
 * this warranty disclaimer.
 *
 */

/********************************************************
Change log:
    01/05/2012: initial version
********************************************************/

#ifndef _WOAL_ETH_PRIV_H_
#define _WOAL_ETH_PRIV_H_

/** Marvell private command identifier string */
#define CMD_MARVELL     "MRVL_CMD"

/** Private command: Version */
#define PRIV_CMD_VERSION    "version"
/** Private command: Band cfg */
#define PRIV_CMD_BANDCFG    "bandcfg"
/** Private command: Host cmd */
#define PRIV_CMD_HOSTCMD    "hostcmd"
/** Private command: Custom IE config*/
#define PRIV_CMD_CUSTOMIE   "customie"
/** Private command: HT Tx Cfg */
#define PRIV_CMD_HTTXCFG    "httxcfg"
/** Private command: HT Cap Info */
#define PRIV_CMD_HTCAPINFO  "htcapinfo"
/** Private command: Add BA para */
#define PRIV_CMD_ADDBAPARA  "addbapara"
/** Private command: Aggragation priority table */
#define PRIV_CMD_AGGRPRIOTBL    "aggrpriotbl"
/** Private command: Add BA reject cfg */
#define PRIV_CMD_ADDBAREJECT    "addbareject"
/** Private command: Delete BA */
#define PRIV_CMD_DELBA      "delba"
#define PRIV_CMD_DATARATE   "getdatarate"
#define PRIV_CMD_TXRATECFG  "txratecfg"
#define PRIV_CMD_ESUPPMODE  "esuppmode"
#define PRIV_CMD_PASSPHRASE "passphrase"
#define PRIV_CMD_DEAUTH     "deauth"
#ifdef UAP_SUPPORT
#define PRIV_CMD_AP_DEAUTH     "apdeauth"
#define PRIV_CMD_GET_STA_LIST     "getstalist"
#define PRIV_CMD_BSS_CONFIG   "bssconfig"
#endif
#if defined(WIFI_DIRECT_SUPPORT)
#if defined(STA_SUPPORT) && defined(UAP_SUPPORT)
#define PRIV_CMD_BSSROLE    "bssrole"
#endif
#endif
#ifdef STA_SUPPORT
#define PRIV_CMD_GETSCANTABLE   "getscantable"
#define PRIV_CMD_SETUSERSCAN    "setuserscan"
#endif
#define PRIV_CMD_DEEPSLEEP      "deepsleep"
#define PRIV_CMD_IPADDR         "ipaddr"
#define PRIV_CMD_WPSSESSION     "wpssession"
#define PRIV_CMD_OTPUSERDATA    "otpuserdata"
#define PRIV_CMD_COUNTRYCODE    "countrycode"
#define PRIV_CMD_WAKEUPREASON     "wakeupreason"
#ifdef STA_SUPPORT
#define PRIV_CMD_LISTENINTERVAL "listeninterval"
#endif
#ifdef DEBUG_LEVEL1
#define PRIV_CMD_DRVDBG         "drvdbg"
#endif
#define PRIV_CMD_HSCFG          "hscfg"
#define PRIV_CMD_HSSETPARA      "hssetpara"
#define PRIV_CMD_SCANCFG        "scancfg"
#define PRIV_CMD_SET_BSS_MODE   "setbssmode"
#ifdef STA_SUPPORT
#define PRIV_CMD_SET_AP         "setap"
#define PRIV_CMD_SET_POWER      "setpower"
#define PRIV_CMD_SET_ESSID      "setessid"
#define PRIV_CMD_SET_AUTH       "setauth"
#define PRIV_CMD_GET_AP         "getap"
#define PRIV_CMD_GET_POWER      "getpower"
#endif

#ifdef WIFI_DIRECT_SUPPORT
/** Private command ID to Host command */
#define	WOAL_WIFIDIRECT_HOST_CMD    (SIOCDEVPRIVATE + 1)
#endif

/** Private command ID to pass mgmt frame */
#define WOAL_MGMT_FRAME_TX          WOAL_MGMT_FRAME_TX_IOCTL

/** Private command ID to pass custom IE list */
#define WOAL_CUSTOM_IE_CFG          (SIOCDEVPRIVATE + 13)

/** Private command ID for Android ICS priv CMDs */
#define	WOAL_ANDROID_PRIV_CMD       (SIOCDEVPRIVATE + 14)

/** Private command ID to get BSS type */
#define WOAL_GET_BSS_TYPE           (SIOCDEVPRIVATE + 15)

int woal_do_ioctl(struct net_device *dev, struct ifreq *req, int cmd);

/*
 * For android private commands, fixed value of ioctl is used.
 * Internally commands are differentiated using strings.
 *
 * application needs to specify "total_len" of data for copy_from_user
 * kernel updates "used_len" during copy_to_user
 */
/** Private command structure from app */
typedef struct _android_wifi_priv_cmd
{
    /** Buffer pointer */
    char *buf;
    /** buffer updated by driver */
    int used_len;
    /** buffer sent by application */
    int total_len;
} android_wifi_priv_cmd;

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

/* Maximum size of the ESSID and NICKN strings */
#define MW_ESSID_MAX_SIZE   32

/* Modes of operation */
#define MW_MODE_AUTO    0       /* Let the driver decides */
#define MW_MODE_ADHOC   1       /* Single cell network */
#define MW_MODE_INFRA   2       /* Multi cell network, roaming, ... */
#define MW_MODE_MASTER  3       /* Synchronisation master or Access Point */
#define MW_MODE_REPEAT  4       /* Wireless Repeater (forwarder) */
#define MW_MODE_SECOND  5       /* Secondary master/repeater (backup) */
#define MW_MODE_MONITOR 6       /* Passive monitor (listen only) */
#define MW_MODE_MESH    7       /* Mesh (IEEE 802.11s) network */

#define MW_AUTH_INDEX       0x0FFF
#define MW_AUTH_FLAGS       0xF000
#define MW_AUTH_WPA_VERSION     0
#define MW_AUTH_CIPHER_PAIRWISE     1
#define MW_AUTH_CIPHER_GROUP        2
#define MW_AUTH_KEY_MGMT        3
#define MW_AUTH_TKIP_COUNTERMEASURES    4
#define MW_AUTH_DROP_UNENCRYPTED    5
#define MW_AUTH_80211_AUTH_ALG      6
#define MW_AUTH_WPA_ENABLED     7
#define MW_AUTH_RX_UNENCRYPTED_EAPOL    8
#define MW_AUTH_ROAMING_CONTROL     9
#define MW_AUTH_PRIVACY_INVOKED     10
#define MW_AUTH_CIPHER_GROUP_MGMT   11
#define MW_AUTH_MFP         12

#define MW_AUTH_CIPHER_NONE 0x00000001
#define MW_AUTH_CIPHER_WEP40    0x00000002
#define MW_AUTH_CIPHER_TKIP 0x00000004
#define MW_AUTH_CIPHER_CCMP 0x00000008
#define MW_AUTH_CIPHER_WEP104   0x00000010
#define MW_AUTH_CIPHER_AES_CMAC 0x00000020

#define MW_AUTH_ALG_OPEN_SYSTEM 0x00000001
#define MW_AUTH_ALG_SHARED_KEY  0x00000002
#define MW_AUTH_ALG_LEAP    0x00000004

/* Generic format for most parameters that fit in an int */
struct mw_param
{
    t_s32 value;                /* The value of the parameter itself */
    t_u8 fixed;                 /* Hardware should not use auto select */
    t_u8 disabled;              /* Disable the feature */
    t_u16 flags;                /* Various specifc flags (if any) */
};

/*  
 *  For all data larger than 16 octets, we need to use a
 *  pointer to memory allocated in user space.
 */
struct mw_point
{
    t_u8 *pointer;              /* Pointer to the data (in user space) */
    t_u16 length;               /* number of fields or size in bytes */
    t_u16 flags;                /* Optional params */
};

/*
 * This structure defines the payload of an ioctl, and is used 
 * below.
 */
union mwreq_data
{
    /* Config - generic */
    char name[IFNAMSIZ];

    struct mw_point essid;      /* Extended network name */
    t_u32 mode;                 /* Operation mode */
    struct mw_param power;      /* PM duration/timeout */
    struct sockaddr ap_addr;    /* Access point address */
    struct mw_param param;      /* Other small parameters */
    struct mw_point data;       /* Other large parameters */
};

 /* The structure to exchange data for ioctl */
struct mwreq
{
    union
    {
        char ifrn_name[IFNAMSIZ];       /* if name, e.g. "eth0" */
    } ifr_ifrn;

    /* Data part */
    union mwreq_data u;
};

typedef struct woal_priv_ht_cap_info
{
    t_u32 ht_cap_info_bg;
    t_u32 ht_cap_info_a;
} woal_ht_cap_info;

typedef struct woal_priv_addba
{
    t_u32 time_out;
    t_u32 tx_win_size;
    t_u32 rx_win_size;
    t_u32 tx_amsdu;
    t_u32 rx_amsdu;
} woal_addba;

/** data structure for cmd txratecfg */
typedef struct woal_priv_tx_rate_cfg
{
    /* LG rate: 0, HT rate: 1, VHT rate: 2 */
    t_u32 rate_format;
    /** Rate/MCS index (0xFF: auto) */
    t_u32 rate_index;
} woal_tx_rate_cfg;

typedef struct woal_priv_esuppmode_cfg
{
    /* RSN mode */
    t_u16 rsn_mode;
    /* Pairwise cipher */
    t_u8 pairwise_cipher;
    /* Group cipher */
    t_u8 group_cipher;
} woal_esuppmode_cfg;

mlan_status woal_set_ap_wps_p2p_ie(moal_private * priv, t_u8 * ie, size_t len);

int woal_android_priv_cmd(struct net_device *dev, struct ifreq *req);

#endif /* _WOAL_ETH_PRIV_H_ */
