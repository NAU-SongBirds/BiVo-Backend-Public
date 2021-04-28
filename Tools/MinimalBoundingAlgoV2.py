# Author: Kevin Imlay
# Date: 3-6-21
#
# This file is to test my minimal-bounding algorithm used in the audio analysis
# module before we all have our parts together and I can test it for real in the
# system. After creating my version 1, I have thought up a second version that
# may be more efficient. The algoritm is as follows.
#
# 1. Divide the size by 32*2^3 and round up to the next size in
#    {32*2^N: 0<=N<=7, N is an Integer}.
# 2. Create an array with this many elements, all of which are 32*2^3.
# 3. Loop over the array in reverse and combine equal sizes until combining
#    cannot be done any more.

def minimalBoundingAlgo(size):
    # 1. Divide the size by 32*2^3 and round up to the next size in
    #    {32*2^N: 0<=N<=7, N is an Integer}.
    minBoundingSize = size / (32*pow(2,3))
    if minBoundingSize % 1 != 0:
        minBoundingSize = minBoundingSize + 1
    minBoundingSize = int(minBoundingSize)

    # 2. Create an array with this many elements, all of which are 32*2^3.
    minBoundingList = [32*pow(2,3)] * minBoundingSize

    # 3. Loop over the array in reverse and combine equal sizes until combining
    #    cannot be done any more.
    for iteration in range(minBoundingSize*minBoundingSize):
        #print(iteration)
        for index in reversed(range(minBoundingSize)):
            # if not first index, combine with previous index if possible
            try:
                if index != 0:
                    if minBoundingList[index] == minBoundingList[index-1] \
                        and minBoundingList[index] < 32*pow(2,7):
                        # combine and set to zero for now
                        minBoundingList[index-1] = minBoundingList[index-1] * 2
                        minBoundingList.pop(index)
                        #print(minBoundingList)
            except (IndexError):
                pass

    # remove all zeros
    combinedMinBoundingList = list()
    for element in minBoundingList:
        if element != 0:
            combinedMinBoundingList.append(element)

    #print(combinedMinBoundingList)
    return combinedMinBoundingList


#===============================================================================
if __name__=="__main__":
    #arrayOrder = minimalBoundingAlgo( 255 )


    for inputSize in range(0, 20000):
        arrayOrder = minimalBoundingAlgo( inputSize )
        minBoundingSize = sum(arrayOrder)
        print( "\nSize:", inputSize, "Minimum bounding size is", \
            minBoundingSize, "Created with", arrayOrder, "and excess of", \
            minBoundingSize - inputSize )
