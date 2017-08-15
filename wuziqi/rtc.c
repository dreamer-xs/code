#include <config.h>
#include <rtc.h>
#include <stdio.h>

static inline unsigned int bcd2bin(u8 val)
{   
	return ((val) & 0x0f) + ((val) >> 4) * 10;
}          

static inline u8 bin2bcd (unsigned int val)
{          
	return (((val / 10) << 4) | (val % 10));
}


typedef enum {
		RTC_ENABLE,
		RTC_DISABLE
} RTC_ACCESS;


static inline void SetRTC_Access(RTC_ACCESS a)
{
	switch (a) {
		case RTC_ENABLE:
			set_one(RTCCON, 0);
			break;

		case RTC_DISABLE:
			set_zero(RTCCON, 0);
			break;
	}
}

int rtc_get (struct rtc_time *tmp)
{
	u8 sec, min, hour, mday, wday, mon, year;
	//u8 a_sec, a_min, a_hour, a_date, a_mon, a_year, a_armed;

	/* read RTC registers */
	do {
		sec  = get_nbit(BCDSEC, 0, 8);
		min  = get_nbit(BCDMIN, 0, 8);
		hour = get_nbit(BCDHOUR, 0, 8);
		mday = get_nbit(BCDDAYWEEK, 0, 8);
		wday = get_nbit(BCDDAY, 0, 8);
		mon  = get_nbit(BCDMON, 0, 8);
		year = get_nbit(BCDYEAR, 0, 8);
	} while (sec != get_nbit(BCDSEC, 0, 8));

	tmp->tm_sec  = bcd2bin(sec & 0x7F);
	tmp->tm_min  = bcd2bin(min & 0x7F);
	tmp->tm_hour = bcd2bin(hour & 0x3F);
	tmp->tm_mday = bcd2bin(mday & 0x3F);
	tmp->tm_mon  = bcd2bin(mon & 0x1F);
	tmp->tm_year = bcd2bin(year);
	tmp->tm_wday = bcd2bin(wday & 0x07);
	if (tmp->tm_year < 70)
		tmp->tm_year += 2000;
	else
		tmp->tm_year += 1900;
	tmp->tm_yday  = 0;
	tmp->tm_isdst = 0;

#if 1
	pf("Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
	       tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		   tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
#endif
	return 0;
}

int rtc_set (struct rtc_time *tmp)
{
	u8 sec, min, hour, mday, wday, mon, year;

	year = bin2bcd(tmp->tm_year % 100);
	mon  = bin2bcd(tmp->tm_mon);
	wday = bin2bcd(tmp->tm_wday);
	mday = bin2bcd(tmp->tm_mday);
	hour = bin2bcd(tmp->tm_hour);
	min  = bin2bcd(tmp->tm_min);
	sec  = bin2bcd(tmp->tm_sec);

	/* enable access to RTC registers */
	SetRTC_Access(RTC_ENABLE);

	/* write RTC registers */
	set_nbit(BCDSEC, 0, 8, sec);
	set_nbit(BCDMIN, 0, 8, min);
	set_nbit(BCDHOUR, 0, 8, hour);
	set_nbit(BCDDAYWEEK, 0, 8, mday);
	set_nbit(BCDDAY, 0, 8, wday);
	set_nbit(BCDMON, 0, 8, mon);
	set_nbit(BCDYEAR, 0, 8, year);

	/* disable access to RTC registers */
	SetRTC_Access(RTC_DISABLE);
}

void rtc_reset (void)
{

}

void rtc_tick_init(u32 sec)
{
	set_nbit(RTCCON, 4, 4, 0xf);
	set_one(RTCCON, 8);

	sec = sec > 3 ? sec : 4;
	set_val(TICNT, sec);
}

void do_rtc_tick(void)
{
	printf("rtc tick time out\n");
	set_one(INTP, 0);
}

void rtc_alarm_sec_init(u32 sec_set)
{
	u32 sec = bin2bcd(sec_set);

	set_nbit(ALMSEC, 0, 8, sec);

	set_one(RTCALM, 6);	
	set_one(RTCALM, 0);
}

void do_rtc_alarm_sec(void)
{
	struct rtc_time tmp;
	printf("rtc alarm sec time out\n");
	set_one(INTP, 1);
	rtc_get(&tmp);
}

