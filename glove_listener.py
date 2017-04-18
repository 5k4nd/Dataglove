#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
WARNING! IT'S PYTHON 3 !

    you should:
    1) power on the glove
    2) start pd
    3) start this python. press enter inside this python.

ToDo:
    - faire un ping au démrrage du glove_listener (ou un handshake tcp ?)
        pour checker la présence du gant sur le réseau 

"""
import socket

GLOVE_IP = "192.168.83.101"
#GLOVE_IP = "192.168.1.31"
#GLOVE_IP = "192.168.12.209"
#COMPUTER_IP = "192.168.1.35"
COMPUTER_IP = "192.168.83.1"


UDP_PORT = 4210
# VVV doesn't work!
#COMPUTER_IP = socket.gethostbyname(socket.getfqdn())
#COMPUTER_IP = "127.1"
MAX_PORT = 4211


from ast import literal_eval  # pour les cast de str vers dict
from sys import argv
from threading import Thread
from time import sleep
from os import system


import argparse
from pythonosc import osc_message_builder
from pythonosc import udp_client






def pd_send(MSG, PD_PORT):
    system("echo '%s;' | pdsend %s" % (MSG, PD_PORT))


def start_max_client():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=MAX_PORT)
    args = parser.parse_args()
    client = udp_client.SimpleUDPClient(args.ip, args.port)
    return client


class glove_listener(Thread):
    def __init__(self, UDP_IP, UDP_PORT):
        Thread.__init__(self)
        self.must_terminate = False
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
            """ récupération des données
            """
            if self.must_terminate:
                break

            got, addr = self.sock.recvfrom(1024)
            got = got.decode('utf-8')    # python3
            got = got.replace(" ", "")   # remove blanks
            got = literal_eval(got)      # cast str to dict
            # print got

            if "X" in got.keys():
                self.x_axe = float(got['X'])
            if "Y" in got.keys():
                self.y_axe = float(got['Y'])
            if "Z" in got.keys():
                self.z_axe = float(got['Z'])
                

            """ conversion des données
            """
            self.x_axe = int(self.x_axe)
            
            yaxe = self.y_axe
            if yaxe < -5:
                yaxe = -5
            if yaxe > 10:
                yaxe = 10
            yaxe += 5
            yaxe /= 15
            yaxe *= 255
            self.y_axe = int(yaxe)
    
            
            self.z_axe = int(self.z_axe)
            

            """ envoie des données à MAX
            """
            #pd_send('X %s' % self.x_axe, PD_PORT)
            #pd_send('Y %s' % self.y_axe, PD_PORT)
            #pd_send('Z %s' % self.z_axe, PD_PORT)

            client.send_message('X', self.x_axe)
            client.send_message('Y', self.y_axe)
            client.send_message('Z', self.z_axe)


        print("Glove: i'm dying... TERMINATED.")




def glove_send(GLOVE_IP, MSG):

    sock = socket.socket(
        socket.AF_INET,     # Internet
        socket.SOCK_DGRAM   # UDP
    )
    sock.sendto(MSG.encode('utf-8'), (GLOVE_IP, UDP_PORT))



if __name__ == '__main__':
    client = start_max_client()

    print("entry when pure data is ready to start")
    entry = input()
    glove_send(GLOVE_IP, "READY_TO_START")
    print("READY_TO_START packet sent to the glove!\n now listening")

    receiving_thread = glove_listener(COMPUTER_IP, UDP_PORT)
    receiving_thread.start()
    while 1:
        try:
            sleep(.1)
            print ("X: %s            Y: %s           Z: %s" % (receiving_thread.x_axe, receiving_thread.y_axe, receiving_thread.z_axe))
        except KeyboardInterrupt:
            break

    receiving_thread.must_terminate = True

    sleep(.7)
    print("THE END.")
