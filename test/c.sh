#!/bin/bash
# start client

PORT_LOCAL=7777
PORT_SERVER=6666
IP_SERVER=127.0.0.1

../dtlssrtp_example -c certs2/cert.pem -k certs2/key.pem -p $PORT_LOCAL $IP_SERVER $PORT_SERVER

