#!/bin/bash
# start server

PORT=6666

../dtlssrtp_example -b 0.0.0.0 -c certs/cert.pem -k certs/key.pem -s -p $PORT

