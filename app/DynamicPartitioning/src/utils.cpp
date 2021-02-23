#include <QString>

#include <math.h>
#include <stdio.h>

QString formatBytesToSize(float bytes)
{
	/*
	// http://www.mpgh.net/forum/showthread.php?t=811707
	 float tb = 1099511627776;
	 float gb = 1073741824;
	 float mb = 1048576;
	 float kb = 1024;


	 char returnSize[256];


	 if( Bytes >= tb )
		 sprintf(returnSize, "%.2f TB", (float)Bytes/tb);
	 else if( Bytes >= gb && Bytes < tb )
		 sprintf(returnSize, "%.2f GB", (float)Bytes/gb);
	 else if( Bytes >= mb && Bytes < gb )
		 sprintf(returnSize, "%.2f MB", (float)Bytes/mb);
	 else if( Bytes >= kb && Bytes < mb )
		 sprintf(returnSize, "%.2f KB", (float)Bytes/kb);
	 else if ( Bytes < kb)
		 sprintf(returnSize, "%.2f Bytes", Bytes);
	 else
		 sprintf(returnSize, "%.2f Bytes", Bytes);


	 return returnSize;
	 */
	// https://www.mbeckler.org/blog/?p=114
	const char* suffixes[7];
	suffixes[0] = "B";
	suffixes[1] = "KB";
	suffixes[2] = "MB";
	suffixes[3] = "GB";
	suffixes[4] = "TB";
	suffixes[5] = "PB";
	suffixes[6] = "EB";
	uint s = 0; // which suffix to use
	double count = bytes;
	char buf[256];

	while (count >= 1024 && s < 7)
	{
		s++;
		count /= 1024;
	}
	if (count - floor(count) == 0.0)
		sprintf(buf, "%d %s", (int)count, suffixes[s]);
	else
		sprintf(buf, "%.1f %s", count, suffixes[s]);

	return buf;
}



