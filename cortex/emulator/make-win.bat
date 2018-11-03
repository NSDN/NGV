@echo off

if exist .\Output\ (
    rmdir /S /Q .\Output\
)
mkdir .\Output\

echo Compiling console binary...
g++ -std=c++11 -static-libstdc++ -static-libgcc -O3 -B ./ -DNGV_SYS_VERSION="""%date:~2,2%%date:~5,2%%date:~8,2%""" ./Drivers/NGV/*.c ./Drivers/NSASM/*.cpp ./Drivers/NSGDX/*.cpp *.cpp -LDrivers/SDL2/Lib -lm -lmingw32 -lSDL2main -lSDL2 -o ./Output/ngv-emu
echo Compiling windows binary...
g++ -std=c++11 -mwindows -static-libstdc++ -static-libgcc -O3 -B ./ -DNGV_SYS_VERSION="""%date:~2,2%%date:~5,2%%date:~8,2%""" ./Drivers/NGV/*.c ./Drivers/NSASM/*.cpp ./Drivers/NSGDX/*.cpp *.cpp -LDrivers/SDL2/Lib -lm -lmingw32 -lSDL2main -lSDL2 -o ./Output/ngv-emu-win
echo Done

echo Copying library...
copy /Y .\Drivers\SDL2\SDL2.dll .\Output\SDL2.dll

echo Copying assets...
xcopy /Y /E /Q /I Assets Output\Assets

pause
