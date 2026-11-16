; Лабораторная работа №2 по дисциплине "Организация ЭВМ и систем";
;
EOL  EQU '$'
ind  EQU 2
n1   EQU 500
n2   EQU -50

; Стек программы
AStack SEGMENT STACK
        DW 12 DUP(?)
AStack ENDS

; Данные программы
DATA SEGMENT
mem1  DW 0
mem2  DW 0
mem3  DW 0

vec1  DB 5,6,7,8,12,11,10,9
vec2  DB 20,30,-20,-30,-40,-50,40,50
matr  DB -5,-6,4,3,-7,-8,2,1,-1,-2,-3,-4,8,7,2,5
DATA ENDS

; Код программы
CODE SEGMENT
        ASSUME CS:CODE, DS:DATA, SS:AStack

Main PROC FAR
        push DS
        sub  AX,AX
        push AX
        mov  AX,DATA
        mov  DS,AX

; Инициализация регистров и загрузка констант
        mov  ax,n1
        mov  cx,ax
        mov  bl,EOL
        mov  bh,n2

; Перемещение значений
        mov  mem2,n2
        mov  bx,OFFSET vec1
        mov  mem1,ax

; Чтение элементов массива
        mov  al,[bx]
; Ошибка: размер [bx] не определён. mem3 — слово.
; Исправленный вариант:
;       mov  ax,[bx]
;       mov  mem3,ax

; Загрузка следующего элемента
        mov  al,[bx]+3
; Предупреждение: размеры источника и приёмника различаются.
        mov  cx,3[bx]

; Доступ к элементам массива
        mov  di,ind
        mov  al,vec2[di]
; Предупреждение: vec2 — байтовый массив, CX — слово.
        mov  cx,vec2[di]

; Работа с матрицей и индексами
        mov  bx,3
        mov  al,matr[bx][di]
; Предупреждение: matr — байтовый массив, CX — слово.
        mov  cx,matr[bx][di]

; Ошибка: 8086 не поддерживает адресацию с умножением регистра.
; Исправленный эквивалент: вычислить смещение заранее.
;       mov  si,bx
;       shl  si,1
;       shl  si,1
;       mov  ax,matr[si][di]

; Работа с сегментами и адресацией
; Случай 1
        mov  ax,SEG vec2
        mov  es,ax
        mov  ax,es:[bx]
        mov  ax,0

; Случай 2
        mov  es,ax
        push ds
        pop  es
        mov  cx,es:[bx-1]
        xchg cx,ax

; Случай 3
        mov  di,ind
        mov  es:[bx+di],ax

; Случай 4
        mov  bp,sp

; Ошибка: BP+BX не является допустимой парой адресных регистров 8086.
; Сложить смещения заранее, затем обратиться через SI.
;       mov  si,bp
;       add  si,bx
;       mov  ax,matr[si]

; Ошибка: одновременно заданы два индексных регистра, DI и SI.
; Сложить одно смещение заранее, затем использовать допустимую пару.
;       mov  si,bp
;       add  si,si
;       mov  ax,matr[bp+di] ; пример допустимой адресной формы

; Восстановление стека и возврат
        push mem1
        push mem2
        mov  bp,sp
        mov  dx,[bp]+2
        ret
Main ENDP

CODE ENDS
        END Main
