#ifndef _INCLUDED_SDNCAL_H
#define _INCLUDED_SDNCAL_H
/*
 * This code has been modified for use with PHP
 * by Shane Caraveo shane@caraveo.com
 * see below for more details
 *
 */

/* $selId: sdncal.h,v 2.0 1995/10/24 01:13:06 lees Exp $
 * Copyright 1993-1995, Scott E. Lee, all rights reserved.
 * Permission granted to use, copy, modify, distribute and sell so long as
 * the above copyright and this permission statement are retained in all
 * copies.  THERE IS NO WARRANTY - USE AT YOUR OWN RISK.
 */

/**************************************************************************
 *
 * This package defines a set of routines that convert calendar dates to
 * and from a serial day number (SDN).  The SDN is a serial numbering of
 * days where SDN 1 is November 25, 4714 BC in the Gregorian calendar and
 * SDN 2447893 is January 1, 1990.  This system of day numbering is
 * sometimes referred to as Julian days, but to avoid confusion with the
 * Julian calendar, it is referred to as serial day numbers here.  The term
 * Julian days is also used to mean the number of days since the beginning
 * of the current year.
 *
 * The SDN can be used as an intermediate step in converting from one
 * calendar system to another (such as Gregorian to Jewish).  It can also
 * be used for date computations such as easily comparing two dates,
 * determining the day of the week, finding the date of yesterday or
 * calculating the number of days between two dates.
 *
 * When using this software on 16 bit systems, be careful to store SDNs in
 * a long int, because it will not fit in the 16 bits that some systems
 * allocate to an int.
 *
 * For each calendar, there are two routines provided.  One converts dates
 * in that calendar to SDN and the other converts SDN to calendar dates.
 * The routines are named SdnTo<CALENDAR>() and <CALENDAR>ToSdn(), where
 * <CALENDAR> is the name of the calendar system.
 *
 * SDN values less than one are not supported.  If a conversion routine
 * returns an SDN of zero, this means that the date given is either invalid
 * or is outside the supported range for that calendar.
 *
 * At least some validity checks are performed on input dates.  For
 * example, a negative month number will result in the return of zero for
 * the SDN.  A returned SDN greater than one does not necessarily mean that
 * the input date was valid.  To determine if the date is valid, convert it
 * to SDN, and if the SDN is greater than zero, convert it back to a date
 * and compare to the original.  For example:
 *
 *    int y1, m1, d1;
 *    int y2, m2, d2;
 *    long int sdn;
 *    ...
 *    sdn = GregorianToSdn(y1, m1, d1);
 *    if (sdn > 0) {
 *        SdnToGregorian(sdn, &y2, &m2, &d2);
 *        if (y1 == y2 && m1 == m2 && d1 == d2) {
 *            ... date is valid ...
 *        }
 *    }
 *
 **************************************************************************/

#include "../phpdl.h"

/* Functions accessable to PHP */

DLEXPORT void cal_jdtogreg(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_gregtojd(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_jdtojul(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_jultojd(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_jdtojewish(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_jewishtojd(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_jdtofrench(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_frenchtojd(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_jddayofweek(INTERNAL_FUNCTION_PARAMETERS);
DLEXPORT void cal_monthname(INTERNAL_FUNCTION_PARAMETERS);

/* These functions are internal to the dll */
/* Gregorian calendar conversions. */
void SdnToGregorian(long int sdn, int *pYear, int *pMonth, int *pDay);
long int GregorianToSdn(int year, int month, int day);
extern char *MonthNameShort[13];
extern char *MonthNameLong[13];

/* Julian calendar conversions. */
void SdnToJulian(long int sdn, int *pYear, int *pMonth, int *pDay);
long int JulianToSdn(int year, int month, int day);

/* Jewish calendar conversions. */
void SdnToJewish(long int sdn, int *pYear, int *pMonth, int *pDay);
long int JewishToSdn(int year, int month, int day);
extern char *JewishMonthName[14];

/* French republic calendar conversions. */
void SdnToFrench(long int sdn, int *pYear, int *pMonth, int *pDay);
long int FrenchToSdn(int inputYear, int inputMonth, int inputDay);
extern char *FrenchMonthName[14];

/* Islamic calendar conversions. */
/* Not implemented yet. */

/* Day of week conversion.  0=Sunday, 6=Saturday */
int DayOfWeek(long int sdn);
extern char *DayNameShort[7];
extern char *DayNameLong[7];

#endif							/* _INCLUDED_SDNCAL_H */
