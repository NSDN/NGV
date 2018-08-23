@echo off

if exist .\Output\ (
    rmdir /S /Q .\Output\
)
mkdir .\Output\

echo Compiling binary...
g++ -std=c++11 -static-libstdc++ -static-libgcc -O3 -B ./ ./Drivers/NGV/*.c ./Drivers/NSASM/*.cpp *.cpp -LDrivers/SDL2/Lib -lm -lmingw32 -lSDL2main -lSDL2 -o ./Output/ngv-emu
echo Done

echo Copying library...
copy /Y .\Drivers\SDL2\SDL2.dll .\Output\SDL2.dll

echo Copying assets...
xcopy /Y /E /Q /I Assets Output\Assets

pause
