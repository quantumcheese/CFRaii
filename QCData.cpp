/*
 *  QCData.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#include "QCData.h"

#define kBufferSize (4096)

BEGIN_QC_NAMESPACE

void QCData::show() const
{
#ifndef NDEBUG
	CFShow(data);
#endif
}

bool QCData::writeToFile(QCString const &filePath) const
{
	QCURL fileURL(filePath, 0); // is not directory
	CFWriteStreamRef writeStream(CFWriteStreamCreateWithFile(kCFAllocatorDefault, fileURL));
	bool result(CFWriteStreamOpen(writeStream) != 0); // convert from Boolean
	
	if (result)
	{
		CFIndex bytesWritten( CFWriteStreamWrite(writeStream, GetBytePtr(), length()) );
		result = bytesWritten == length();
	}
	
	CFWriteStreamClose(writeStream);
	QCRelease(writeStream);
	
	return result;
}

// static method
QCData QCData::dataFromFile(QCString const &filePath)
{
	QCURL fileURL(filePath, false);
	return dataFromFile(fileURL);
}

// static method
QCData QCData::dataFromFile(QCURL const &fileURL)
{
	//	return QCData(CFURLCreateData(kCFAllocatorDefault, fileURL, kCFStringEncodingUTF8, false));
	
	CFReadStreamRef readStream = CFReadStreamCreateWithFile(kCFAllocatorDefault, fileURL);
	if (CFReadStreamOpen(readStream) == 0) // treat Boolean
	{
		// couldn't read file; return empty data
		CFRelease(readStream);
		return QCData();
	}
	UInt8 bytes[kBufferSize];
	
	QCData fileData;
	CFIndex bytesRead(0);
	while ( (bytesRead = CFReadStreamRead(readStream, bytes, kBufferSize)) > 0 )
	{
		fileData.AppendBytes(bytes, bytesRead);
	}
	
	CFReadStreamClose(readStream);
	QCRelease(readStream);
	
	return fileData;
}

END_QC_NAMESPACE

#undef kBufferSize
