#!/bin/bash
./ifj22 < $1 > tmp.ifjc22
../interpret/ic22int  tmp.ifjc22
