
#ifndef DATAMETEOABSTRACT_H
#define DATAMETEOABSTRACT_H

#include <iostream>
#include "DataDefines.h"
#include "Therm.h"

#define isleapyear(y) ((((y)%4==0) && ((y)%100!=0))||((y)%400==0))

//--------------------------------------------------------
enum FileDataType 
{
	DATATYPE_NONE,
	DATATYPE_CANCELLED,
	DATATYPE_GRIB,
};

//--------------------------------------------------------
class DataRecordAbstract
{
	public:
		DataRecordAbstract () {}
		virtual ~DataRecordAbstract () {}

        virtual time_t getRecordRefDate () const = 0;
        virtual time_t getRecordCurrentDate () const = 0;

		/** Return a timestamp in the UTC timezone.
		*/
		static time_t UTC_mktime (int year,int month,int day,int hour,int min,int sec);
		
		/** Compute a dew point with the Magnus-Tetens formula.
		*/
		static double dewpointMagnusTetens (double tempK, double humidRel);
		
		/** Compute a dew point with the Hardy formula.
			Hardy B., Thunder Scientific Corporation, Albuquerque, NM, USA 
			The proceedings of the Third international Symposium on Humidity & Moisture, 
			Teddington, London, England, April 1998.
		*/
		static double dewpointHardy (double tempK, double humidRel);
		
		/** Compute the mean geopotential altitude in meters
		*/
		static double computeGeopotentialAltitude (
							double Phpa  // pressure (hPa)
						);
};

//--------------------------------------------------------
class DataReaderAbstract
{
	public:
		DataReaderAbstract () {}
		virtual ~DataReaderAbstract () {}
		
		virtual FileDataType getReaderFileDataType () = 0;
};

#endif
