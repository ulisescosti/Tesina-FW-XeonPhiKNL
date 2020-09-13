#!/bin/bash

bs_array=(32 64 128 256)

for bs in ${bs_array[@]}
do
  make -B BS=$bs
done
