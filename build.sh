gcc `pkg-config --cflags gtk+-3.0 libxml-2.0` -o bin/currency-converter src/*.c `pkg-config --libs gtk+-3.0 libxml-2.0` -lcurl
