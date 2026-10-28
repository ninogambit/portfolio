import pyautogui
import serial

ser = serial.Serial()
ser.port = '/dev/cu.usbmodem12401'
ser.baudrate = 115200

ser.open()
ser.reset_input_buffer()

def serial_readline(obj):
 data = obj.readline()
 return data.decode("utf-8").strip()

while True:
 data_str = serial_readline(ser)

 # Mute / Ummute
 if str(data_str) == "HalfCircle":
   pyautogui.press('m')
 
 # Play /Pause video
 if str(data_str) == "AnnGes":
   pyautogui.press('k')

 # Change video
 #if str(data_str) == "Pan":
 # pyautogui.hotkey('shift', 'n')
