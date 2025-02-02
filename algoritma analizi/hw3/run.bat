

@echo off

IF EXIST "C19052025.exe" (del "C19052025.exe")

"C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\x86_64-w64-mingw32-gcc-4.9.2" C19052025.c -o C19052025.exe

@echo on

C19052025.exe code3.txt DEBUG

