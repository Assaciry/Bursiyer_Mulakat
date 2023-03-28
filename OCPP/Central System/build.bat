@echo off

gcc cJSON.c server_socket.c OCPP.c main.c -luuid -o main.exe