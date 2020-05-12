touch tests//.timeout
CMD="/usr/bin/valgrind --leak-check=full /home/mario/Documentos/MP/NetBeans/tiles/dist/Debug/GNU-Linux/tiles  1> tests//.out13 2>&1"
eval $CMD
rm tests//.timeout
