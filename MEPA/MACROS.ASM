; Biblioteca de macros da linguagem MEPA
; Equipe : Felipe, Rafael, Claudio, Mauricio 
; Segundo Semestre de 1995
; Adaptacoes Prof. Diogenes Furlan 2009
; --------------------------------------------------------------------------
; Definicoes das macros

; AMEM  MACRO m         ; Aloca memoria na pilha
; ARMI  MACRO m, n      ; Armazena indireto
; ARMZ  MACRO n         ; Armazena em variavel  
; ARMD    MACRO    m	; Armazena valor dinamico
; CHPR  MACRO p, m      ; Faz chamada a um procedimento criando R. A.      
; CONJ  MACRO           ; Conjuncao
; CRCT  MACRO k         ; Carrega constante
; CRDN    MACRO   m   ; Carrega dinamico
; CRVL  MACRO m, n      ; Carrega valor
; CREN  MACRO m, n      ; Carrega endereco
; CRVI  MACRO m, n      ; Carrega valor indireto
; CMME  MACRO           ; Compara se menor
; CMMA  MACRO           ; Compara se maior
; CMIG  MACRO           ; Compara se igual
; CMDG  MACRO           ; Compara se desigual
; CMEG  MACRO           ; Compara se menor ou igual
; CMAG  MACRO           ; Compara se maior ou igual
; DISJ  MACRO           ; Disjuncao
; DIVI  MACRO           ; Divide
; DMEM  MACRO m         ; Desaloca memoria na pilha           
; DSVR  MACRO p, j, k   ; Causa um desvio para um rotulo def. pelo usuario
; DSVS  MACRO p         ; Desvia incondicionalmente para p
; DSVF  MACRO p         ; Desvia para p se o topo da pilha contiver 0   
; ENPR  MACRO k         ; Inicializa apos entrada em procedimento
; ENRT  MACRO j, n      ; Entra em um rotulo       
; INPP  MACRO           ; Inicializa o programa
; INVR  MACRO           ; Inverte
; IMPE  MACRO           ; Imprime enter (CR+LF)
; IMPR  MACRO           ; Imprime um numero em decimal
; LEIT  MACRO           ; Le um numero do teclado
; MODI  MACRO			; Resto divisao inteira
; MULT  MACRO           ; Multiplica
; NADA  MACRO           ; Nao faz nada
; PARA  MACRO           ; Termina o programa
; RTPR  MACRO k, n      ; Deixa tudo em ordem antes de sair de um proc. 
; SOMA  MACRO           ; Soma duas posicoes no topo da pilha
; SUBT  MACRO           ; Subtrai
; --------------------------------------------------------------------------
;
;	ds -> dados
;	bp -> armazena o topo da memoria s
;	es -> programa
;



AMEM    MACRO m
;;Aloca posicoes na pilha para variaveis locais

		add     bp, 2*m
		endm

ARMI    MACRO   m, n
;;Armazena indireto
;; M[M[D[m]+n]] := M[s] ; s := s+1;

		mov     cx, word ptr es:[bp]
		sub     bp, 2
		mov     si, word ptr ds :[2*m]
		mov     di, word ptr es :[si+n+n]
		mov     word ptr es:[di], cx
		endm

ARMZ    MACRO    m, n
;;Armazena valor em variavel local
;; M[D[m]+n] := M [s]; s := s-1;

		mov     ax, word ptr es:[bp]
		sub     bp, 2
		mov     si, word ptr ds:[2*m]
		mov     word ptr es:[si+n+n], ax
		endm

ARMD    MACRO    m
;;Armazena valor dinamico
;; M[ D[m]+M[s-1] ] = M[s]; s = s-2;

		mov     ax, word ptr es:[bp]	;; M[s]
		sub     bp, 2
		mov     si, word ptr ds:[2*m]	;; D[m]
		mov     bx, word ptr es:[bp]	;; M[s-1]
		add		si, bx
		add		si, bx					;; D[m]+M[s-1]
		mov     word ptr es:[si], ax	
		sub     bp, 2
		endm
			
CHPR    MACRO   p, m
		local l1
;;Faz chamada a procedimento criando um Registro de Ativacao

		mov     ax, OFFSET l1
		add     bp, 4
		mov     word ptr es:[bp-2], ax
		mov     word ptr es:[bp], m
		jmp     p
	 l1:
		endm

CONJ    MACRO
		local   l1, l2
;;Realiza conjuncao entre [s-1] e [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     bx, 1
		jnz     l1
		cmp     ax, 1
		jnz     l1
		mov     word ptr es:[bp], 1
		jmp     l2
	li: mov     word ptr es:[bp], 0
	l2:
		endm                                              
		
CRCT    MACRO   k
;;Carrega constante no topo da pilha
;; s:=s+1; M[s] := k;

		mov     ax,k
		add     bp, 2				;;s:=s+1
		mov     word ptr es:[bp], ax
		endm
		
CRVL    MACRO   m, n
;;Carrega valor
;; s:=s+1; M[s] := M[D[m]+n];

		mov     si, word ptr ds:[2*m]
		mov     bx, word ptr es:[si+n+n]
		add     bp, 2
		mov     word ptr es:[bp], bx
		endm

CRDN    MACRO   m
;;Carrega dinamico
;; M[s] = M[ D[m]+M[s] ];

		mov     si, word ptr ds:[2*m] 	;;D[m]
		mov     di, word ptr es:[bp]	;; M[s]
		add 	si, di
		add 	si, di					;; D[m]+M[s]
		mov     bx, word ptr es:[si]
		mov     word ptr es:[bp], bx
		endm
		
CREN    MACRO   m, n
;;Carrega endereco. Usado para parametros passados por referencia.
;; s:=s+1; M[s] := D[m]+n;

		mov     ax, word ptr ds:[2*m]
		add     ax, n+n
		add     bp, 2
		mov     word ptr es:[bp], ax
		endm

CRVI    MACRO   m, n
;;Carrega valor indireto
;; s:=s+1; M[s] := M[M[D[m]+n]];

		mov     si, word ptr ds:[2*m]
		mov     di, word ptr es:[si+n+n]
		mov     ax, word ptr es:[di]
		add     bp, 2
		mov     word ptr es:[bp], ax
		endm

CMME    MACRO 
		local l1, l2
;;Compara se [s-1] < [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     ax, bx
		jl      l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1
	l2:
		endm
		
CMMA    MACRO
		local l1, l2
;;Compara se [s-1] > [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     ax, bx
		jg      l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1  
	l2:
		endm                
		
CMIG    MACRO
		local   l1, l2
;;Compara se [s-1] = [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     ax, bx
		jz      l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1
	l2:
		endm
		
CMDG    MACRO
		local   l1, l2
;;Compara se [s-1] <> [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     ax, bx
		jnz     l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1
	l2:
		endm
		
CMEG    MACRO
		local   l1, l2
;;Compara se [s-1] <= [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     ax, bx
		jle     l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1
	l2:
		endm
		
CMAG    MACRO
		local   l1, l2
;;Compara se [s-1] >= [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     ax, bx
		jge     l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1
	l2:
		endm
		
DISJ    MACRO
		local   l1, l2
;;Realiza disjuncao entre [s-1] e [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cmp     bx, 1
		jz      l1
		cmp     ax, 1
		jz      l1
		mov     word ptr es:[bp], 0
		jmp     l2
	l1: mov     word ptr es:[bp], 1
	l2:
		endm
		
DIVI    MACRO
;; Faz a divisao entre [s-1] e [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cwd
		idiv    bx
		mov     word ptr es:[bp], ax
		endm
		
DMEM    MACRO    m
;;Desaloca as posicoes da pilha
		
		sub     bp, m+m
		endm
		
DSVR    MACRO   p, j, k
		local   loop3,loop4
;;Executa um desvio para um rotulo p que esta no nivel lexico j (o nivel
;;lexido de chamada e' k).

		xor     bx, bx
		xor     cx, cx
		xor     dx, dx
		xor     si, si
		xor     ax, ax
		mov     bx, k
		mov     cx, j
 loop3: cmp     cx, bx
		jz      loop4
		add     bx, bx
		mov     si, word ptr ds:[bx]
		mov     ax, word ptr es:[si-4]
		mov     dx, word ptr es:[si-2]
		mov     word ptr ds:[bx], dx
		mov     bx, ax
		jmp     loop3
 loop4: jmp     p
		endm
		
DSVS   MACRO   p
 ;;Desvia incondicionalmente para p
 
		jmp     p
		endm
		
DSVF   MACRO   p
		local l1
 ;;Desvia para 'p' se o topo da pilha contiver 0
 
		mov     ax, word ptr es:[bp]
		sub     bp, 2
		or      ax, ax
		jnz     l1
		jmp     p
	l1: 
		endm

ENPR    MACRO    k
;;Organiza entrada em procedimento. 'k' e' o nivel lexido do proc.
;;Salva na memoria o nivel lexico antigo (D[k]) e atualiza-o para
;;apontar para a primeira variavel atual deste nivel lexico

		mov     ax, word ptr ds:[k+k]
		add     bp, 2                           ;;s:=s+1
		mov     word ptr es:[bp], ax            ;;M[s] :=M[k]
		mov     bx, bp
		add     bx, 2
		mov     word ptr ds:[k+k], bx           ;;D[K] := s+1
		endm

ENRT    MACRO   j, n
;;Entra num rotulo de nivel lexido j, tal que esse nivel possui n var.
;;locais

		mov     bx, word ptr ds:[j+j]
		mov     bp, bx
		add     bp, n+n-2
		endm
		
INPP    MACRO
;;Inicializa um programa

		mov     ax, dados
		mov     ds, ax
		mov     ax, procs
		mov     es, ax
		mov     bp, -2
		endm
		
INVR    MACRO
;;Executa complemento de dois (.-1)

		mov     ax, word ptr es:[bp]
		neg     ax
		mov     word ptr es:[bp], ax
		endm
		
IMPE   MACRO
 ;;Pula uma linha
 
		mov     dl, 0Dh
		mov     ah, 2
		int     21h
		mov     dl, 0Ah
		int     21h
		endm
	
IMPR    MACRO
		local loop1,printit
;;Imprime um numero em decimal

		push ax
		push bx
		push cx
		push dx
		push di
		mov dx, word ptr es:[bp]
		sub bp, 2
		mov cx,0
		lea di,buffer
 loop1: push cx
		mov ax,dx
		mov dx,0
		mov cx,10
		div cx
		xchg ax,dx
		add al,30h
		mov [di],al
		inc di
		pop cx
		inc cx
		cmp dx,0
		jnz loop1
 printit: dec di
		  mov al,[di]
		  push dx
		  mov dl,al
		  mov ah,2
		  int 21h
		  pop dx
		  loop printit
		  pop di
		  pop dx
		  pop cx
		  pop bx
		  pop ax
		  endm


LEIT   MACRO
		local   loop1, loop2, l1, mesg, arnd, l2
 ;; Faz uma leitura de um numero pelo teclado

		jmp     short arnd
 mesg   db      20, ?, 21 dup (0)
 arnd:  mov     ax, codigo
		mov     ds, ax
	l2: mov     dx, offset mesg  ;;buffer de teclado
		mov     ah, 0Ah          ;;le teclado
		int     21h
		mov     al, mesg[2]
		cmp     al, 0Dh
		jz      l2
		xor     bx, bx
		xor     cx, cx
		xor     dx, dx
		xor     di, di
		push    dx
		mov     di, 02h
		mov     cl, mesg[1]
 loop1: xor     ax, ax
		mov     al, mesg[di]
		sub     al, 30h
		push    ax              ;;Empilha o no. simples
		mov     ax, 1
		mov     si, 1
 loop2: cmp     si, cx
		jz      l1
		mov     bx, 0Ah         ;;                               i
		mul     bx              ;;Em AX tenho os coeficientes (10 )
		inc     si
		jmp     short loop2
	l1: pop     bx              ;;Em BX tenho o numero simples (.1.9.9.4.)
		mul     bx              ;;Em AX tenho no. coef.
		pop     dx              ;;Desempilha resultado parcial do polinomio
		add     dx, ax          ;;Em DX tenho o resultado do polinomio
		push    dx
		inc     di
		loop    loop1
		pop     dx
		mov     ax, dados
		mov     ds, ax
		add     bp, 2
		mov     word ptr es:[bp], dx
		endm

MODI    MACRO
;; Faz a divisao entre [s-1] e [s] e pega o resto

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		cwd
		idiv    bx
		mov     word ptr es:[bp], dx
		endm
		
MULT    MACRO
;;Multiplica (unsigned) [s-1] por [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		mul     bx                      ;;unsigned
		mov     word ptr es:[bp], ax
		endm



NADA   MACRO
 ;;Nao executa qualquer operacao

		endm


NEGA   MACRO
 ;;Faz 1-[s]

		mov     bx, word ptr es:[bp]
		mov     ax, 1
		sub     ax, bx
		mov     word ptr es:[bp], ax
		endm


PARA   MACRO
 ;;Termina o programa

		mov     ah, 4ch
		int     21h
		endm

RTPR   MACRO   k, n        
 ;;Organiza o retorno de procedimento

		mov     ax, word ptr es:[bp]
		mov     word ptr ds:[k+k], ax
		mov     bx, word ptr es:[bp-4]  ;;end retorno
		sub     bp, (n+3)*2
		jmp     bx
		endm

SOMA   MACRO
 ;;Soma [s-1] com [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		add     ax, bx
		mov     word ptr es:[bp], ax
		endm


SUBT   MACRO
 ;;Subtrai [s-1] com [s]

		mov     bx, word ptr es:[bp]
		mov     ax, word ptr es:[bp-2]
		sub     bp, 2
		sub     ax, bx
		mov     word ptr es:[bp], ax
		endm

IMPC	MACRO
;; imprime um caractere em [s]

		mov	ah, 02h
		mov     dl, word ptr es:[bp]  ;;caractere 'a' ou 61h
		int 21h
		
		sub	bp, 2		
		endm
		
LECH	MACRO
;; le um caractere do teclado para [s]

		mov	ah, 01h		;;funcao leitura
		int	21h
		
		add	bp, 2
		mov     word ptr es:[bp], al
		endm