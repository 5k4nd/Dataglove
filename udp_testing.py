#!/usr/bin/env python3
# -*- coding: utf-8 -*-


UDP_PORT = 4210

BUFFERSIZE = 1024

import socket
from sys import argv
from ast import literal_eval  # pour les cast de str vers dict

def receive():
    sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
    sock.bind((UDP_IP, UDP_PORT))

    while True:
        got, addr = sock.recvfrom(BUFFERSIZE)
        got = got.replace(" ", "")   # remove blanks
        got = literal_eval(got)      # cast str to dict

        if "X" in got.keys():
            x_axe = float(got['X'])
        if "Y" in got.keys():
            y_axe = float(got['Y'])
        if "Z" in got.keys():
            z_axe = float(got['Z'])
            
        print "X: %s            Y: %s           Z: %s" % (x_axe, y_axe, z_axe)



def send():
    MESSAGE = "{'X':0.98, 'Y':999.98}"

    print( "UDP target IP:", UDP_IP)
    print( "UDP target port:", UDP_PORT)
    print( "message:", MESSAGE)

    sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))


if argv[1] == "send":
    # UDP_IP = "192.168.12.209"
    UDP_IP = "192.168.12.1"
    while(1):
        send()
        entry = raw_input()
        if entry=="q":
            break
elif argv[1] == "receive":
    UDP_IP = "192.168.12.1"
    receive()

