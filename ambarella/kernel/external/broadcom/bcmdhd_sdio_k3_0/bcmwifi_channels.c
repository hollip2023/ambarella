/*
 * Misc utility routines used by kernel or app-level.
 * Contents are wifi-specific, used by any kernel or app-level
 * software that might want wifi things as it grows.
 *
 * $Copyright Open Broadcom Corporation$
 * $Id: bcmwifi_channels.c 309193 2012-01-19 00:03:57Z $
 */

#include <bcm_cfg.h>
#include <typedefs.h>
#include <bcmutils.h>

#ifdef BCMDRIVER
#include <osl.h>
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define tolower(c) (bcm_isupper((c)) ? ((c) + 'a' - 'A') : (c))
#else
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef ASSERT
#define ASSERT(exp)
#endif
#endif 

#ifdef _bcmwifi_c_

#include <bcmwifi.h>
#else
#include <bcmwifi_channels.h>
#endif

#if defined(WIN32) && (defined(BCMDLL) || defined(WLMDLL))
#include <bcmstdlib.h> 	
#endif

#ifndef D11AC_IOTYPES

char *
wf_chspec_ntoa_ex(chanspec_t chspec, char *buf)
{
	if (wf_chspec_ntoa(chspec, buf) == NULL)
		snprintf(buf, CHANSPEC_STR_LEN, "invalid 0x%04x", chspec);
	return buf;
}


char *
wf_chspec_ntoa(chanspec_t chspec, char *buf)
{
	const char *band, *bw, *sb;
	uint channel;

	band = "";
	bw = "";
	sb = "";
	channel = CHSPEC_CHANNEL(chspec);
	
	if ((CHSPEC_IS2G(chspec) && channel > CH_MAX_2G_CHANNEL) ||
	    (CHSPEC_IS5G(chspec) && channel <= CH_MAX_2G_CHANNEL))
		band = (CHSPEC_IS2G(chspec)) ? "b" : "a";
	if (CHSPEC_IS40(chspec)) {
		if (CHSPEC_SB_UPPER(chspec)) {
			sb = "u";
			channel += CH_10MHZ_APART;
		} else {
			sb = "l";
			channel -= CH_10MHZ_APART;
		}
	} else if (CHSPEC_IS10(chspec)) {
		bw = "n";
	}

	
	snprintf(buf, 6, "%d%s%s%s", channel, band, bw, sb);
	return (buf);
}


chanspec_t
wf_chspec_aton(const char *a)
{
	char *endp = NULL;
	uint channel, band, bw, ctl_sb;
	char c;

	channel = strtoul(a, &endp, 10);

	
	if (endp == a)
		return 0;

	if (channel > MAXCHANNEL)
		return 0;

	band = ((channel <= CH_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);
	bw = WL_CHANSPEC_BW_20;
	ctl_sb = WL_CHANSPEC_CTL_SB_NONE;

	a = endp;

	c = tolower(a[0]);
	if (c == '\0')
		goto done;

	
	if (c == 'a' || c == 'b') {
		band = (c == 'a') ? WL_CHANSPEC_BAND_5G : WL_CHANSPEC_BAND_2G;
		a++;
		c = tolower(a[0]);
		if (c == '\0')
			goto done;
	}

	
	if (c == 'n') {
		bw = WL_CHANSPEC_BW_10;
	} else if (c == 'l') {
		bw = WL_CHANSPEC_BW_40;
		ctl_sb = WL_CHANSPEC_CTL_SB_LOWER;
		
		if (channel <= (MAXCHANNEL - CH_20MHZ_APART))
			channel += CH_10MHZ_APART;
		else
			return 0;
	} else if (c == 'u') {
		bw = WL_CHANSPEC_BW_40;
		ctl_sb = WL_CHANSPEC_CTL_SB_UPPER;
		
		if (channel > CH_20MHZ_APART)
			channel -= CH_10MHZ_APART;
		else
			return 0;
	} else {
		return 0;
	}

done:
	return (channel | band | bw | ctl_sb);
}


bool
wf_chspec_malformed(chanspec_t chanspec)
{
	
	if (!CHSPEC_IS5G(chanspec) && !CHSPEC_IS2G(chanspec))
		return TRUE;
	
	if (!CHSPEC_IS40(chanspec) && !CHSPEC_IS20(chanspec))
		return TRUE;

	
	if (CHSPEC_IS20(chanspec)) {
		if (!CHSPEC_SB_NONE(chanspec))
			return TRUE;
	} else {
		if (!CHSPEC_SB_UPPER(chanspec) && !CHSPEC_SB_LOWER(chanspec))
		return TRUE;
	}

	return FALSE;
}


uint8
wf_chspec_ctlchan(chanspec_t chspec)
{
	uint8 ctl_chan;

	
	if (CHSPEC_CTL_SB(chspec) == WL_CHANSPEC_CTL_SB_NONE) {
		return CHSPEC_CHANNEL(chspec);
	} else {
		
		ASSERT(CHSPEC_BW(chspec) == WL_CHANSPEC_BW_40);
		
		if (CHSPEC_CTL_SB(chspec) == WL_CHANSPEC_CTL_SB_UPPER) {
			
			ctl_chan = UPPER_20_SB(CHSPEC_CHANNEL(chspec));
		} else {
			ASSERT(CHSPEC_CTL_SB(chspec) == WL_CHANSPEC_CTL_SB_LOWER);
			
			ctl_chan = LOWER_20_SB(CHSPEC_CHANNEL(chspec));
		}
	}

	return ctl_chan;
}

chanspec_t
wf_chspec_ctlchspec(chanspec_t chspec)
{
	chanspec_t ctl_chspec = 0;
	uint8 channel;

	ASSERT(!wf_chspec_malformed(chspec));

	
	if (CHSPEC_CTL_SB(chspec) == WL_CHANSPEC_CTL_SB_NONE) {
		return chspec;
	} else {
		if (CHSPEC_CTL_SB(chspec) == WL_CHANSPEC_CTL_SB_UPPER) {
			channel = UPPER_20_SB(CHSPEC_CHANNEL(chspec));
		} else {
			channel = LOWER_20_SB(CHSPEC_CHANNEL(chspec));
		}
		ctl_chspec = channel | WL_CHANSPEC_BW_20 | WL_CHANSPEC_CTL_SB_NONE;
		ctl_chspec |= CHSPEC_BAND(chspec);
	}
	return ctl_chspec;
}

#else 






static const char *wf_chspec_bw_str[] =
{
	"5",
	"10",
	"20",
	"40",
	"80",
	"160",
	"80+80",
	"na"
};

static const uint8 wf_chspec_bw_mhz[] =
{5, 10, 20, 40, 80, 160, 160};

#define WF_NUM_BW \
	(sizeof(wf_chspec_bw_mhz)/sizeof(uint8))


static const uint8 wf_5g_40m_chans[] =
{38, 46, 54, 62, 102, 110, 118, 126, 134, 142, 151, 159};
#define WF_NUM_5G_40M_CHANS \
	(sizeof(wf_5g_40m_chans)/sizeof(uint8))


static const uint8 wf_5g_80m_chans[] =
{42, 58, 106, 122, 138, 155};
#define WF_NUM_5G_80M_CHANS \
	(sizeof(wf_5g_80m_chans)/sizeof(uint8))


static const uint8 wf_5g_160m_chans[] =
{50, 114};
#define WF_NUM_5G_160M_CHANS \
	(sizeof(wf_5g_160m_chans)/sizeof(uint8))



static uint
bw_chspec_to_mhz(chanspec_t chspec)
{
	uint bw;

	bw = (chspec & WL_CHANSPEC_BW_MASK) >> WL_CHANSPEC_BW_SHIFT;
	return (bw >= WF_NUM_BW ? 0 : wf_chspec_bw_mhz[bw]);
}


static uint8
center_chan_to_edge(uint bw)
{
	
	return (uint8)(((bw - 20) / 2) / 5);
}


static uint8
channel_low_edge(uint center_ch, uint bw)
{
	return (uint8)(center_ch - center_chan_to_edge(bw));
}


static int
channel_to_sb(uint center_ch, uint ctl_ch, uint bw)
{
	uint lowest = channel_low_edge(center_ch, bw);
	uint sb;

	if ((ctl_ch - lowest) % 4) {
		
		return -1;
	}

	sb = ((ctl_ch - lowest) / 4);

	
	if (sb >= (bw / 20)) {
		
		return -1;
	}

	return sb;
}


static uint8
channel_to_ctl_chan(uint center_ch, uint bw, uint sb)
{
	return (uint8)(channel_low_edge(center_ch, bw) + sb * 4);
}


static int
channel_80mhz_to_id(uint ch)
{
	uint i;
	for (i = 0; i < WF_NUM_5G_80M_CHANS; i ++) {
		if (ch == wf_5g_80m_chans[i])
			return i;
	}

	return -1;
}


char *
wf_chspec_ntoa_ex(chanspec_t chspec, char *buf)
{
	if (wf_chspec_ntoa(chspec, buf) == NULL)
		snprintf(buf, CHANSPEC_STR_LEN, "invalid 0x%04x", chspec);
	return buf;
}


char *
wf_chspec_ntoa(chanspec_t chspec, char *buf)
{
	const char *band;
	uint ctl_chan;

	if (wf_chspec_malformed(chspec))
		return NULL;

	band = "";

	
	if ((CHSPEC_IS2G(chspec) && CHSPEC_CHANNEL(chspec) > CH_MAX_2G_CHANNEL) ||
	    (CHSPEC_IS5G(chspec) && CHSPEC_CHANNEL(chspec) <= CH_MAX_2G_CHANNEL))
		band = (CHSPEC_IS2G(chspec)) ? "2g" : "5g";

	
	ctl_chan = wf_chspec_ctlchan(chspec);

	
	if (CHSPEC_IS20(chspec)) {
		snprintf(buf, CHANSPEC_STR_LEN, "%s%d", band, ctl_chan);
	} else if (!CHSPEC_IS8080(chspec)) {
		const char *bw;
		const char *sb = "";

		bw = wf_chspec_bw_str[(chspec & WL_CHANSPEC_BW_MASK) >> WL_CHANSPEC_BW_SHIFT];

#ifdef CHANSPEC_NEW_40MHZ_FORMAT
		
		if (CHSPEC_IS40(chspec) && CHSPEC_IS2G(chspec)) {
			sb = CHSPEC_SB_UPPER(chspec) ? "u" : "l";
		}

		snprintf(buf, CHANSPEC_STR_LEN, "%s%d/%s%s", band, ctl_chan, bw, sb);
#else
		
		if (CHSPEC_IS40(chspec)) {
			sb = CHSPEC_SB_UPPER(chspec) ? "u" : "l";
			snprintf(buf, CHANSPEC_STR_LEN, "%s%d%s", band, ctl_chan, sb);
		} else {
			snprintf(buf, CHANSPEC_STR_LEN, "%s%d/%s", band, ctl_chan, bw);
		}
#endif 

	} else {
		
		uint chan1 = (chspec & WL_CHANSPEC_CHAN1_MASK) >> WL_CHANSPEC_CHAN1_SHIFT;
		uint chan2 = (chspec & WL_CHANSPEC_CHAN2_MASK) >> WL_CHANSPEC_CHAN2_SHIFT;

		
		chan1 = (chan1 < WF_NUM_5G_80M_CHANS) ? wf_5g_80m_chans[chan1] : 0;
		chan2 = (chan2 < WF_NUM_5G_80M_CHANS) ? wf_5g_80m_chans[chan2] : 0;

		
		snprintf(buf, CHANSPEC_STR_LEN, "%d/80+80/%d-%d", ctl_chan, chan1, chan2);
	}

	return (buf);
}

static int
read_uint(const char **p, unsigned int *num)
{
	unsigned long val;
	char *endp = NULL;

	val = strtoul(*p, &endp, 10);
	
	if (endp == *p)
		return 0;

	
	*p = endp;
	
	*num = (unsigned int)val;

	return 1;
}


chanspec_t
wf_chspec_aton(const char *a)
{
	chanspec_t chspec;
	uint chspec_ch, chspec_band, bw, chspec_bw, chspec_sb;
	uint num, ctl_ch;
	uint ch1, ch2;
	char c, sb_ul = '\0';
	int i;

	bw = 20;
	chspec_sb = 0;
	chspec_ch = ch1 = ch2 = 0;

	
	if (!read_uint(&a, &num))
		return 0;

	
	c = tolower((int)a[0]);
	if (c == 'g') {
		a ++; 

		
		if (num == 2)
			chspec_band = WL_CHANSPEC_BAND_2G;
		else if (num == 5)
			chspec_band = WL_CHANSPEC_BAND_5G;
		else
			return 0;

		
		if (!read_uint(&a, &ctl_ch))
			return 0;

		c = tolower((int)a[0]);
	}
	else {
		
		ctl_ch = num;
		chspec_band = ((ctl_ch <= CH_MAX_2G_CHANNEL) ?
		               WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);
	}

	if (c == '\0') {
		
		chspec_bw = WL_CHANSPEC_BW_20;
		goto done_read;
	}

	a ++; 

	
	if (c == 'u' || c == 'l') {
		sb_ul = c;
		chspec_bw = WL_CHANSPEC_BW_40;
		goto done_read;
	}

	
	if (c != '/')
		return 0;

	
	if (!read_uint(&a, &bw))
		return 0;

	
	if (bw == 20) {
		chspec_bw = WL_CHANSPEC_BW_20;
	} else if (bw == 40) {
		chspec_bw = WL_CHANSPEC_BW_40;
	} else if (bw == 80) {
		chspec_bw = WL_CHANSPEC_BW_80;
	} else if (bw == 160) {
		chspec_bw = WL_CHANSPEC_BW_160;
	} else {
		return 0;
	}

	

	c = tolower((int)a[0]);

	
	if (chspec_band == WL_CHANSPEC_BAND_2G && bw == 40) {
		if (c == 'u' || c == 'l') {
			a ++; 
			sb_ul = c;
			goto done_read;
		}
	}

	
	if (c == '+') {
		
		static const char *plus80 = "80/";

		
		chspec_bw = WL_CHANSPEC_BW_8080;

		a ++; 

		
		for (i = 0; i < 3; i++) {
			if (*a++ != *plus80++) {
				return 0;
			}
		}

		
		if (!read_uint(&a, &ch1))
			return 0;

		
		if (a[0] != '-')
			return 0;
		a ++; 

		
		if (!read_uint(&a, &ch2))
			return 0;
	}

done_read:
	
	while (a[0] == ' ') {
		a ++;
	}

	
	if (a[0] != '\0')
		return 0;

	

	
	if (sb_ul != '\0') {
		if (sb_ul == 'l') {
			chspec_ch = UPPER_20_SB(ctl_ch);
			chspec_sb = WL_CHANSPEC_CTL_SB_LLL;
		} else if (sb_ul == 'u') {
			chspec_ch = LOWER_20_SB(ctl_ch);
			chspec_sb = WL_CHANSPEC_CTL_SB_LLU;
		}
	}
	
	else if (chspec_bw == WL_CHANSPEC_BW_20) {
		chspec_ch = ctl_ch;
		chspec_sb = 0;
	}
	
	else if (chspec_bw != WL_CHANSPEC_BW_8080) {
		
		const uint8 *center_ch = NULL;
		int num_ch = 0;
		int sb = -1;

		if (chspec_bw == WL_CHANSPEC_BW_40) {
			center_ch = wf_5g_40m_chans;
			num_ch = WF_NUM_5G_40M_CHANS;
		} else if (chspec_bw == WL_CHANSPEC_BW_80) {
			center_ch = wf_5g_80m_chans;
			num_ch = WF_NUM_5G_80M_CHANS;
		} else if (chspec_bw == WL_CHANSPEC_BW_160) {
			center_ch = wf_5g_160m_chans;
			num_ch = WF_NUM_5G_160M_CHANS;
		} else {
			return 0;
		}

		for (i = 0; i < num_ch; i ++) {
			sb = channel_to_sb(center_ch[i], ctl_ch, bw);
			if (sb >= 0) {
				chspec_ch = center_ch[i];
				chspec_sb = sb << WL_CHANSPEC_CTL_SB_SHIFT;
				break;
			}
		}

		
		if (sb < 0) {
			return 0;
		}
	}
	
	else {
		int ch1_id = 0, ch2_id = 0;
		int sb;

		ch1_id = channel_80mhz_to_id(ch1);
		ch2_id = channel_80mhz_to_id(ch2);

		
		if (ch1 >= ch2 || ch1_id < 0 || ch2_id < 0)
			return 0;

		
		chspec_ch = (((uint16)ch1_id << WL_CHANSPEC_CHAN1_SHIFT) |
			((uint16)ch2_id << WL_CHANSPEC_CHAN2_SHIFT));

		

		
		sb = channel_to_sb(ch1, ctl_ch, bw);
		if (sb < 0) {
			
			sb = channel_to_sb(ch2, ctl_ch, bw);
			if (sb < 0) {
				
				return 0;
			}
			
			sb += 4;
		}

		chspec_sb = sb << WL_CHANSPEC_CTL_SB_SHIFT;
	}

	chspec = (chspec_ch | chspec_band | chspec_bw | chspec_sb);

	if (wf_chspec_malformed(chspec))
		return 0;

	return chspec;
}


bool
wf_chspec_malformed(chanspec_t chanspec)
{
	uint chspec_bw = CHSPEC_BW(chanspec);
	uint chspec_ch = CHSPEC_CHANNEL(chanspec);

	
	if (CHSPEC_IS2G(chanspec)) {
		
		if (chspec_bw != WL_CHANSPEC_BW_20 &&
		    chspec_bw != WL_CHANSPEC_BW_40) {
			return TRUE;
		}
	} else if (CHSPEC_IS5G(chanspec)) {
		if (chspec_bw == WL_CHANSPEC_BW_8080) {
			uint ch1_id, ch2_id;

			
			ch1_id = CHSPEC_CHAN1(chanspec);
			ch2_id = CHSPEC_CHAN2(chanspec);
			if (ch1_id >= WF_NUM_5G_80M_CHANS || ch2_id >= WF_NUM_5G_80M_CHANS)
				return TRUE;

			
			if (ch2_id <= ch1_id)
				return TRUE;
		} else if (chspec_bw == WL_CHANSPEC_BW_20 || chspec_bw == WL_CHANSPEC_BW_40 ||
		           chspec_bw == WL_CHANSPEC_BW_80 || chspec_bw == WL_CHANSPEC_BW_160) {

			if (chspec_ch > MAXCHANNEL) {
				return TRUE;
			}
		} else {
			
			return TRUE;
		}
	} else {
		
		return TRUE;
	}

	
	if (chspec_bw == WL_CHANSPEC_BW_20) {
		if (CHSPEC_CTL_SB(chanspec) != WL_CHANSPEC_CTL_SB_LLL)
			return TRUE;
	} else if (chspec_bw == WL_CHANSPEC_BW_40) {
		if (CHSPEC_CTL_SB(chanspec) > WL_CHANSPEC_CTL_SB_LLU)
			return TRUE;
	} else if (chspec_bw == WL_CHANSPEC_BW_80) {
		if (CHSPEC_CTL_SB(chanspec) > WL_CHANSPEC_CTL_SB_LUU)
			return TRUE;
	}

	return FALSE;
}


bool
wf_chspec_valid(chanspec_t chanspec)
{
	uint chspec_bw = CHSPEC_BW(chanspec);
	uint chspec_ch = CHSPEC_CHANNEL(chanspec);

	if (wf_chspec_malformed(chanspec))
		return FALSE;

	if (CHSPEC_IS2G(chanspec)) {
		
		if (chspec_bw == WL_CHANSPEC_BW_20) {
			if (chspec_ch >= 1 && chspec_ch <= 14)
				return TRUE;
		} else if (chspec_bw == WL_CHANSPEC_BW_40) {
			if (chspec_ch >= 3 && chspec_ch <= 11)
				return TRUE;
		}
	} else if (CHSPEC_IS5G(chanspec)) {
		if (chspec_bw == WL_CHANSPEC_BW_8080) {
			uint16 ch1, ch2;

			ch1 = wf_5g_80m_chans[CHSPEC_CHAN1(chanspec)];
			ch2 = wf_5g_80m_chans[CHSPEC_CHAN2(chanspec)];

			
			if (ch2 > ch1 + CH_80MHZ_APART)
				return TRUE;
		} else {
			const uint8 *center_ch;
			uint num_ch, i;

			if (chspec_bw == WL_CHANSPEC_BW_20 || chspec_bw == WL_CHANSPEC_BW_40) {
				center_ch = wf_5g_40m_chans;
				num_ch = WF_NUM_5G_40M_CHANS;
			} else if (chspec_bw == WL_CHANSPEC_BW_80) {
				center_ch = wf_5g_80m_chans;
				num_ch = WF_NUM_5G_80M_CHANS;
			} else if (chspec_bw == WL_CHANSPEC_BW_160) {
				center_ch = wf_5g_160m_chans;
				num_ch = WF_NUM_5G_160M_CHANS;
			} else {
				
				return FALSE;
			}

			
			if (chspec_bw == WL_CHANSPEC_BW_20) {
				
				for (i = 0; i < num_ch; i ++) {
					if (chspec_ch == (uint)LOWER_20_SB(center_ch[i]) ||
					    chspec_ch == (uint)UPPER_20_SB(center_ch[i]))
						break; 
				}

				if (i == num_ch) {
					
					if (chspec_ch == 165)
						i = 0;

					
					if (chspec_ch == 34 || chspec_ch == 38 ||
					    chspec_ch == 42 || chspec_ch == 46)
						i = 0;
				}
			} else {
				
				for (i = 0; i < num_ch; i ++) {
					if (chspec_ch == center_ch[i])
						break; 
				}
			}

			if (i < num_ch) {
				
				return TRUE;
			}
		}
	}

	return FALSE;
}


uint8
wf_chspec_ctlchan(chanspec_t chspec)
{
	uint center_chan;
	uint bw_mhz;
	uint sb;

	ASSERT(!wf_chspec_malformed(chspec));

	
	if (CHSPEC_IS20(chspec)) {
		return CHSPEC_CHANNEL(chspec);
	} else {
		sb = CHSPEC_CTL_SB(chspec) >> WL_CHANSPEC_CTL_SB_SHIFT;

		if (CHSPEC_IS8080(chspec)) {
			bw_mhz = 80;

			if (sb < 4) {
				center_chan = CHSPEC_CHAN1(chspec);
			}
			else {
				center_chan = CHSPEC_CHAN2(chspec);
				sb -= 4;
			}

			
			center_chan = wf_5g_80m_chans[center_chan];
		}
		else {
			bw_mhz = bw_chspec_to_mhz(chspec);
			center_chan = CHSPEC_CHANNEL(chspec) >> WL_CHANSPEC_CHAN_SHIFT;
		}

		return (channel_to_ctl_chan(center_chan, bw_mhz, sb));
	}
}


chanspec_t
wf_chspec_ctlchspec(chanspec_t chspec)
{
	chanspec_t ctl_chspec = chspec;
	uint8 ctl_chan;

	ASSERT(!wf_chspec_malformed(chspec));

	
	if (!CHSPEC_IS20(chspec)) {
		ctl_chan = wf_chspec_ctlchan(chspec);
		ctl_chspec = ctl_chan | WL_CHANSPEC_BW_20;
		ctl_chspec |= CHSPEC_BAND(chspec);
	}
	return ctl_chspec;
}


uint16
wf_channel2chspec(uint ctl_ch, uint bw)
{
	uint16 chspec;
	const uint8 *center_ch = NULL;
	int num_ch = 0;
	int sb = -1;
	int i = 0;

	chspec = ((ctl_ch <= CH_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);

	chspec |= bw;

	if (bw == WL_CHANSPEC_BW_40) {
		center_ch = wf_5g_40m_chans;
		num_ch = WF_NUM_5G_40M_CHANS;
		bw = 40;
	} else if (bw == WL_CHANSPEC_BW_80) {
		center_ch = wf_5g_80m_chans;
		num_ch = WF_NUM_5G_80M_CHANS;
		bw = 80;
	} else if (bw == WL_CHANSPEC_BW_160) {
		center_ch = wf_5g_160m_chans;
		num_ch = WF_NUM_5G_160M_CHANS;
		bw = 160;
	} else if (bw == WL_CHANSPEC_BW_20) {
		chspec |= ctl_ch;
		return chspec;
	} else {
		return 0;
	}

	for (i = 0; i < num_ch; i ++) {
		sb = channel_to_sb(center_ch[i], ctl_ch, bw);
		if (sb >= 0) {
			chspec |= center_ch[i];
			chspec |= (sb << WL_CHANSPEC_CTL_SB_SHIFT);
			break;
		}
	}

	
	if (sb < 0) {
		return 0;
	}

	return chspec;
}

#endif 


extern chanspec_t wf_chspec_primary40_chspec(chanspec_t chspec)
{
	chanspec_t chspec40 = chspec;
	uint center_chan;
	uint sb;

	ASSERT(!wf_chspec_malformed(chspec));

	if (CHSPEC_IS80(chspec)) {
		center_chan = CHSPEC_CHANNEL(chspec);
		sb = CHSPEC_CTL_SB(chspec);

		if (sb == WL_CHANSPEC_CTL_SB_UL) {
			
			sb = WL_CHANSPEC_CTL_SB_L;
			center_chan += CH_20MHZ_APART;
		} else if (sb == WL_CHANSPEC_CTL_SB_UU) {
			
			sb = WL_CHANSPEC_CTL_SB_U;
			center_chan += CH_20MHZ_APART;
		} else {
			
			
			center_chan -= CH_20MHZ_APART;
		}

		
		chspec40 = (WL_CHANSPEC_BAND_5G | WL_CHANSPEC_BW_40 |
		            sb | center_chan);
	}

	return chspec40;
}


int
wf_mhz2channel(uint freq, uint start_factor)
{
	int ch = -1;
	uint base;
	int offset;

	
	if (start_factor == 0) {
		if (freq >= 2400 && freq <= 2500)
			start_factor = WF_CHAN_FACTOR_2_4_G;
		else if (freq >= 5000 && freq <= 6000)
			start_factor = WF_CHAN_FACTOR_5_G;
	}

	if (freq == 2484 && start_factor == WF_CHAN_FACTOR_2_4_G)
		return 14;

	base = start_factor / 2;

	
	if ((freq < base) || (freq > base + 1000))
		return -1;

	offset = freq - base;
	ch = offset / 5;

	
	if (offset != (ch * 5))
		return -1;

	
	if (start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 13))
		return -1;

	return ch;
}


int
wf_channel2mhz(uint ch, uint start_factor)
{
	int freq;

	if ((start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 14)) ||
	    (ch > 200))
		freq = -1;
	else if ((start_factor == WF_CHAN_FACTOR_2_4_G) && (ch == 14))
		freq = 2484;
	else
		freq = ch * 5 + start_factor / 2;

	return freq;
}


static const struct chan_info {
	uint16	chan;	
	uint16	freq;	
} chan_info[] = {
	
		{1,	2412},
		{2,	2417},
		{3,	2422},
		{4,	2427},
		{5,	2432},
		{6,	2437},
		{7,	2442},
		{8,	2447},
		{9,	2452},
		{10,	2457},
	{11,	2462},
	{12,	2467},
	{13,	2472},
	{14,	2484},

#ifdef BAND5G

	{34,	5170},
	{38,	5190},
	{42,	5210},
	{46,	5230},


	{36,	5180},
	{40,	5200},
	{44,	5220},
	{48,	5240},
	{52,	5260},
	{56,	5280},
	{60,	5300},
	{64,	5320},


	{100,	5500},
	{104,	5520},
	{108,	5540},
	{112,	5560},
	{116,	5580},
	{120,	5600},
	{124,	5620},
	{128,	5640},
	{132,	5660},
	{136,	5680},
	{140,	5700},


	{149,	5745},
	{153,	5765},
	{157,	5785},
	{161,	5805},
	{165,	5825},


	{184,	4920},
	{188,	4940},
	{192,	4960},
	{196,	4980},
	{200,	5000},
	{204,	5020},
	{208,	5040},
	{212,	5060},
	{216,	5080}
#endif 
};


uint
wf_freq2channel(uint freq)
{
	uint i;

	for (i = 0; i < ARRAYSIZE(chan_info); i++) {
		if (chan_info[i].freq == freq)
			return (chan_info[i].chan);
	}
	return (0);
}


uint
wf_channel2freq(uint channel)
{
	uint i;

	for (i = 0; i < ARRAYSIZE(chan_info); i++)
		if (chan_info[i].chan == channel)
			return (chan_info[i].freq);
	return (0);
}
