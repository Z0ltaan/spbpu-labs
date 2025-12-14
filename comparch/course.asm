%macro inputchar 1  ;вызов read из stdin
    push ebx
    push ecx
    push edx

    mov eax, 3
    mov ebx, 0
    mov ecx, %1
    mov edx, 1
    int 80h

    pop edx
    pop ecx
    pop ebx
%endmacro

%macro printchar 1   ;вызов write в stdout
    push eax
    push ebx
    push ecx
    push edx

    mov eax, 4
    mov ebx, 1
    mov ecx, %1
    mov edx, 1
    int 80h

    pop edx
    pop ecx
    pop ebx
    pop eax
%endmacro

%macro determinechar 1    ;определяем: строчная/заглавная буква или не буква
  pushf

  cmp byte [%1], 'A' ; Проверка на uppercase
  jl not_letter
  cmp byte [%1], 'Z'
  jle is_uppercase

  cmp byte [%1], 'a' ; Проверка на lowercase
  jl not_letter
  cmp byte [%1], 'z'
  jle is_lowercase

  not_letter:
    mov ax, 0 ; AX = 0, если символ не является буквой
    jmp exit

  is_uppercase:
    mov ax, 1 ; AX = 1, если символ является uppercase буквой
    jmp exit

  is_lowercase:
    mov ax, 2 ; AX = 2, если символ является lowercase буквой
  exit:

  popf
%endmacro

global _start

section .data
a db ';'
space db 0x20
eol db 0x0A
count dw 26 dup(0)
status dd 0

section .text
_start:
  mov ebx, count        ;начало ввода
  input_loop:
    inputchar a
    mov dword [status], eax   ;сохраняем статус ввода

    determinechar a   ; проверка на буквы

    cmp ax, 1
    jl not_let
    jg lower

    add byte [a], 32
    lower:
      xor edx, edx           ;получаем индекс массива из ascii-кода буквы
      mov dl, [a]
      sub dl, 97
      mov ax, [count + edx * 2]   ; увеличиваем соответствующий счетчик
      add ax, 1
      mov [count + edx * 2], ax
    not_let:
      cmp dword [status], 0    ; проверка на конец ввода
      jne input_loop

;  вывод диаграммы

  printchar eol

  mov byte [a], 'a'  ;начальное значение на оси диаграммы
  mov ecx, 0
  while:
    cmp cx, 26
    je end_while

    printchar a      ;выводим значение на оси и отступ для данных
    printchar space
    printchar space

    mov ax, [count + ecx * 2] ; берем соответствующее значение счетчика и печатаем данные
    outstr:
      cmp ax, 0
      je endout

      printchar a

      dec ax
      jmp outstr
    endout:

    printchar eol   ;переход на следующую строку и соответственно след. значение
    inc cx
    add byte [a], 1
    jmp while

  end_while:
  printchar eol

  mov eax,1
  mov ebx,0
  int 80h
