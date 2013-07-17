asio-dccp
=========

Description
===========
Boost asio extension headers and test program for DCCP using boost asio on linux (Ubuntu).

Prerequisites
=============
The asio extension headers are based on boost 1.53 though
earlier versions of boost may be compatible.

Check DCCP configuration
========================
sudo modprobe dccp
sysctl -a | grep dccp

Set CCID to CCID2:
=================
sudo sysctl -w net.dccp.default.rx_ccid=2
sudo sysctl -w net.dccp.default.tx_ccid=2

Compilation
===========

g++ dccp_server.cpp -lboost_system -lboost_thread -o dccp_server

g++ dccp_client.cpp -lboost_system -lboost_thread -lpthread -o dccp_client


