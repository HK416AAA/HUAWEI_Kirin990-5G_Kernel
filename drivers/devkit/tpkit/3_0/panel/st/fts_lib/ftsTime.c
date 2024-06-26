/*
  *
  **************************************************************************
  **                        STMicroelectronics				**
  **************************************************************************
  **                        marco.cali@st.com				**
  **************************************************************************
  *                                                                        *
  *                  FTS Utility for mesuring/handling the time		  *
  *                                                                        *
  **************************************************************************
  **************************************************************************
  *
  */

/*!
  * \file ftsTime.c
  * \brief Contains all functions to handle and measure the time in the driver
  */

#include "ftsTime.h"


#include <linux/errno.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <stdarg.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/ctype.h>

#define ELAPSED_MILLISECOND_TIME	1000
#define ELAPSED_MILLISECOND	1000000
#define ELAPSED_NANOSECOND	1000000000
/**
  * Take the starting time and save it in a StopWatch variable
  * @param w pointer of a StopWatch struct
  */
void startStopWatch(StopWatch *w)
{
	if(w){
		ktime_get_real_ts64(&w->start);
	}
	return;
}

/**
  * Take the stop time and save it in a StopWatch variable
  * @param w pointer of a StopWatch struct
  */
void stopStopWatch(StopWatch *w)
{
	if(w){
		ktime_get_real_ts64(&w->end);
	}
	return;
}

/**
  * Compute the amount of time spent from when the startStopWatch and then
  * the stopStopWatch were called on the StopWatch variable
  * @param w pointer of a StopWatch struct
  * @return amount of time in ms (the return value is meaningless
  * if the startStopWatch and stopStopWatch were not called before)
  */
int elapsedMillisecond(const StopWatch *w)
{
	int result = 0;
	if(w){
		result = ((w->end.tv_sec - w->start.tv_sec) *
				ELAPSED_MILLISECOND_TIME) + (w->end.tv_nsec -
				w->start.tv_nsec)  / ELAPSED_MILLISECOND;
	}
	return result;
}

/**
  * Compute the amount of time spent from when the startStopWatch and
  * then the stopStopWatch were called on the StopWatch variable
  * @param w pointer of a StopWatch struct
  * @return amount of time in ns (the return value is meaningless
  * if the startStopWatch and stopStopWatch were not called before)
  */
int elapsedNanosecond(const StopWatch *w)
{
	int result = 0;
	if(w){
		result = ((w->end.tv_sec - w->start.tv_sec) *
			ELAPSED_NANOSECOND) + (w->end.tv_nsec -
			w->start.tv_nsec);
	}
	return result;
}
