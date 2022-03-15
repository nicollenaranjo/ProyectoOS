import socket
import sys
import os

def createContainer(name):
    # Create a TCP/IP socket
    sock = socket.create_connection(('localhost', 7070))

    try:
            mesStr = "create, {0}".format(name)
            message = bytes(mesStr, 'utf-8')
            print('sending {!r}'.format(message))
            sock.sendall(message)

            amount_received = 0
            amount_expected = 6

            while amount_received < amount_expected:
                data = sock.recv(160)
                amount_received += len(data)
                print('received {!r}'.format(data))

    finally:
        print('closing socket')
        sock.close()
        return "creation successful"

def startContainer(name):
    # Create a TCP/IP socket
    sock = socket.create_connection(('localhost', 7070))

    try:
            mesStr = "start, {0}".format(name)
            message = bytes(mesStr, 'utf-8')
            print('sending {!r}'.format(message))
            sock.sendall(message)

            amount_received = 0
            amount_expected = 6

            while amount_received < amount_expected:
                data = sock.recv(160)
                amount_received += len(data)
                print('received {!r}'.format(data))

    finally:
        print('closing socket')
        sock.close()
        return "start successful"

def stopContainer(name):
    # Create a TCP/IP socket
    sock = socket.create_connection(('localhost', 7070))

    try:
            mesStr = "stop, {0}".format(name)
            message = bytes(mesStr, 'utf-8')
            print('sending {!r}'.format(message))
            sock.sendall(message)

            amount_received = 0
            amount_expected = 6

            while amount_received < amount_expected:
                data = sock.recv(160)
                amount_received += len(data)
                print('received {!r}'.format(data))

    finally:
        print('closing socket')
        sock.close()
        return "stop successful"

def deleteContainer(name):
    # Create a TCP/IP socket
    sock = socket.create_connection(('localhost', 7070))

    try:
            mesStr = "delete, {0}".format(name)
            message = bytes(mesStr, 'utf-8')
            print('sending {!r}'.format(message))
            sock.sendall(message)

            amount_received = 0
            amount_expected = 6

            while amount_received < amount_expected:
                data = sock.recv(160)
                amount_received += len(data)
                print('received {!r}'.format(data))

    finally:
        print('closing socket')
        sock.close()
        return "deletion successful"

def listContainers():
    # Create a TCP/IP socket
    sock = socket.create_connection(('localhost', 7070))

    try:
            mesStr = "list"
            message = bytes(mesStr, 'utf-8')
            print('sending {!r}'.format(message))
            sock.sendall(message)

            amount_received = 0
            amount_expected = 6

            while amount_received < amount_expected:
                data = sock.recv(160)
                amount_received += len(data)
                print('received {!r}'.format(data))

    finally:
        print('closing socket')
        sock.close()
        return 0
