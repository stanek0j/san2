
#include "bufferprocessor.hpp"

using namespace CPPL;

BufferProcessor::BufferProcessor() :
    mReadBufferLen(0) // important
{

}

BufferProcessor::~BufferProcessor()
{

}

bool findCharacter(const char *buf, unsigned int len, unsigned int *index, char delimeter)
{
	for (*index = 0; *index < len; (*index)++) if(buf[*index] == delimeter) return true;
	return false;
}

ErrorCode BufferProcessor::readSome(char *buffer, unsigned int bufferSize, unsigned int *bytesRead)
{
        ErrorCode rval = fillBuffer();
        if (rval != ErrorCode::SUCCESS) return rval;

        *bytesRead = mReadBufferLen;
        return extractBuffer(buffer, bufferSize, mReadBufferLen);
}

ErrorCode BufferProcessor::readDelimeter(char *output, unsigned int outputSize, unsigned int *outputLen, char delimeter)
{            
    while(1)
    {        
        if (isTerminated()) return ErrorCode::TERMINATED;

        if (findCharacter(mReadBuffer, mReadBufferLen, outputLen, 0x0A))
        {
            // There is newline in the ReadBuffer,
            // we will copy the part till newline to line buffer
            // and then shift the remaining buffer
            (*outputLen)++;
            return extractBuffer(output, outputSize, *outputLen);
        }
        else
        {
            if (bufferFull()) 
            {
                // line is too long
                return ErrorCode::SEGMENT_TOO_LONG;
            }
            ErrorCode rval = fillBuffer();
            assert(rval != ErrorCode::FAILURE);
            if (rval != ErrorCode::SUCCESS) return rval;
            continue;
        }
    }
}

ErrorCode BufferProcessor::readLine(char *line, unsigned int lineSize)
{
    unsigned int lineLen;
    ErrorCode rval = readDelimeter(line, lineSize, &lineLen, 0x0A);
    if (rval != ErrorCode::SUCCESS) return rval;

    if (lineLen == 0) return ErrorCode::SUCCESS;

    // Check if there is an illegal character
    for (unsigned int i = 0; i < lineLen; i++)
    {
        if (line[i] >= 0 && line[i] <= 9) return ErrorCode::ILLEGAL_CHARACTER_RECEIVED;
        if (line[i] >= 11 && line[i] <= 31) return ErrorCode::ILLEGAL_CHARACTER_RECEIVED;
    }

    // Check if line ends with line feed <LF>
    // this is additional check to readLine() and is not necessary
    if (line[lineLen-1] != 0x0A) return ErrorCode::FAILURE;
    line[lineLen-1] = 0; // Terminate string
     
    return ErrorCode::SUCCESS;
}

ErrorCode BufferProcessor::extractBuffer(char *outputBuffer, unsigned int outputBufferSize, unsigned int extractCount)
{
    // This if is only for clarification purposes, this would be caught later in cppl_extract_buffer() as FAILURE.
    if (extractCount > outputBufferSize) return ErrorCode::BUFFER_TO_SMALL;

    if (cppl_extract_buffer(mReadBuffer, mReadBufferSize, &mReadBufferLen, outputBuffer, outputBufferSize, extractCount)) return ErrorCode::FAILURE;
    return ErrorCode::SUCCESS;
}

ErrorCode BufferProcessor::fillBuffer()
{
    return fillBuffer(mReadBuffer, mReadBufferSize, &mReadBufferLen);
}

ErrorCode BufferProcessor::fillBuffer(char *readBuffer, unsigned int readBufferSize, unsigned int *readBufferLen)
{
    unsigned int remainingLen = readBufferSize - *readBufferLen;
    if (remainingLen == 0) return ErrorCode::SUCCESS;
    unsigned int bytesRead;
    ErrorCode rval = read(readBuffer + *readBufferLen, remainingLen, &bytesRead);
    *readBufferLen += bytesRead;
    return rval;
}

bool BufferProcessor::bufferFull()
{
    if (mReadBufferSize == mReadBufferLen) return true;
    return false;
}

// This sends data without \0. And that is the correct way.
ErrorCode BufferProcessor::send(const char *nullTerminatedString)
{
    int len = strlen(nullTerminatedString);
    if (len == 0) return ErrorCode::SUCCESS; // nothing to send
    return send((char*)nullTerminatedString, len);
}

ErrorCode BufferProcessor::send(const std::string &data)
{
    return send(data.c_str());
}

ErrorCode BufferProcessor::sendLine(const char *nullTerminatedString)
{
    ErrorCode rval = send(nullTerminatedString);
    if (rval != ErrorCode::SUCCESS) return rval;
    return send("\x0A"); // LF
}

ErrorCode BufferProcessor::sendLine(void)
{
    return send("\x0A");
}