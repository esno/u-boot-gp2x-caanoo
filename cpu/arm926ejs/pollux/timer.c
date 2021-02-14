/*
 * (C) Copyright 2003
 * Texas Instruments <www.ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002-2004
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * (C) Copyright 2004
 * Philippe Robin, ARM Ltd. <philippe.robin@arm.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <arm926ejs.h>

#include <system.h>
#include <mes_timer.h>

#define TIMER_LOAD_VAL (TIMERSRCCLK/100) // defined in system.h, 100 is 100ms

static ulong timestamp;
static ulong lastdec;

/* macro to read the 32 bit timer */
#if (CONFIG_TIMER_ENABLE == 1)
#define READ_TIMER	MES_TIMER_GetTimerCounter()
#else
#define READ_TIMER 	1
#endif

int timer_init (void)
{
#if (CONFIG_TIMER_ENABLE == 1)

	MES_TIMER_SelectModule( TIMER_SYSTEM_TIMER );

	MES_TIMER_SetClockDivisorEnable( CFALSE );
	MES_TIMER_SetClockSource( 0, TIMERSELPLL );		// systimer clk use PLL0
	MES_TIMER_SetClockDivisor( 0, TIMERDIV );
	MES_TIMER_SetClockPClkMode( MES_PCLKMODE_ALWAYS );
	MES_TIMER_SetClockDivisorEnable( CTRUE );

	MES_TIMER_Stop();
	MES_TIMER_SetWatchDogEnable( CFALSE );
	MES_TIMER_SetInterruptEnableAll( CFALSE );
	MES_TIMER_ClearInterruptPendingAll();

	MES_TIMER_SetTClkDivider ( MES_TIMER_CLOCK_TCLK );
	MES_TIMER_SetTimerCounter( 0 );
	MES_TIMER_SetMatchCounter( TIMER_LOAD_VAL );

	MES_TIMER_Run();

	reset_timer_masked ();
#endif
	return 0;
}

/*
 * timer without interrupts
 */
void reset_timer (void)
{
	reset_timer_masked ();
}

ulong get_timer (ulong base)
{
	return get_timer_masked () - base;
}

void set_timer (ulong t)
{
	timestamp = t;
}

/* delay x useconds AND perserve advance timstamp value */
void udelay (unsigned long usec)
{
#if (CONFIG_TIMER_ENABLE == 1)
	ulong tmo, tmp;

	if(usec >= 1000){		// if "big" number, spread normalization to seconds //
		tmo = usec / 1000;	// start to normalize for usec to ticks per sec //
		tmo *= CFG_HZ;		// find number of "ticks" to wait to achieve target //
		tmo /= 1000;		// finish normalize. //
	}else{				// else small number, don't kill it prior to HZ multiply //
		tmo = usec * CFG_HZ;
		tmo /= (1000*1000);
	}

	tmp = get_timer (0);		// get current timestamp //
	if( (tmo + tmp + 1) < tmp )	// if setting this fordward will roll time stamp //
		reset_timer_masked ();	// reset "advancing" timestamp to 0, set lastdec value //
	else
		tmo += tmp;		// else, set advancing stamp wake up time //

	while (get_timer_masked () < tmo)// loop till event //
	{
		//*NOP*/;
	}
#else
	unsigned long i, k;
	for(k=0; k<usec; k++);
#endif
	
	return;
}

void reset_timer_masked (void)
{
	/* reset time */
	lastdec = READ_TIMER;  		/* capure current decrementer value time */
	timestamp = 0;	       /* start "advancing" time stamp from 0 */
}

ulong get_timer_masked (void)
{
	ulong now = READ_TIMER;		/* current tick value */

	if (lastdec < now) 		/* normal mode (non roll) */
	{		
		timestamp += now - lastdec; /* move stamp fordward with absoulte diff ticks */
	} 
	else				/* we have overflow of the count down timer */
	{
		/* nts = ts + ld + (TLV - now)
		 * ts=old stamp, ld=time that passed before passing through -1
		 * (TLV-now) amount of time after passing though -1
		 * nts = new "advancing time stamp"...it could also roll and cause problems.
		 */
		timestamp += now + TIMER_LOAD_VAL - lastdec;
	}
	lastdec = now;

	return timestamp;
}

/* waits specified delay value and resets timestamp */
void udelay_masked (unsigned long usec)
{
	ulong tmo;
	ulong endtime;
	signed long diff;

	if (usec >= 1000) {		/* if "big" number, spread normalization to seconds */
		tmo = usec / 1000;	/* start to normalize for usec to ticks per sec */
		tmo *= CFG_HZ;		/* find number of "ticks" to wait to achieve target */
		tmo /= 1000;		/* finish normalize. */
	} else {			/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CFG_HZ;
		tmo /= (1000*1000);
	}

	endtime = get_timer_masked () + tmo;

	do {
		ulong now = get_timer_masked ();
		diff = endtime - now;
	} while (diff >= 0);
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk (void)
{
	ulong tbclk;
	
	tbclk = CFG_HZ;
	return tbclk;
}
