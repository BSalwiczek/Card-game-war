# War card game
Project was created for subject basics of programming (IT specialization) in Gdansk university (Politechnika Gdańska). It contains two modes:
* Normal war (variant A or B)
* Wise war

Rules of the gameplay are described in file [rules.pdf](rules.pdf) (only Polish)

## Prerequisites
This program contains graphics text interface which is only available on Linux machine. In order to run war card game you need to install gcc compiler, make, and library ncurses
```
sudo apt install gcc
sudo apt-get install build-essential
sudo apt-get install libncurses-dev
```

## Running
You need to compile .c and .h files
```
make
```
To run simply type
```
./main
```

## Simulations
If you want to make some simulations you need to uncomment ```#define SIMULATION``` flag in main.h file and change settings in ```#ifdef SIMULATION``` in main.c file to adjust it to your needs.

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
