#!/bin/bash
make e
./expr > tmp.ifjc22
../interpret/ic22int  tmp.ifjc22
