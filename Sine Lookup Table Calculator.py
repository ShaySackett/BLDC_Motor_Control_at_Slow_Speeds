## A program to approximate 'x' number of points on 1 period of sine and output them to a formatted array for an arduino lookup table

import math

# Sine takes the form y = a*sin*b(x - h) + k , we assume B, 2pi/B, is 1 so the period is a standard 2pi.


a = 127                                                                                             #amplitude = .5 * height of entire sine wave
k = 127                                                                                             #shift entire sine wave up 127 units so that entire wave is above x axis and we don't get negative y values.

num_approxs = 720                                                                               # Set number of approximations here.
characters = 150                                                                                              #Set max number of numbers in a row here
spacing = 2 * math.pi / num_approxs                                                                            #calculate the size of the discrete steps between each calculation assuming a period of 2 pi.
write_counter = 0;                                                                                          #for debugging and verification that number of elements in text file are actually being written

arduino_array = []


for i in range(num_approxs):                                                        #calculate values for lookup table
    x = spacing * i
    y = a*math.sin(x) + k                                                            #multiplying by -1 shouldnt affect anything besides flip the sine wave upside, done since this is the way i did it in excel for cosmetic reasons and im just keeping things consistent for now.
    y = int(round(y, 0))                                                                # round y to the nearest integer and 0 decimal places, and make it an int type
    arduino_array.append(y)

with open('arduino_array.txt', 'w') as writer:                                       #open a new text file, and write the open bracket
    writer.write('{')

with open('arduino_array.txt', 'a') as a_writer:                                            #open previous file, write in append mode and append values of array one by one
    length = 0
    for i in range(num_approxs):
        if length <= characters:                                                      #add new line after row_item number of elements
            a_writer.write(str(arduino_array[i]) + ',')
            length += len(str(arduino_array[i]) + ',')

        else:
            a_writer.write('\n')
            a_writer.write(str(arduino_array[i]) + ',')
            length =  len(str(arduino_array[i]) + ',')
            #write_counter += 1
    a_writer.write(str(arduino_array[0]) + '};')                                             #finish off lookup table with final value which is the same as the first value in the list, and then add curly bracket



print(arduino_array)                                                                    #confirmation of results/debug
print('# of values in calculated array:' + str(len(arduino_array)))
#print('# of values printed inside of text file:' + str(write_counter))






