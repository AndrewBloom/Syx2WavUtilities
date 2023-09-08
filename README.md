# Syx2WavUtilities
This repo contains a c++ file and bash script to convert non standard syx files to wav
it was tested on mac

How to use:
1) compile the cpp file with g++ -o syx2standard.out -std=c++2a syx2standard.cpp
2) change the STRIPPED variable inside the bash script to point to the location of  syx2standard.out
3) launch the script as: ./syx2wavScript.sh /Users/bloom/Music/Samples/MachineDrumUW/Arcadix/SysEx, passing the actual path to the folder of your files. It expects a structure like 
SysEx/bigbeats
SysEx/dnb1
SysEx/dnb2
and inside those folders there will be the syx sample files. It will create a symmetrical structure as
SysEx/wav/bigbeats
SysEx/wav/dnb1
SysEx/wav/dnb2
containing your new wav files.

Enjoy!
