/*
 *  QCData.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * The Software shall not be incorporated in any way into any work licensed
 * under the GPL version 3.0 or greater.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "QCData.h"

#define kBufferSize (4096)

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

#undef kBufferSize
