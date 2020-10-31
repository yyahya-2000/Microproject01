; Ф.И.Г: Яхя Янал БПИ197
; Вариант №28
; Задание:
;Разработать программу, вычисляющую с помощью степенного ряда с точностью не хуже 0,1% значение функции
;гиперболического тангенса th(x)=(ex-ex)/(ex+e-x) для заданного параметра x (использовать FPU)

format PE console
entry start

include 'win32a.inc'
include 'my_lib.inc'                     ; including my Library

section '.code' code readable executable

start:
    ; input x
    invoke printf, strx
    invoke scanf,  formatEnterX, x;
    FINIT                                 ;coprocessor initialization
    jmp FirstCon

IncorrectInput:
    invoke printf, IncInput
    jmp start

FirstCon:
    ;check if x is in range [-4.4, 4.4]
    fld [x]                               ;loading x into the top of the stack st(0)
    fcomp [rLimitation]                   ; comparing st(0) and rLimitation, with clearing st(0)
    fstsw   ax
    sahf
    jnbe IncorrectInput                            ; if st(0) > rLimitation

secondCon:
    fld [x]
    fcomp [lLimitation]                   ; comparing st(0) and lLimitation, with clearing st(0)
    fstsw   ax
    sahf
    jb IncorrectInput                              ; if st(0) < lLimitation

    invoke printf, formatX,  dword[x], dword[x+4] ;print x
    invoke printf, formatInt ,[N]

    TH [x]                                ; calling TH macro in my_lib

    ; printing the result of tanh(x)
    invoke printf, formatTH,dword[x], dword[x+4],  dword[th_res], dword[th_res+4]
    invoke getch

;-------------------------------third act - including HeapApi--------------------------
section '.data' data readable writable

    strx        db    "Enter x between [-4.4, 4.4], to calculate tanh(x): ", 0
    IncInput    db    "Oops, your input value was outside the range [-4.4, 4.4]. Please try agian!",10,0
    formatEnterX db   '%lf' , 0
    formatTH    db    'tanh(%lf) = %lf',10, 0
    formatX     db    'x = %lf',10, 0
    formatInt   db    'N = %d',10, 0
    x           dq    0
    e_To_X      dq    ?
    e_To_nX     dq    ?
    N           dd    12
    resPow      dq    ?
    resFac      dd    ?
    resSum      dq    1.0
    j           dd    0
    th_res      dq    ?
    denominator dq    ?
    rLimitation dq    4.4
    lLimitation dq    -4.4
    tmp         dq    ?

section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll'

    import kernel,\
           ExitProcess, 'ExitProcess'
    import msvcrt,\
           printf, 'printf',\
           sprintf, 'sprintf',\
           scanf, 'scanf',\
           getch, '_getch'
