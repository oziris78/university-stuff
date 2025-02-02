@echo off

@rem IF EXIST "main.obj" (del "main.obj")
@rem IF EXIST "main.exe" (del "main.exe")
@rem IF EXIST "stdout.txt" (del "stdout.txt")
@rem IF EXIST "stderr.txt" (del "stderr.txt")

"C:\Program Files (x86)\DOSBox-0.74-3\DOSBox.exe" -noconsole -userconf ^
    -c "mount c \"%~dp0\"" ^
    -c "c:" ^
    -c "_masm.exe main.asm;" ^
    -c "_link.exe main.obj;" ^
    -c "_debug.exe main.exe"

@rem IF EXIST "main.obj" (del "main.obj")
@rem IF EXIST "main.exe" (del "main.exe")
@rem IF EXIST "stdout.txt" (del "stdout.txt")
@rem IF EXIST "stderr.txt" (del "stderr.txt")
