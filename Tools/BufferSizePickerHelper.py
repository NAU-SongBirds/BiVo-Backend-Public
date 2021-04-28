# Author: Kevin Imlay
# Date: 3-4-21
# Purpose: I needed a tool to help me figure out the maximim number of seconds
#   that can be recorded with the sending rate of the VCOM being ~0.6 the speed
#   of recording. As I write this tool, I am considering "fixing" this issue by
#   finding the right combination of recording buffer length, number of sub-
#   buffers, and starting buffer for the recording and sending. Let me explain.
#   Because the sending rate is slower than the sample rate, I have to figure
#   out some way to remedy this. One possible method, albiet not the best for
#   allowing back-to-back segment to be recorded, is to send while recording and
#   cut off the sending just before the samples being sent catch up to where
#   they are being sent from. In other words, stop sending just before you would
#   be sending samples that are old and haven't been replaced with new samples.
#   I hope this is a decent explaination. If you have any questions, please
#   contact me at kgimlay@gmail.com and put "BiVo" somewhere in the subject
#   line. I'll be happy to explain.
#
#   This tool is used to solve:
#   floor( (a+tb) % c ) = floor( (d+tb) % c )
#   where   a = starting position of recording pointer
#           b = sample rate
#           c = total of buffer sizes
#           d = starting position of sending pointer
#           t is the time to solve for
#
#   This equation gives you the total length of time you can record until the
#   recording pointer catched up with the sending pointer. This will dictate
#   how long a segment can be, and consequently, how long after recording has
#   stopped until sending has caught up and you can being recording again. It
#   is best to pick a configuration that has the smallest difference of the
#   max amout of time you can record and the segment length, with the max amount
#   of you can record being greater than the segment length. An ideal
#   configuration would have a max recording length equal to the segment size
#   (plus a little bit of time for overhead or time-consuming interrupts).

import math

# Parameters:
# number of buffers in the circular buffer structure
# number of samples total in circular buffer structure
# where to start the record pointer
# where to start the send pointer
def test(numBuffers, circularBufferStructSize, recordPtrLoc, sendPtrLoc):
    # physical characteristics
    sampleRate = 22050                  # samples per second
    transferRate = 115200               # bits per second
    sampleSize = 8                      # samples are 2 bytes
    sendRateOfsamples = transferRate // sampleSize    # samples per second

    # get greatest common factor to help reduce run time
    # reduces 22050/14400 to 49/32
    # effectively scales the counter, but I'm doing it this way because I don't
    # know if longs in python reduce performance. This will help make that
    # performance hit happen later
    gcf_sampleRate_sendRate = math.gcd(sampleRate, sendRateOfsamples)
    sampleRateOptimized = sampleRate // gcf_sampleRate_sendRate
    sendRateOfSamplesOptimized = sendRateOfsamples // gcf_sampleRate_sendRate

    # loop counter to help keep track of where sending and recording is
    loopCounter = 0

    # loop until anser is found
    while True:
        # find values for this iteration
        recPtr = int(recordPtrLoc) + loopCounter    # omit recording sample rate / samlple rate
                                                    # because it would just be 1
        sendPtr = int(sendPtrLoc) + loopCounter * sendRateOfsamples/sampleRate

        # report, uncomment for debug
        '''
        print("Record:", recPtr % circularBufferStructSize, \
            "in buffer:", (recPtr%circularBufferStructSize) \
            // (circularBufferStructSize/numBuffers), \
            "Send:", int(sendPtr) % circularBufferStructSize, \
            "in buffer:", (sendPtr%circularBufferStructSize) \
            // (circularBufferStructSize/numBuffers))
        '''

        # if the record pointer has caught up with the send pointer
        # return to report
        if math.floor(recPtr % circularBufferStructSize) \
            == math.floor(int(sendPtr) % circularBufferStructSize):
            # seconds, num samples sent, num samples recorded
            return int(recPtr - recordPtrLoc)/sampleRate, \
                int(sendPtr - sendPtrLoc), \
                int(recPtr - recordPtrLoc)

        # increment counter
        loopCounter = loopCounter + 1



# Loop through some possible configurations to find the best set
if __name__=="__main__":
    # configurable characteristics
    sendPtrLoc = 0  # where to start the send pointer
                    # let this be 0 because it won't be different if you're
                    # starting to send from 0 and starting to record from 3,
                    # or starting to send from 1 and starting to record from 4,
                    # or so on. It's about the offset of the start of sending
                    # and the start of recording.

    # loop and create table over
    '''
    for numBuffers in range(1,10,1):
        for circularBufferStructSize in range (22050, int(22050*2.5), int(22050/4)):
            for recordPtrLoc in range(3, numBuffers, 1):
                print("Num Buffs:", numBuffers, "| Buff Total Size:", \
                    circularBufferStructSize, " |Rec Start:", recordPtrLoc, \
                    "/",numBuffers)

                seconds, numSent, numRec = test(numBuffers, circularBufferStructSize, \
                    (recordPtrLoc/numBuffers)*circularBufferStructSize, sendPtrLoc)

                print("Length:", round(seconds, 4), "seconds | Samples:", \
                    numRec, "| Sent:", numSent)
                print()
    '''
    numBuffers = 1
    circularBufferStructSize = 38250
    sendPtrLoc = 0
    recordPtrLoc = 7650

    print("Num Buffs:", numBuffers, "| Buff Total Size:", \
        circularBufferStructSize, " |Rec Start:", recordPtrLoc, \
        "/",numBuffers)

    seconds, numSent, numRec = test(numBuffers, circularBufferStructSize, \
        recordPtrLoc, sendPtrLoc)

    print("Length:", round(seconds, 4), "seconds | Samples:", \
        numRec, "| Sent:", numSent)
    print()
