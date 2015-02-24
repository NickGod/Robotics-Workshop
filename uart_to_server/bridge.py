import sys
import serial
from struct import pack, unpack
from websocket import create_connection

if (len(sys.argv) != 3):
    sys.stderr.write("Bah! Wrong number of arguments. bridge.py takes two \
arguments. The format is:\n\n\t python bridge.py REMOTE_ADDR SERIAL_PORT\n \
\nUse ? for the SERIAL_PORT to list available serial devices\n")
    sys.exit(1)

if (sys.argv[2] == "?"):
    import serial.tools.list_ports
    print("\n".join([str(x) for x in serial.tools.list_ports.comports()]))
    sys.exit(0)

REMOTE_ADDR = sys.argv[1]
SERIAL_PORT = sys.argv[2]

print ("Connecting to {0}".format(REMOTE_ADDR))
ws = create_connection(REMOTE_ADDR)

UART = serial.Serial(port=SERIAL_PORT)

try:
    while(True):
        numstr = ws.recv()
        num = int(numstr)
        databyte = pack('B', num)
        UART.write(databyte)
        print(num)
except:
    UART.close()
    ws.close()
    raise
