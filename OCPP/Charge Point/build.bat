@echo off

gcc cJSON.c client_socket.c OCPP.c main.c -luuid -o main.exe