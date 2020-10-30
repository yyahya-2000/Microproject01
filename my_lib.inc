;------------Factorial macro-----------------------------
macro Fac N {
local Factorial, endf
    mov eax, 1                  ; loading 1 into register eax, in eax will be saved the result of N!
    mov ecx, N                  ; loading N into register ecx
Factorial:
          cmp ecx,0             ;compare ecx with 0
          jle endf              ;end loop if N <= 0
          mul ecx               ; eax <- eax * ecx
          dec ecx               ; ecx <- ecx - 1
          jmp Factorial         ; goto  Factorial
     endf:
          push eax              ; pushing the value from eax to the stack
}

;-------------Power macro----------------------------------
macro Power X, N{
local CalcPower, endPoweer
     mov eax, 1                 ; loading 1 into register eax, in eax will be saved the result of N!
     mov ecx, N                 ; loading N into register ecx
     fld1                       ; loading 1 into the top of the stack st(0)
CalcPower:
          cmp ecx, 0            ; comparing ecx with 0
          je endPoweer          ; end loop if ecx == 0
          fmul X                ; st(0) <- st(0) * X
          dec ecx               ; ecx <- ecx - 1
          jmp CalcPower         ; goto CalcPower
endPoweer:
        fstp [resPow]           ; writing the result of x^N to memory in resPow, with clearing the stack

}

;------------e^x macro--------------------------------------
macro EpowX X, N{
local  Ex_Loop, Start_Ex_Loop, end_Ex_Loop
Ex_Loop:
    mov ebx, N ; loading N into register ebx
Start_Ex_Loop:
        cmp ebx,0               ; compare ebx with 0
        je end_Ex_Loop          ; end the loop if ebx == 0
        mov [j],ebx             ; saving temp value of ebx in j
        Fac ebx                 ; calling Fac macro to calculate ebx!
        pop [resFac]            ; saving the result of Fac macro(ebx!) which was pushed to stack, in memory into resFac
        Power X, ebx            ; calling Power macro to calculate X^ebx
        fld [resPow]            ; loading resPow into the top of the stack st(0)
        fidiv [resFac]          ; s(0) <- s(0) / resFac
        fadd [resSum]           ; s(0) <- s(0) + resSum
        fstp [resSum]           ; resSum <- s(0), and clear s(0)
        mov ebx, [j]            ; restore the value of ebx from the value of j
        dec ebx                 ; ebx <- ebx - 1
        jmp Start_Ex_Loop       ; goto Start_Ex_Loop
end_Ex_Loop:
        nop                     ; no operation
}

;---------------tanh(x) macro-----------------------------------
macro TH X{
        fld X                   ; loading X into the top of the stack st(0)
        fstp [tmp]              ; tmp <- st(0), and clear st(0)
        EpowX X, [N]            ; calling EpowX macro (e^X)
        fld [resSum]            ; loading resSum into the top of the stack st(0)
        fstp [e_To_X]           ; e_To_X <- st(0), and clearing st(0)
        ; reinitialize resSum to 1
        fld1                    ; loading 1 into the top of the stack st(0)
        fstp [resSum]           ; resSum <- st(0), and clearing st(0)
        ; getting -X
        fldz                    ; loading 0 into the top of the stack st(0)
        fsub X                  ; st(0) <- st(0) - X
        fstp X                  ; X <- st(0), and clear st(0)

        EpowX X, [N]            ; calling EpowX macro (e^-X)
        fld [resSum]            ; loading resSum into the top of the stack st(0)
        fstp [e_To_nX]          ; e_To_X <- st(0), and clearing st(0)
        fld [e_To_X]            ; loading e_To_X into the top of the stack st(0)
        fadd [e_To_nX]          ; st(0) <- st(0) + e_To_nX
        fstp [denominator]      ; denominator <- st(0), and clear st(0)
        fld [e_To_X]            ; loading e_To_X into the top of the stack st(0)
        fsub [e_To_nX]          ; st(0) <- st(0) - e_To_nX
        fdiv [denominator]      ; st(o) <- st(0) / denominator
        fstp [th_res]           ; th_res <- st(0), and clear st(0)
        ; re-assign to base its base value
        fld[tmp]
        fstp X
}
