################################################################################
# Author: Kevin Imlay
# Date: February 10, 2021
################################################################################

import time
import serial
import random
import string
from serial.tools import list_ports
import threading
import platform

################################################################################
# "Constants"
################################################################################
SPACE_CHAR = ' '

UNIX_USB_PORT_STR = "/dev/cu.usbmodem0004401565641"
WIN_USB_PORT_STR = "COM3"
SERIAL_TIMEOUT = 0.2  # seconds
BAUD_RATE = 115200
READ_BUFFER_SIZE = 1000
WRITE_BUFFER_SIZE = 5
COM_ENCODING = 'ISO-8859-1'

################################################################################
# Functions
################################################################################

################################################################################
# Reading Thread
################################################################################
def startReadThread( connection ):
  try:
    while True:
      # get response from boards
      #board_input = conn.read( READ_BUFFER_SIZE ).decode( COM_ENCODING )[0:-1]
      board_input = conn.read( READ_BUFFER_SIZE )
      if board_input[-5:] == bytes('-end-'.encode(COM_ENCODING)):
          print("End Segment")

      to_ints = list()
      for index in range(0, len(board_input)//2, 2):
          to_ints.append(int.from_bytes(board_input[index:index+1], byteorder='little', signed=True))

      with open("samples.txt", "ab") as file:
          file.write(board_input)

      # print if not None
      if len(board_input) > 0:
        print( "<<<", to_ints, end='\n' )
        pass
  except serial.serialutil.SerialException as e:
    print( "\n\nSomething went wrong! :-(  ",
      "You may have unplugged the board.\n\n" )
    exit(1)

################################################################################
# Writing Thread
################################################################################
def startWriteThread( connections):
  try:
    while True:
      # sleep to prevent sending messages too fast
      # both for the board's receiving and to let the receiving thread print
      # before asking for input again
      time.sleep(0.1)

      # initialize/fill empty buffer of size
      send_buffer = ['\0'] * WRITE_BUFFER_SIZE

      # get input
      #print(">>> ", end='')
      user_input = input(">>> ")
      for index in range( len( user_input ) ):
        if index < WRITE_BUFFER_SIZE:
          send_buffer[index] = user_input[index]
        elif index == WRITE_BUFFER_SIZE:
          print("Truncating Message.")

      # send
      for char in send_buffer:
        conn.write( char.encode( COM_ENCODING ) )
  except serial.serialutil.SerialException as e:
    print( "\n\nSomething went wrong! :-(  ",
      "You may have unplugged the board.\n\n" )
    exit(1)

################################################################################
# Main
################################################################################
if __name__ == '__main__':
  # establish connection
  platform_str = platform.platform()
  if "mac" in platform_str:
    port_str = UNIX_USB_PORT_STR
  elif "windows" in platform_str:
    port_str = WIN_USB_PORT_STR
  print("Connecting... Make sure the board is plugged in!")
  while True:
    try:
      conn = serial.Serial( port=(port_str), baudrate=BAUD_RATE,
                  timeout=SERIAL_TIMEOUT )
      break
    except serial.serialutil.SerialException:
      pass
  print("Connected to board.")

  # split into reading and writing threads
  readThread = threading.Thread(target=startReadThread, args=(conn,))
  writeThread = threading.Thread(target=startWriteThread, args=(conn,))
  readThread.start()
  writeThread.start()
