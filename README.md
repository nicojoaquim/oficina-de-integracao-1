# Oficina de Integração 1

https://stackoverflow.com/questions/77850844/how-do-i-run-a-binary-linked-against-a-shared-library-without-using-ld-library-p
https://stackoverflow.com/questions/8482152/whats-the-difference-between-rpath-and-l

$ g++ -o BINARIO *.cpp -L/opt/sfml2/lib/ -lsfml-system -lsfml-window -lsfml-graphics -lcurl -I/opt/sfml2/include/ -Wl,-rpath=/opt/sfml2/lib/
$ ./BINARIO

OU

$ g++ -o BINARIO *.cpp -L/opt/sfml2/lib/ -lsfml-system -lsfml-window -lsfml-graphics -lcurl -I/opt/sfml2/include/
$ LD_LIBRARY_PATH=/opt/sfml2/lib/:$LD_LIBRARY_PATH ./BINARIO
