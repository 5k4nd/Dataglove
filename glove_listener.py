#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
    you should:
    1) power on the glove
    2) start pd
    3) start this python. press enter inside this python.

ToDo:
    - faire un ping au démrrage du glove_listener (ou un handshake tcp ?)
        pour checker la présence du gant sur le réseau 

"""
import socket
from sys import argv
from ast import literal_eval  # pour les cast de str vers dict
from threading import Thread
from time import sleep



GLOVE_IP = "192.168.1.31"
UDP_PORT = 4210
COMPUTER_IP = socket.gethostbyname(socket.gethostname())
COMPUTER_IP = "192.168.1.35"
PD_PORT = 4211




class glove_listen(Thread):
    def __init__(self, UDP_IP, UDP_PORT):
        Thread.__init__(self)
        self.sock = socket.socket(
            socket.AF_INET,     # Internet
            socket.SOCK_DGRAM   # UDP
        )
        self.x_axe = 0
        self.y_axe = 0
        self.z_axe = 0
        self.computer_ip = UDP_IP
        self.computer_port = UDP_PORT

    def run(self):
        self.sock.bind((self.computer_ip, self.computer_port))

        while 1:
            got, addr = self.sock.recvfrom(1024)
            got = got.replace(" ", "")   # remove blanks
            got = literal_eval(got)      # cast str to dict
            # print got

            if "X" in got.keys():
                self.x_axe = float(got['X'])
                self.x_axe = 
            if "Y" in got.keys():
                self.y_axe = float(got['Y'])
            if "Z" in got.keys():
                self.z_axe = float(got['Z'])
                



def glove_send(GLOVE_IP, MSG):

    sock = socket.socket(
        socket.AF_INET,     # Internet
        socket.SOCK_DGRAM   # UDP
    )
    sock.sendto(MSG, (GLOVE_IP, UDP_PORT))


if __name__ == '__main__':
    print("entry when pure data is ready to start")
    entry = raw_input()
    glove_send(GLOVE_IP, "READY_TO_START")
    print("READY_TO_START packet sent to the glove!\n now listening")

    receiving_thread = glove_listen(COMPUTER_IP, UDP_PORT)
    receiving_thread.start()
    while 1:
        try:
            sleep(.001)
            print "X: %s            Y: %s           Z: %s" % (receiving_thread.x_axe, receiving_thread.y_axe, receiving_thread.z_axe)
        except KeyboardInterrupt:
            break

    receiving_thread._Thread__stop()

    print("THE END.")
