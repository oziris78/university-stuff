@echo off

IF EXIST "main.obj" (del "main.obj")
IF EXIST "main.exe" (del "main.exe")
IF EXIST "main.com" (del "main.com")
IF EXIST "stdout.txt" (del "stdout.txt")
IF EXIST "stderr.txt" (del "stderr.txt")

"C:\Program Files (x86)\DOSBox-0.74-3\DOSBox.exe" -noconsole -userconf ^
    -c "mount c \"%~dp0\"" ^
    -c "c:" ^
    -c "_masm.exe main.asm;" ^
    -c "_link.exe main.obj;" ^
    -c "_exe2com.exe main.exe" ^
    -c "_debug.exe main.com"

@rem IF EXIST "main.obj" (del "main.obj")
@rem IF EXIST "main.exe" (del "main.exe")
@rem IF EXIST "main.com" (del "main.com")
@rem IF EXIST "stdout.txt" (del "stdout.txt")
@rem IF EXIST "stderr.txt" (del "stderr.txt")
