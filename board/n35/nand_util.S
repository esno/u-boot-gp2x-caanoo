#include <config.h>
#include <version.h>


	.text
    .align
/*
 *	void boot_read_verify(void);
 */
	.globl boot_read_verify
boot_read_verify:
    stmdb   sp!, { r1 - r12, lr }   
    sub sp, sp, #4                  
	str r0, [ sp ]   
    
    ldr	r5, ram_base	    @ R5 RAM base address */
    mov	r6, #0x0	        @ R6 nand flash base page
	mov	r9, #0x80	        @ R9 boot page size (252kb)
    	
/* Read loop */
	mov r4, #0
next_page:
    mov r2, #0              @ R2 nand flash offset  
read_command:
	ldr r1, MCTRLW
	ldrh r0, [r1]
	orr	r0, r0, #0x8000	
	strh r0, [r1]

    @ Set read cmd 
	ldr	r1, NCMD
	mov	r0, #0x0
	strb r0, [r1]

	@ Set address 1
	ldr	r1, NADDR
	mov	r0, r2			
	strb r0, [r1]
	@ Set address 2
	mov	r0, r2, lsr #8
	strb r0, [r1]
	
	@ Set address 3
	mov	r0, r6 
	strb r0, [r1]
	@ Set address 4
	mov	r0, r6, lsr #8
	strb r0, [r1]
	@ Set address 5     
	mov	r0, r6, lsr #16
	strb r0, [r1]
    
    @Set read end cmd 
    ldr	r1, NCMD
    mov	r0, #0x30
	strb r0, [r1]

/*	Wait ready */
wait_ready:
	ldr	r1, MCTRLW
	ldrh r0, [r1]
	and	r0, r0, #0x8000
	cmp	r0, #0x8000
	bne	wait_ready
	ldrh r0, [r1]
	orr	r0, r0, #0x8000	
	strh r0, [r1]
			
/*	Get data */
    mov r8, #0x0
get_page:
	ldr	r1, NDATA
	ldr	r0, [r1]
	str	r0, [r5]
	add	r8, r8, #0x4
	add	r5, r5, #0x4
	cmp	r8, #0x200
	bne	get_page

    add r2, r2, #0x200
#   cmp r2, #0x1000             @ 4096
    cmp r2, #0x800              @ 2048
    bne read_command  
    
	add	r6, r6, #0x1
	cmp	r6, r9
	ble	next_page

	add sp, sp, #4
	ldmia   sp!, { r1 - r12, pc }   @ restore callee saved regs and return
	
	#mov	pc, lr

NDATA:		
	.word	0xAC000000
NCMD:		
	.word	0xAC000010
NADDR:		
	.word	0xAC000018
MCTRLW:	    
	.word	0xC0015874
ram_base:
	.word	0x01000000