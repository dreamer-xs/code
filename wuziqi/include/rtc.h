#ifndef	__RTC_H__
#define __RTC_H__

#define INTP 		0x10070030
#define RTCCON 		0x10070040
#define TICNT 		0x10070044
#define RTCALM 		0x10070050
#define ALMSEC 		0x10070054
#define ALMMIN 		0x10070058
#define ALMHOUR 	0x1007005c
#define ALMDAY 		0x10070060
#define ALMMON 		0x10070064
#define ALMYEAR 	0x10070068
#define BCDSEC 		0x10070070
#define BCDMIN 		0x10070074
#define BCDHOUR 	0x10070078
#define BCDDAYWEEK	0x1007007C
#define BCDDAY		0x10070080
#define BCDMON		0x10070084
#define BCDYEAR 	0x10070088
#define CURTICCNT 	0x10070090

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

int rtc_get (struct rtc_time *);
int rtc_set (struct rtc_time *);
void rtc_reset (void);

#endif
