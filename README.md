# SCP-DATA
## to compile:
### statically assemble bezier.c 
[code]
${YOUR_C_COMPILER} -c bezier.c -o bezier.o
[/code]
### fully compile
[code]
${YOUR_C_COMPILER} bezier.o img.c -o img
[/code]

## use:
[code]
./img (uint16)height (uint16)width (uint16)circle_radius (uint16)max_of_scps (char)is_range
[/code]
## closing remarks:

is_range should be 'r' if you are looking to create more then one scp.pbm file at a time. If you use any character but 'r' you will generate one image.

radius should not exceed height or width (duh).

Please do not come complaining to me about segfaults. This is *not* commercial grade software and at some indeterminate point I'll obviously stop maintaining this repo. 

