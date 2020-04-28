touch tests//.timeout
CMD="/usr/bin/valgrind --leak-check=full /home/mario/Documentos/MP/NetBeans/tiles/dist/Debug/GNU-Linux/tiles  -l ES -r 100 -w 10 -h 10 -p data/OPEN_ERROR.data 1> tests//.out12 2>&1"
eval $CMD
rm tests//.timeout
