# Author: Kevin Imlay
# Date: 3-6-21
#
# This file is to test my minimal-bounding algorithm used in the audio analysis
# module before we all have our parts together and I can test it for real in the
# system. The algoritm is as follows.
#
#  FFT buffer sizes and creator. The FFT function can only take an
#  array of length {32*2^N: 0<=N<=7, N is an Integer}, so we need a function to
#  help calculate the sizes and number of buffers needed to bound the given
#  array of samples. Additionally, the size of a single buffer cannot be smaller
#  that 32*2^3 = 256 because all of the array lengths smaller than such are not
#  sufficient to capture frequencies lower than 172 Hz. Size 256 will let us
#  analyze down to 86 Hz, which is sufficient for the application.
#
#  The general outline of how the algorithm works is as follows:
#  1. Make as many of the largest size arrays as you can without their total
#  	  sizes exceeding the size given.
#  	1.a. If the first largest size that can be added is the smallest allowed
#  	  size, then just add the next size up instead and be done.
#  2. Add the smallest array you can that makes the total size exceed the size
#  	  given.
#  3. if the last two sizes are both the smallest size, combine them. loop on
#     this until combining can't be done any more.
#  4. reverse the places of the last two arrays.
#
#  This will create a set of arrays that minimally bound the size given, and by
#  swapping the positions of the last two arrays, you guarantee that the
#  smallest array doesn't have less than the minimum number of samples, if
#  possible.
#
#  The created arrays will be added to the copy array struct along with the
#  appropriate windowing function for the amount of data in each buffer.

def minimalBoundingAlgo(size):
    # running total of sizes added
    sizeRunningTotal = 0

    # list to hold ordered sub-array sizes
    arraySizesList = list()

    #print("----- Phase 1 -----")
    # check for step 1.a.
    if size <= 32*pow(2,3):
        arraySizesList.append(32*pow(2,3))
        sizeRunningTotal = 32*pow(2,3)
        #print("Max found in phase 1")
    # 1.a failed, continue with algo at 1
    else:
        # loop until total is larger than size
        while True:
            # if smallest size, break
            if sizeRunningTotal + 32*pow(2,3) > size:
                #print("Max found in phase 1")
                break

            # add next size down
            elif sizeRunningTotal + 32*pow(2,4) > size:
                arraySizesList.append(32*pow(2,3))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,3)
                #print("Adding size", 32*pow(2,3))

            elif sizeRunningTotal + 32*pow(2,5) > size:
                arraySizesList.append(32*pow(2,4))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,4)
                #print("Adding size", 32*pow(2,4))

            elif sizeRunningTotal + 32*pow(2,6) > size:
                arraySizesList.append(32*pow(2,5))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,5)
                #print("Adding size", 32*pow(2,5))

            elif sizeRunningTotal + 32*pow(2,7) > size:
                arraySizesList.append(32*pow(2,6))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,6)
                #print("Adding size", 32*pow(2,6))

            # else, add max size
            else:
                arraySizesList.append(32*pow(2,7))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,7)
                #print("Adding size", 32*pow(2,7))

        # end while

        # step 2
        #print("----- Phase 2 -----")
        # all maxsized array that can be inserted have been, now add
        # smallest size possible to go over the size
        if sizeRunningTotal < size:
            if sizeRunningTotal + 32*pow(2,3) > size:
                arraySizesList.append(32*pow(2,3))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,3)
                #print("Adding size", 32*pow(2,3))

            elif sizeRunningTotal + 32*pow(2,4) > size:
                arraySizesList.append(32*pow(2,4))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,4)
                #print("Adding size", 32*pow(2,4))

            elif sizeRunningTotal + 32*pow(2,5) > size:
                arraySizesList.append(32*pow(2,5))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,5)
                #print("Adding size", 32*pow(2,5))

            elif sizeRunningTotal + 32*pow(2,6) > size:
                arraySizesList.append(32*pow(2,6))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,6)
                #print("Adding size", 32*pow(2,6))

            else:
                arraySizesList.append(32*pow(2,7))
                sizeRunningTotal = sizeRunningTotal +  32*pow(2,7)
                #print("Adding size", 32*pow(2,7))

        # end if
        else:
            #print("No extra sizes were needed")
            pass

        # step 4
        #print("----- Phase 3 -----")
        # combine last two sizes if they are the same
        while True:
            if len(arraySizesList) > 1 and arraySizesList[-1] == arraySizesList[-2]\
                and arraySizesList[-1] != (32*pow(2,7)):
                #print("Combining last two sizes")
                arraySizesList[-2] = arraySizesList[-2] + arraySizesList[-1]
                arraySizesList.pop(-1)
            else:
                break

        # step 3
        #print("----- Phase 4 -----")
        # swap last two indexes, if applicable
        if len(arraySizesList) >= 2:
            temp = arraySizesList[-2]
            arraySizesList[-2] = arraySizesList[-1]
            arraySizesList[-1] = temp
            #print("Last two indexes swapped")
        else:
            #print("Length is only one long, no swap")
            pass

    # end else

    # sanity check
    if sum(arraySizesList) != sizeRunningTotal:
        print("UH OH! Sanity check failed!")
        quit()

    # return result
    return arraySizesList


#===============================================================================
if __name__=="__main__":
    for inputSize in range(0, 192606):
        arrayOrder = minimalBoundingAlgo( inputSize )
        minBoundingSize = sum(arrayOrder)
        print( "\nSize:", inputSize, "Minimum bounding size is", \
            minBoundingSize, "Created with", arrayOrder, "and excess of", \
            minBoundingSize - inputSize )
