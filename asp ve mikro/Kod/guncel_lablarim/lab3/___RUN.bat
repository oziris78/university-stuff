@echo off

@rem IF EXIST "19052025.obj" (del "19052025.obj")
@rem IF EXIST "19052025.exe" (del "19052025.exe")
@rem IF EXIST "stdout.txt" (del "stdout.txt")
@rem IF EXIST "stderr.txt" (del "stderr.txt")

"C:\Program Files (x86)\DOSBox-0.74-3\DOSBox.exe" -noconsole -userconf ^
    -c "mount c \"%~dp0\"" ^
    -c "c:" ^
    -c "_masm.exe 19052025.asm;" ^
    -c "_link.exe 19052025.obj;" ^
    -c "_debug.exe 19052025.exe"

@rem IF EXIST "19052025.obj" (del "19052025.obj")
@rem IF EXIST "19052025.exe" (del "19052025.exe")
@rem IF EXIST "stdout.txt" (del "stdout.txt")
@rem IF EXIST "stderr.txt" (del "stderr.txt")
