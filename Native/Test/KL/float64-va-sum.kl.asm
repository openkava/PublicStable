	.def	 ___construct_String__Boolean;
	.scl	2;
	.type	32;
	.endef
	.text
	.globl	___construct_String__Boolean
	.align	16, 0x90
___construct_String__Boolean:           # @__construct_String__Boolean
# BB#0:                                 # %entry
	pushl	%esi
Ltmp0:
	subl	$16, %esp
Ltmp1:
	movl	24(%esp), %esi
	testb	$1, 28(%esp)
	je	LBB0_2
# BB#1:                                 # %true
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_1, 4(%esp)
	calll	___construct_String__ConstString4
	movl	12(%esp), %eax
	jmp	LBB0_3
LBB0_2:                                 # %false
	movl	$0, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_2, 4(%esp)
	calll	___construct_String__ConstString5
	movl	8(%esp), %eax
LBB0_3:                                 # %merge
	movl	(%esi), %ecx
	movl	%eax, (%esi)
	movl	%ecx, (%esp)
	calll	___String__Release
	addl	$16, %esp
	popl	%esi
	ret

	.def	 ___construct_Boolean__ConstString4;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__ConstString4
	.align	16, 0x90
___construct_Boolean__ConstString4:     # @__construct_Boolean__ConstString4
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	movb	$1, (%eax)
	ret

	.def	 ___construct_String__ConstString4;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__ConstString4
	.align	16, 0x90
___construct_String__ConstString4:      # @__construct_String__ConstString4
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp2:
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	movl	$___ConstString4__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___String__Release;
	.scl	2;
	.type	32;
	.endef
	.globl	___String__Release
	.align	16, 0x90
___String__Release:                     # @__String__Release
# BB#0:                                 # %entry
	pushl	%eax
Ltmp3:
	movl	8(%esp), %eax
	testl	%eax, %eax
	je	LBB3_3
# BB#1:                                 # %nonNull
	movl	$-1, %ecx
	lock
	xaddl	%ecx, (%eax)
	cmpl	$1, %ecx
	jne	LBB3_3
# BB#2:                                 # %free
	movl	%eax, (%esp)
	calll	_free
LBB3_3:                                 # %done
	popl	%eax
	ret

	.def	 ___construct_Boolean__ConstString5;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__ConstString5
	.align	16, 0x90
___construct_Boolean__ConstString5:     # @__construct_Boolean__ConstString5
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	movb	$1, (%eax)
	ret

	.def	 ___construct_String__ConstString5;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__ConstString5
	.align	16, 0x90
___construct_String__ConstString5:      # @__construct_String__ConstString5
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp4:
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	movl	$___ConstString5__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___operator_BIT_NOT__Boolean;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_NOT__Boolean
	.align	16, 0x90
___operator_BIT_NOT__Boolean:           # @__operator_BIT_NOT__Boolean
# BB#0:                                 # %entry
	movb	4(%esp), %al
	xorb	$1, %al
	ret

	.def	 ___operator_BIT_OR__Boolean__Boolean;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_OR__Boolean__Boolean
	.align	16, 0x90
___operator_BIT_OR__Boolean__Boolean:   # @__operator_BIT_OR__Boolean__Boolean
# BB#0:                                 # %entry
	movb	4(%esp), %al
	orb	8(%esp), %al
	ret

	.def	 ___operator_BIT_AND__Boolean__Boolean;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_AND__Boolean__Boolean
	.align	16, 0x90
___operator_BIT_AND__Boolean__Boolean:  # @__operator_BIT_AND__Boolean__Boolean
# BB#0:                                 # %entry
	movb	4(%esp), %al
	andb	8(%esp), %al
	ret

	.def	 ___operator_BIT_XOR__Boolean__Boolean;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_XOR__Boolean__Boolean
	.align	16, 0x90
___operator_BIT_XOR__Boolean__Boolean:  # @__operator_BIT_XOR__Boolean__Boolean
# BB#0:                                 # %entry
	movb	4(%esp), %al
	xorb	8(%esp), %al
	ret

	.def	 ___operator_EQ__Boolean__Boolean;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__Boolean__Boolean
	.align	16, 0x90
___operator_EQ__Boolean__Boolean:       # @__operator_EQ__Boolean__Boolean
# BB#0:                                 # %entry
	movb	4(%esp), %al
	xorb	8(%esp), %al
	xorb	$1, %al
	ret

	.def	 ___operator_NE__Boolean__Boolean;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__Boolean__Boolean
	.align	16, 0x90
___operator_NE__Boolean__Boolean:       # @__operator_NE__Boolean__Boolean
# BB#0:                                 # %entry
	movb	4(%esp), %al
	xorb	8(%esp), %al
	ret

	.def	 ___construct_Boolean__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Byte
	.align	16, 0x90
___construct_Boolean__Byte:             # @__construct_Boolean__Byte
# BB#0:                                 # %entry
	cmpb	$0, 8(%esp)
	movl	4(%esp), %eax
	setne	(%eax)
	ret

	.def	 ___construct_Integer__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Integer__Byte
	.align	16, 0x90
___construct_Integer__Byte:             # @__construct_Integer__Byte
# BB#0:                                 # %entry
	movzbl	8(%esp), %eax
	movl	4(%esp), %ecx
	movl	%eax, (%ecx)
	ret

	.def	 ___construct_Size__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Size__Byte
	.align	16, 0x90
___construct_Size__Byte:                # @__construct_Size__Byte
# BB#0:                                 # %entry
	movzbl	8(%esp), %eax
	movl	4(%esp), %ecx
	movl	%eax, (%ecx)
	ret

	.def	 ___construct_Scalar__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Scalar__Byte
	.align	16, 0x90
___construct_Scalar__Byte:              # @__construct_Scalar__Byte
# BB#0:                                 # %entry
	movzbl	8(%esp), %eax
	cvtsi2ss	%eax, %xmm0
	movl	4(%esp), %eax
	movss	%xmm0, (%eax)
	ret

	.def	 ___construct_String__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Byte
	.align	16, 0x90
___construct_String__Byte:              # @__construct_String__Byte
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp5:
	movb	20(%esp), %al
	movb	%al, 11(%esp)
	leal	11(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Byte__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___operator_POS__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POS__Byte
	.align	16, 0x90
___operator_POS__Byte:                  # @__operator_POS__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	ret

	.def	 ___operator_BIT_NOT__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_NOT__Byte
	.align	16, 0x90
___operator_BIT_NOT__Byte:              # @__operator_BIT_NOT__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	notb	%al
	ret

	.def	 ___operator_PRE_INC__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_PRE_INC__Byte
	.align	16, 0x90
___operator_PRE_INC__Byte:              # @__operator_PRE_INC__Byte
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movb	(%ecx), %al
	incb	%al
	movb	%al, (%ecx)
	ret

	.def	 ___operator_PRE_DEC__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_PRE_DEC__Byte
	.align	16, 0x90
___operator_PRE_DEC__Byte:              # @__operator_PRE_DEC__Byte
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movb	(%ecx), %al
	decb	%al
	movb	%al, (%ecx)
	ret

	.def	 ___operator_POST_INC__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POST_INC__Byte
	.align	16, 0x90
___operator_POST_INC__Byte:             # @__operator_POST_INC__Byte
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movb	(%ecx), %al
	movb	%al, %dl
	incb	%dl
	movb	%dl, (%ecx)
	ret

	.def	 ___operator_POST_DEC__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POST_DEC__Byte
	.align	16, 0x90
___operator_POST_DEC__Byte:             # @__operator_POST_DEC__Byte
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movb	(%ecx), %al
	movb	%al, %dl
	decb	%dl
	movb	%dl, (%ecx)
	ret

	.def	 ___operator_ADD__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__Byte__Byte
	.align	16, 0x90
___operator_ADD__Byte__Byte:            # @__operator_ADD__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	addb	8(%esp), %al
	ret

	.def	 ___operator_SUB__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_SUB__Byte__Byte
	.align	16, 0x90
___operator_SUB__Byte__Byte:            # @__operator_SUB__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	subb	8(%esp), %al
	ret

	.def	 ___operator_MUL__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MUL__Byte__Byte
	.align	16, 0x90
___operator_MUL__Byte__Byte:            # @__operator_MUL__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	mulb	8(%esp)
	ret

	.def	 ___operator_DIV__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_DIV__Byte__Byte
	.align	16, 0x90
___operator_DIV__Byte__Byte:            # @__operator_DIV__Byte__Byte
# BB#0:                                 # %entry
	pushl	%esi
Ltmp6:
	subl	$16, %esp
Ltmp7:
	movb	28(%esp), %cl
	testb	%cl, %cl
	je	LBB26_3
# BB#1:                                 # %nonZero
	movb	24(%esp), %al
	movzbl	%al, %eax
	divb	%cl
LBB26_2:                                # %nonZero
	addl	$16, %esp
	popl	%esi
	ret
LBB26_3:                                # %zero
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_3, 4(%esp)
	calll	___construct_String__ConstString25
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	xorb	%al, %al
	jmp	LBB26_2

	.def	 ___construct_Boolean__ConstString25;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__ConstString25
	.align	16, 0x90
___construct_Boolean__ConstString25:    # @__construct_Boolean__ConstString25
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	movb	$1, (%eax)
	ret

	.def	 ___construct_String__ConstString25;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__ConstString25
	.align	16, 0x90
___construct_String__ConstString25:     # @__construct_String__ConstString25
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp8:
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	movl	$___ConstString25__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___String__Report;
	.scl	2;
	.type	32;
	.endef
	.globl	___String__Report
	.align	16, 0x90
___String__Report:                      # @__String__Report
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp9:
	movl	16(%esp), %eax
	testl	%eax, %eax
	je	LBB29_2
# BB#1:                                 # %notNull
	movl	8(%eax), %ecx
	movl	%ecx, 4(%esp)
	addl	$12, %eax
	movl	%eax, (%esp)
	calll	_report
LBB29_2:                                # %done
	addl	$12, %esp
	ret

	.def	 ___operator_MOD__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MOD__Byte__Byte
	.align	16, 0x90
___operator_MOD__Byte__Byte:            # @__operator_MOD__Byte__Byte
# BB#0:                                 # %entry
	pushl	%esi
Ltmp10:
	subl	$16, %esp
Ltmp11:
	movb	28(%esp), %cl
	testb	%cl, %cl
	je	LBB30_3
# BB#1:                                 # %nonZero
	movb	24(%esp), %al
	movzbl	%al, %eax
	divb	%cl
	movb	%ah, %al
LBB30_2:                                # %nonZero
	addl	$16, %esp
	popl	%esi
	ret
LBB30_3:                                # %zero
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_4, 4(%esp)
	calll	___construct_String__ConstString25
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	xorb	%al, %al
	jmp	LBB30_2

	.def	 ___operator_BIT_OR__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_OR__Byte__Byte
	.align	16, 0x90
___operator_BIT_OR__Byte__Byte:         # @__operator_BIT_OR__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	orb	8(%esp), %al
	ret

	.def	 ___operator_BIT_AND__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_AND__Byte__Byte
	.align	16, 0x90
___operator_BIT_AND__Byte__Byte:        # @__operator_BIT_AND__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	andb	8(%esp), %al
	ret

	.def	 ___operator_BIT_XOR__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_XOR__Byte__Byte
	.align	16, 0x90
___operator_BIT_XOR__Byte__Byte:        # @__operator_BIT_XOR__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	xorb	8(%esp), %al
	ret

	.def	 ___operator_EQ__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__Byte__Byte
	.align	16, 0x90
___operator_EQ__Byte__Byte:             # @__operator_EQ__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	cmpb	8(%esp), %al
	sete	%al
	ret

	.def	 ___operator_NE__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__Byte__Byte
	.align	16, 0x90
___operator_NE__Byte__Byte:             # @__operator_NE__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	cmpb	8(%esp), %al
	setne	%al
	ret

	.def	 ___operator_GT__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GT__Byte__Byte
	.align	16, 0x90
___operator_GT__Byte__Byte:             # @__operator_GT__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	cmpb	8(%esp), %al
	seta	%al
	ret

	.def	 ___operator_GE__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GE__Byte__Byte
	.align	16, 0x90
___operator_GE__Byte__Byte:             # @__operator_GE__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	cmpb	8(%esp), %al
	setae	%al
	ret

	.def	 ___operator_LT__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LT__Byte__Byte
	.align	16, 0x90
___operator_LT__Byte__Byte:             # @__operator_LT__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	cmpb	8(%esp), %al
	setb	%al
	ret

	.def	 ___operator_LE__Byte__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LE__Byte__Byte
	.align	16, 0x90
___operator_LE__Byte__Byte:             # @__operator_LE__Byte__Byte
# BB#0:                                 # %entry
	movb	4(%esp), %al
	cmpb	8(%esp), %al
	setbe	%al
	ret

	.def	 ___method_dataSize__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Byte
	.align	16, 0x90
___method_dataSize__Byte:               # @__method_dataSize__Byte
# BB#0:                                 # %entry
	movl	$1, %eax
	ret

	.def	 ___method_data__Byte;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Byte
	.align	16, 0x90
___method_data__Byte:                   # @__method_data__Byte
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___construct_Boolean__Data;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Data
	.align	16, 0x90
___construct_Boolean__Data:             # @__construct_Boolean__Data
# BB#0:                                 # %entry
	cmpl	$0, 8(%esp)
	movl	4(%esp), %eax
	setne	(%eax)
	ret

	.def	 ___construct_String__Data;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Data
	.align	16, 0x90
___construct_String__Data:              # @__construct_String__Data
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp12:
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Data__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___method_dataSize__Data;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Data
	.align	16, 0x90
___method_dataSize__Data:               # @__method_dataSize__Data
# BB#0:                                 # %entry
	movl	$4, %eax
	ret

	.def	 ___method_data__Data;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Data
	.align	16, 0x90
___method_data__Data:                   # @__method_data__Data
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___construct_Boolean__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Float64
	.align	16, 0x90
___construct_Boolean__Float64:          # @__construct_Boolean__Float64
# BB#0:                                 # %entry
	pxor	%xmm0, %xmm0
	ucomisd	8(%esp), %xmm0
	movl	4(%esp), %eax
	setne	(%eax)
	ret

	.def	 ___construct_Byte__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Byte__Float64
	.align	16, 0x90
___construct_Byte__Float64:             # @__construct_Byte__Float64
# BB#0:                                 # %entry
	cvttsd2si	8(%esp), %eax
	movl	4(%esp), %ecx
	movb	%al, (%ecx)
	ret

	.def	 ___construct_Integer__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Integer__Float64
	.align	16, 0x90
___construct_Integer__Float64:          # @__construct_Integer__Float64
# BB#0:                                 # %entry
	cvttsd2si	8(%esp), %eax
	movl	4(%esp), %ecx
	movl	%eax, (%ecx)
	ret

	.def	 ___construct_Size__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Size__Float64
	.align	16, 0x90
___construct_Size__Float64:             # @__construct_Size__Float64
# BB#0:                                 # %entry
	subl	$20, %esp
Ltmp13:
	movsd	28(%esp), %xmm0
	movsd	%xmm0, 8(%esp)
	fldl	8(%esp)
	fisttpll	(%esp)
	movl	24(%esp), %eax
	movl	(%esp), %ecx
	movl	%ecx, (%eax)
	addl	$20, %esp
	ret

	.def	 ___construct_Scalar__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Scalar__Float64
	.align	16, 0x90
___construct_Scalar__Float64:           # @__construct_Scalar__Float64
# BB#0:                                 # %entry
	movsd	8(%esp), %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movl	4(%esp), %eax
	movss	%xmm0, (%eax)
	ret

	.def	 ___construct_String__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Float64
	.align	16, 0x90
___construct_String__Float64:           # @__construct_String__Float64
# BB#0:                                 # %entry
	subl	$20, %esp
Ltmp14:
	movsd	28(%esp), %xmm0
	movsd	%xmm0, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Float64__Adapter, 4(%esp)
	calll	___String__Cast
	movl	24(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$20, %esp
	ret

	.def	 ___operator_POS__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POS__Float64
	.align	16, 0x90
___operator_POS__Float64:               # @__operator_POS__Float64
# BB#0:                                 # %entry
	fldl	4(%esp)
	ret

	.def	 ___operator_NEG__Float64;
	.scl	2;
	.type	32;
	.endef
	.section	.rdata,"r"
	.align	16
LCPI53_0:
	.quad	-9223372036854775808    # double -0.000000e+00
	.quad	-9223372036854775808    # double -0.000000e+00
	.text
	.globl	___operator_NEG__Float64
	.align	16, 0x90
___operator_NEG__Float64:               # @__operator_NEG__Float64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp15:
	movsd	16(%esp), %xmm0
	xorpd	LCPI53_0, %xmm0
	movsd	%xmm0, (%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 _sin64;
	.scl	2;
	.type	32;
	.endef
	.globl	_sin64
	.align	16, 0x90
_sin64:                                 # @sin64
# BB#0:                                 # %entry
	jmp	_sin                    # TAILCALL

	.def	 _cos64;
	.scl	2;
	.type	32;
	.endef
	.globl	_cos64
	.align	16, 0x90
_cos64:                                 # @cos64
# BB#0:                                 # %entry
	jmp	_cos                    # TAILCALL

	.def	 _tan64;
	.scl	2;
	.type	32;
	.endef
	.globl	_tan64
	.align	16, 0x90
_tan64:                                 # @tan64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp16:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_tan
	addl	$12, %esp
	ret

	.def	 _log64;
	.scl	2;
	.type	32;
	.endef
	.globl	_log64
	.align	16, 0x90
_log64:                                 # @log64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp17:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_log
	addl	$12, %esp
	ret

	.def	 _acos64;
	.scl	2;
	.type	32;
	.endef
	.globl	_acos64
	.align	16, 0x90
_acos64:                                # @acos64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp18:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_acos
	addl	$12, %esp
	ret

	.def	 _asin64;
	.scl	2;
	.type	32;
	.endef
	.globl	_asin64
	.align	16, 0x90
_asin64:                                # @asin64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp19:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_asin
	addl	$12, %esp
	ret

	.def	 _atan64;
	.scl	2;
	.type	32;
	.endef
	.globl	_atan64
	.align	16, 0x90
_atan64:                                # @atan64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp20:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_atan
	addl	$12, %esp
	ret

	.def	 _atan264;
	.scl	2;
	.type	32;
	.endef
	.globl	_atan264
	.align	16, 0x90
_atan264:                               # @atan264
# BB#0:                                 # %entry
	subl	$20, %esp
Ltmp21:
	movsd	32(%esp), %xmm0
	movsd	%xmm0, 8(%esp)
	movsd	24(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_atan2
	addl	$20, %esp
	ret

	.def	 _sqrt64;
	.scl	2;
	.type	32;
	.endef
	.globl	_sqrt64
	.align	16, 0x90
_sqrt64:                                # @sqrt64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp22:
	movsd	16(%esp), %xmm0
	sqrtsd	%xmm0, %xmm0
	movsd	%xmm0, (%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 _abs64;
	.scl	2;
	.type	32;
	.endef
	.globl	_abs64
	.align	16, 0x90
_abs64:                                 # @abs64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp23:
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	movl	$2147483647, %eax       # imm = 0x7FFFFFFF
	andl	20(%esp), %eax
	movl	%eax, 4(%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 _round64;
	.scl	2;
	.type	32;
	.endef
	.globl	_round64
	.align	16, 0x90
_round64:                               # @round64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp24:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_round
	addl	$12, %esp
	ret

	.def	 _ceil64;
	.scl	2;
	.type	32;
	.endef
	.globl	_ceil64
	.align	16, 0x90
_ceil64:                                # @ceil64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp25:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_ceil
	addl	$12, %esp
	ret

	.def	 _floor64;
	.scl	2;
	.type	32;
	.endef
	.globl	_floor64
	.align	16, 0x90
_floor64:                               # @floor64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp26:
	movsd	16(%esp), %xmm0
	movsd	%xmm0, (%esp)
	calll	_fp64_floor
	addl	$12, %esp
	ret

	.def	 _pow64;
	.scl	2;
	.type	32;
	.endef
	.globl	_pow64
	.align	16, 0x90
_pow64:                                 # @pow64
# BB#0:                                 # %entry
	jmp	_pow                    # TAILCALL

	.def	 ___operator_ADD__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__Float64__Float64
	.align	16, 0x90
___operator_ADD__Float64__Float64:      # @__operator_ADD__Float64__Float64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp27:
	movsd	16(%esp), %xmm0
	addsd	24(%esp), %xmm0
	movsd	%xmm0, (%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 ___operator_SUB__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_SUB__Float64__Float64
	.align	16, 0x90
___operator_SUB__Float64__Float64:      # @__operator_SUB__Float64__Float64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp28:
	movsd	16(%esp), %xmm0
	subsd	24(%esp), %xmm0
	movsd	%xmm0, (%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 ___operator_MUL__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MUL__Float64__Float64
	.align	16, 0x90
___operator_MUL__Float64__Float64:      # @__operator_MUL__Float64__Float64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp29:
	movsd	16(%esp), %xmm0
	mulsd	24(%esp), %xmm0
	movsd	%xmm0, (%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 ___operator_DIV__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_DIV__Float64__Float64
	.align	16, 0x90
___operator_DIV__Float64__Float64:      # @__operator_DIV__Float64__Float64
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp30:
	movsd	16(%esp), %xmm0
	divsd	24(%esp), %xmm0
	movsd	%xmm0, (%esp)
	fldl	(%esp)
	addl	$12, %esp
	ret

	.def	 ___operator_MOD__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MOD__Float64__Float64
	.align	16, 0x90
___operator_MOD__Float64__Float64:      # @__operator_MOD__Float64__Float64
# BB#0:                                 # %entry
	jmp	_fmod                   # TAILCALL

	.def	 ___operator_EQ__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__Float64__Float64
	.align	16, 0x90
___operator_EQ__Float64__Float64:       # @__operator_EQ__Float64__Float64
# BB#0:                                 # %entry
	movsd	4(%esp), %xmm0
	ucomisd	12(%esp), %xmm0
	setnp	%cl
	sete	%al
	andb	%cl, %al
	ret

	.def	 ___operator_NE__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__Float64__Float64
	.align	16, 0x90
___operator_NE__Float64__Float64:       # @__operator_NE__Float64__Float64
# BB#0:                                 # %entry
	movsd	4(%esp), %xmm0
	ucomisd	12(%esp), %xmm0
	setne	%al
	ret

	.def	 ___operator_GT__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GT__Float64__Float64
	.align	16, 0x90
___operator_GT__Float64__Float64:       # @__operator_GT__Float64__Float64
# BB#0:                                 # %entry
	movsd	4(%esp), %xmm0
	ucomisd	12(%esp), %xmm0
	seta	%al
	ret

	.def	 ___operator_GE__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GE__Float64__Float64
	.align	16, 0x90
___operator_GE__Float64__Float64:       # @__operator_GE__Float64__Float64
# BB#0:                                 # %entry
	movsd	4(%esp), %xmm0
	ucomisd	12(%esp), %xmm0
	setae	%al
	ret

	.def	 ___operator_LT__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LT__Float64__Float64
	.align	16, 0x90
___operator_LT__Float64__Float64:       # @__operator_LT__Float64__Float64
# BB#0:                                 # %entry
	movsd	12(%esp), %xmm0
	ucomisd	4(%esp), %xmm0
	seta	%al
	ret

	.def	 ___operator_LE__Float64__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LE__Float64__Float64
	.align	16, 0x90
___operator_LE__Float64__Float64:       # @__operator_LE__Float64__Float64
# BB#0:                                 # %entry
	movsd	12(%esp), %xmm0
	ucomisd	4(%esp), %xmm0
	setae	%al
	ret

	.def	 ___method_dataSize__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Float64
	.align	16, 0x90
___method_dataSize__Float64:            # @__method_dataSize__Float64
# BB#0:                                 # %entry
	movl	$8, %eax
	ret

	.def	 ___method_data__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Float64
	.align	16, 0x90
___method_data__Float64:                # @__method_data__Float64
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___construct_Boolean__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Integer
	.align	16, 0x90
___construct_Boolean__Integer:          # @__construct_Boolean__Integer
# BB#0:                                 # %entry
	cmpl	$0, 8(%esp)
	movl	4(%esp), %eax
	setne	(%eax)
	ret

	.def	 ___construct_Byte__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Byte__Integer
	.align	16, 0x90
___construct_Byte__Integer:             # @__construct_Byte__Integer
# BB#0:                                 # %entry
	movb	8(%esp), %al
	movl	4(%esp), %ecx
	movb	%al, (%ecx)
	ret

	.def	 ___construct_Size__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Size__Integer
	.align	16, 0x90
___construct_Size__Integer:             # @__construct_Size__Integer
# BB#0:                                 # %entry
	movl	8(%esp), %eax
	movl	4(%esp), %ecx
	movl	%eax, (%ecx)
	ret

	.def	 ___construct_Scalar__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Scalar__Integer
	.align	16, 0x90
___construct_Scalar__Integer:           # @__construct_Scalar__Integer
# BB#0:                                 # %entry
	cvtsi2ss	8(%esp), %xmm0
	movl	4(%esp), %eax
	movss	%xmm0, (%eax)
	ret

	.def	 ___construct_String__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Integer
	.align	16, 0x90
___construct_String__Integer:           # @__construct_String__Integer
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp31:
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Integer__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___operator_POS__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POS__Integer
	.align	16, 0x90
___operator_POS__Integer:               # @__operator_POS__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___operator_NEG__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NEG__Integer
	.align	16, 0x90
___operator_NEG__Integer:               # @__operator_NEG__Integer
# BB#0:                                 # %entry
	xorl	%eax, %eax
	subl	4(%esp), %eax
	ret

	.def	 ___operator_BIT_NOT__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_NOT__Integer
	.align	16, 0x90
___operator_BIT_NOT__Integer:           # @__operator_BIT_NOT__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	notl	%eax
	ret

	.def	 ___operator_PRE_INC__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_PRE_INC__Integer
	.align	16, 0x90
___operator_PRE_INC__Integer:           # @__operator_PRE_INC__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	incl	%eax
	movl	%eax, (%ecx)
	ret

	.def	 ___operator_PRE_DEC__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_PRE_DEC__Integer
	.align	16, 0x90
___operator_PRE_DEC__Integer:           # @__operator_PRE_DEC__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	decl	%eax
	movl	%eax, (%ecx)
	ret

	.def	 ___operator_POST_INC__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POST_INC__Integer
	.align	16, 0x90
___operator_POST_INC__Integer:          # @__operator_POST_INC__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	leal	1(%eax), %edx
	movl	%edx, (%ecx)
	ret

	.def	 ___operator_POST_DEC__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POST_DEC__Integer
	.align	16, 0x90
___operator_POST_DEC__Integer:          # @__operator_POST_DEC__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	leal	-1(%eax), %edx
	movl	%edx, (%ecx)
	ret

	.def	 ___operator_ADD__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__Integer__Integer
	.align	16, 0x90
___operator_ADD__Integer__Integer:      # @__operator_ADD__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	addl	8(%esp), %eax
	ret

	.def	 ___operator_SUB__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_SUB__Integer__Integer
	.align	16, 0x90
___operator_SUB__Integer__Integer:      # @__operator_SUB__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	subl	8(%esp), %eax
	ret

	.def	 ___operator_MUL__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MUL__Integer__Integer
	.align	16, 0x90
___operator_MUL__Integer__Integer:      # @__operator_MUL__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	imull	8(%esp), %eax
	ret

	.def	 ___operator_DIV__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_DIV__Integer__Integer
	.align	16, 0x90
___operator_DIV__Integer__Integer:      # @__operator_DIV__Integer__Integer
# BB#0:                                 # %entry
	pushl	%esi
Ltmp32:
	subl	$16, %esp
Ltmp33:
	movl	28(%esp), %ecx
	testl	%ecx, %ecx
	je	LBB96_3
# BB#1:                                 # %nonZero
	movl	24(%esp), %eax
	cltd
	idivl	%ecx
LBB96_2:                                # %nonZero
	addl	$16, %esp
	popl	%esi
	ret
LBB96_3:                                # %zero
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_5, 4(%esp)
	calll	___construct_String__ConstString28
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	xorl	%eax, %eax
	jmp	LBB96_2

	.def	 ___construct_Boolean__ConstString28;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__ConstString28
	.align	16, 0x90
___construct_Boolean__ConstString28:    # @__construct_Boolean__ConstString28
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	movb	$1, (%eax)
	ret

	.def	 ___construct_String__ConstString28;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__ConstString28
	.align	16, 0x90
___construct_String__ConstString28:     # @__construct_String__ConstString28
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp34:
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	movl	$___ConstString28__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___operator_MOD__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MOD__Integer__Integer
	.align	16, 0x90
___operator_MOD__Integer__Integer:      # @__operator_MOD__Integer__Integer
# BB#0:                                 # %entry
	pushl	%esi
Ltmp35:
	subl	$16, %esp
Ltmp36:
	movl	28(%esp), %ecx
	testl	%ecx, %ecx
	je	LBB99_3
# BB#1:                                 # %nonZero
	movl	24(%esp), %eax
	cltd
	idivl	%ecx
	movl	%edx, %eax
LBB99_2:                                # %nonZero
	addl	$16, %esp
	popl	%esi
	ret
LBB99_3:                                # %zero
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_6, 4(%esp)
	calll	___construct_String__ConstString28
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	xorl	%eax, %eax
	jmp	LBB99_2

	.def	 ___operator_BIT_OR__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_OR__Integer__Integer
	.align	16, 0x90
___operator_BIT_OR__Integer__Integer:   # @__operator_BIT_OR__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	orl	8(%esp), %eax
	ret

	.def	 ___operator_BIT_AND__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_AND__Integer__Integer
	.align	16, 0x90
___operator_BIT_AND__Integer__Integer:  # @__operator_BIT_AND__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	andl	8(%esp), %eax
	ret

	.def	 ___operator_BIT_XOR__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_XOR__Integer__Integer
	.align	16, 0x90
___operator_BIT_XOR__Integer__Integer:  # @__operator_BIT_XOR__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	xorl	8(%esp), %eax
	ret

	.def	 ___operator_EQ__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__Integer__Integer
	.align	16, 0x90
___operator_EQ__Integer__Integer:       # @__operator_EQ__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	sete	%al
	ret

	.def	 ___operator_NE__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__Integer__Integer
	.align	16, 0x90
___operator_NE__Integer__Integer:       # @__operator_NE__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setne	%al
	ret

	.def	 ___operator_GT__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GT__Integer__Integer
	.align	16, 0x90
___operator_GT__Integer__Integer:       # @__operator_GT__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setg	%al
	ret

	.def	 ___operator_GE__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GE__Integer__Integer
	.align	16, 0x90
___operator_GE__Integer__Integer:       # @__operator_GE__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setge	%al
	ret

	.def	 ___operator_LT__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LT__Integer__Integer
	.align	16, 0x90
___operator_LT__Integer__Integer:       # @__operator_LT__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setl	%al
	ret

	.def	 ___operator_LE__Integer__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LE__Integer__Integer
	.align	16, 0x90
___operator_LE__Integer__Integer:       # @__operator_LE__Integer__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setle	%al
	ret

	.def	 ___method_dataSize__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Integer
	.align	16, 0x90
___method_dataSize__Integer:            # @__method_dataSize__Integer
# BB#0:                                 # %entry
	movl	$4, %eax
	ret

	.def	 ___method_data__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Integer
	.align	16, 0x90
___method_data__Integer:                # @__method_data__Integer
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___construct_Boolean__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Scalar
	.align	16, 0x90
___construct_Boolean__Scalar:           # @__construct_Boolean__Scalar
# BB#0:                                 # %entry
	pxor	%xmm0, %xmm0
	ucomiss	8(%esp), %xmm0
	movl	4(%esp), %eax
	setne	(%eax)
	ret

	.def	 ___construct_Byte__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Byte__Scalar
	.align	16, 0x90
___construct_Byte__Scalar:              # @__construct_Byte__Scalar
# BB#0:                                 # %entry
	cvttss2si	8(%esp), %eax
	movl	4(%esp), %ecx
	movb	%al, (%ecx)
	ret

	.def	 ___construct_Integer__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Integer__Scalar
	.align	16, 0x90
___construct_Integer__Scalar:           # @__construct_Integer__Scalar
# BB#0:                                 # %entry
	cvttss2si	8(%esp), %eax
	movl	4(%esp), %ecx
	movl	%eax, (%ecx)
	ret

	.def	 ___construct_Size__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Size__Scalar
	.align	16, 0x90
___construct_Size__Scalar:              # @__construct_Size__Scalar
# BB#0:                                 # %entry
	subl	$20, %esp
Ltmp37:
	movss	28(%esp), %xmm0
	movss	%xmm0, 8(%esp)
	flds	8(%esp)
	fisttpll	(%esp)
	movl	24(%esp), %eax
	movl	(%esp), %ecx
	movl	%ecx, (%eax)
	addl	$20, %esp
	ret

	.def	 ___construct_Float64__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Float64__Scalar
	.align	16, 0x90
___construct_Float64__Scalar:           # @__construct_Float64__Scalar
# BB#0:                                 # %entry
	movss	8(%esp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movl	4(%esp), %eax
	movsd	%xmm0, (%eax)
	ret

	.def	 ___construct_String__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Scalar
	.align	16, 0x90
___construct_String__Scalar:            # @__construct_String__Scalar
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp38:
	movss	20(%esp), %xmm0
	movss	%xmm0, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Scalar__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___operator_POS__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POS__Scalar
	.align	16, 0x90
___operator_POS__Scalar:                # @__operator_POS__Scalar
# BB#0:                                 # %entry
	flds	4(%esp)
	ret

	.def	 ___operator_NEG__Scalar;
	.scl	2;
	.type	32;
	.endef
	.section	.rdata,"r"
	.align	16
LCPI118_0:
	.long	2147483648              # float -0.000000e+00
	.long	2147483648              # float -0.000000e+00
	.long	2147483648              # float -0.000000e+00
	.long	2147483648              # float -0.000000e+00
	.text
	.globl	___operator_NEG__Scalar
	.align	16, 0x90
___operator_NEG__Scalar:                # @__operator_NEG__Scalar
# BB#0:                                 # %entry
	pushl	%eax
Ltmp39:
	movss	8(%esp), %xmm0
	xorps	LCPI118_0, %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 _sin;
	.scl	2;
	.type	32;
	.endef
	.globl	_sin
	.align	16, 0x90
_sin:                                   # @sin
# BB#0:                                 # %entry
	jmp	_sinf                   # TAILCALL

	.def	 _cos;
	.scl	2;
	.type	32;
	.endef
	.globl	_cos
	.align	16, 0x90
_cos:                                   # @cos
# BB#0:                                 # %entry
	jmp	_cosf                   # TAILCALL

	.def	 _tan;
	.scl	2;
	.type	32;
	.endef
	.globl	_tan
	.align	16, 0x90
_tan:                                   # @tan
# BB#0:                                 # %entry
	pushl	%eax
Ltmp40:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_tan
	popl	%eax
	ret

	.def	 _log;
	.scl	2;
	.type	32;
	.endef
	.globl	_log
	.align	16, 0x90
_log:                                   # @log
# BB#0:                                 # %entry
	pushl	%eax
Ltmp41:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_log
	popl	%eax
	ret

	.def	 _acos;
	.scl	2;
	.type	32;
	.endef
	.globl	_acos
	.align	16, 0x90
_acos:                                  # @acos
# BB#0:                                 # %entry
	pushl	%eax
Ltmp42:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_acos
	popl	%eax
	ret

	.def	 _asin;
	.scl	2;
	.type	32;
	.endef
	.globl	_asin
	.align	16, 0x90
_asin:                                  # @asin
# BB#0:                                 # %entry
	pushl	%eax
Ltmp43:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_asin
	popl	%eax
	ret

	.def	 _atan;
	.scl	2;
	.type	32;
	.endef
	.globl	_atan
	.align	16, 0x90
_atan:                                  # @atan
# BB#0:                                 # %entry
	pushl	%eax
Ltmp44:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_atan
	popl	%eax
	ret

	.def	 _atan2;
	.scl	2;
	.type	32;
	.endef
	.globl	_atan2
	.align	16, 0x90
_atan2:                                 # @atan2
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp45:
	movss	20(%esp), %xmm0
	movss	%xmm0, 4(%esp)
	movss	16(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_atan2
	addl	$12, %esp
	ret

	.def	 _sqrt;
	.scl	2;
	.type	32;
	.endef
	.globl	_sqrt
	.align	16, 0x90
_sqrt:                                  # @sqrt
# BB#0:                                 # %entry
	pushl	%eax
Ltmp46:
	movss	8(%esp), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 _abs;
	.scl	2;
	.type	32;
	.endef
	.globl	_abs
	.align	16, 0x90
_abs:                                   # @abs
# BB#0:                                 # %entry
	pushl	%eax
Ltmp47:
	movl	$2147483647, %eax       # imm = 0x7FFFFFFF
	andl	8(%esp), %eax
	movd	%eax, %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 _round;
	.scl	2;
	.type	32;
	.endef
	.globl	_round
	.align	16, 0x90
_round:                                 # @round
# BB#0:                                 # %entry
	pushl	%eax
Ltmp48:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_round
	popl	%eax
	ret

	.def	 _ceil;
	.scl	2;
	.type	32;
	.endef
	.globl	_ceil
	.align	16, 0x90
_ceil:                                  # @ceil
# BB#0:                                 # %entry
	pushl	%eax
Ltmp49:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_ceil
	popl	%eax
	ret

	.def	 _floor;
	.scl	2;
	.type	32;
	.endef
	.globl	_floor
	.align	16, 0x90
_floor:                                 # @floor
# BB#0:                                 # %entry
	pushl	%eax
Ltmp50:
	movss	8(%esp), %xmm0
	movss	%xmm0, (%esp)
	calll	_fp32_floor
	popl	%eax
	ret

	.def	 _pow;
	.scl	2;
	.type	32;
	.endef
	.globl	_pow
	.align	16, 0x90
_pow:                                   # @pow
# BB#0:                                 # %entry
	jmp	_powf                   # TAILCALL

	.def	 ___operator_ADD__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__Scalar__Scalar
	.align	16, 0x90
___operator_ADD__Scalar__Scalar:        # @__operator_ADD__Scalar__Scalar
# BB#0:                                 # %entry
	pushl	%eax
Ltmp51:
	movss	8(%esp), %xmm0
	addss	12(%esp), %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 ___operator_SUB__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_SUB__Scalar__Scalar
	.align	16, 0x90
___operator_SUB__Scalar__Scalar:        # @__operator_SUB__Scalar__Scalar
# BB#0:                                 # %entry
	pushl	%eax
Ltmp52:
	movss	8(%esp), %xmm0
	subss	12(%esp), %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 ___operator_MUL__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MUL__Scalar__Scalar
	.align	16, 0x90
___operator_MUL__Scalar__Scalar:        # @__operator_MUL__Scalar__Scalar
# BB#0:                                 # %entry
	pushl	%eax
Ltmp53:
	movss	8(%esp), %xmm0
	mulss	12(%esp), %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 ___operator_DIV__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_DIV__Scalar__Scalar
	.align	16, 0x90
___operator_DIV__Scalar__Scalar:        # @__operator_DIV__Scalar__Scalar
# BB#0:                                 # %entry
	pushl	%eax
Ltmp54:
	movss	8(%esp), %xmm0
	divss	12(%esp), %xmm0
	movss	%xmm0, (%esp)
	flds	(%esp)
	popl	%eax
	ret

	.def	 ___operator_MOD__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MOD__Scalar__Scalar
	.align	16, 0x90
___operator_MOD__Scalar__Scalar:        # @__operator_MOD__Scalar__Scalar
# BB#0:                                 # %entry
	jmp	_fmodf                  # TAILCALL

	.def	 ___operator_EQ__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__Scalar__Scalar
	.align	16, 0x90
___operator_EQ__Scalar__Scalar:         # @__operator_EQ__Scalar__Scalar
# BB#0:                                 # %entry
	movss	4(%esp), %xmm0
	ucomiss	8(%esp), %xmm0
	setnp	%cl
	sete	%al
	andb	%cl, %al
	ret

	.def	 ___operator_NE__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__Scalar__Scalar
	.align	16, 0x90
___operator_NE__Scalar__Scalar:         # @__operator_NE__Scalar__Scalar
# BB#0:                                 # %entry
	movss	4(%esp), %xmm0
	ucomiss	8(%esp), %xmm0
	setne	%al
	ret

	.def	 ___operator_GT__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GT__Scalar__Scalar
	.align	16, 0x90
___operator_GT__Scalar__Scalar:         # @__operator_GT__Scalar__Scalar
# BB#0:                                 # %entry
	movss	4(%esp), %xmm0
	ucomiss	8(%esp), %xmm0
	seta	%al
	ret

	.def	 ___operator_GE__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GE__Scalar__Scalar
	.align	16, 0x90
___operator_GE__Scalar__Scalar:         # @__operator_GE__Scalar__Scalar
# BB#0:                                 # %entry
	movss	4(%esp), %xmm0
	ucomiss	8(%esp), %xmm0
	setae	%al
	ret

	.def	 ___operator_LT__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LT__Scalar__Scalar
	.align	16, 0x90
___operator_LT__Scalar__Scalar:         # @__operator_LT__Scalar__Scalar
# BB#0:                                 # %entry
	movss	8(%esp), %xmm0
	ucomiss	4(%esp), %xmm0
	seta	%al
	ret

	.def	 ___operator_LE__Scalar__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LE__Scalar__Scalar
	.align	16, 0x90
___operator_LE__Scalar__Scalar:         # @__operator_LE__Scalar__Scalar
# BB#0:                                 # %entry
	movss	8(%esp), %xmm0
	ucomiss	4(%esp), %xmm0
	setae	%al
	ret

	.def	 ___method_dataSize__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Scalar
	.align	16, 0x90
___method_dataSize__Scalar:             # @__method_dataSize__Scalar
# BB#0:                                 # %entry
	movl	$4, %eax
	ret

	.def	 ___method_data__Scalar;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Scalar
	.align	16, 0x90
___method_data__Scalar:                 # @__method_data__Scalar
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___construct_Boolean__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Size
	.align	16, 0x90
___construct_Boolean__Size:             # @__construct_Boolean__Size
# BB#0:                                 # %entry
	cmpl	$0, 8(%esp)
	movl	4(%esp), %eax
	setne	(%eax)
	ret

	.def	 ___construct_Byte__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Byte__Size
	.align	16, 0x90
___construct_Byte__Size:                # @__construct_Byte__Size
# BB#0:                                 # %entry
	movb	8(%esp), %al
	movl	4(%esp), %ecx
	movb	%al, (%ecx)
	ret

	.def	 ___construct_Integer__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Integer__Size
	.align	16, 0x90
___construct_Integer__Size:             # @__construct_Integer__Size
# BB#0:                                 # %entry
	movl	8(%esp), %eax
	movl	4(%esp), %ecx
	movl	%eax, (%ecx)
	ret

	.def	 ___construct_Scalar__Size;
	.scl	2;
	.type	32;
	.endef
	.section	.rdata,"r"
	.align	8
LCPI149_0:
	.quad	4841369599423283200     # double 4.503600e+15
	.text
	.globl	___construct_Scalar__Size
	.align	16, 0x90
___construct_Scalar__Size:              # @__construct_Scalar__Size
# BB#0:                                 # %entry
	movsd	LCPI149_0, %xmm0
	movd	8(%esp), %xmm1
	orpd	%xmm0, %xmm1
	subsd	%xmm0, %xmm1
	cvtsd2ss	%xmm1, %xmm0
	movl	4(%esp), %eax
	movss	%xmm0, (%eax)
	ret

	.def	 ___construct_String__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Size
	.align	16, 0x90
___construct_String__Size:              # @__construct_String__Size
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp55:
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Size__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___operator_POS__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POS__Size
	.align	16, 0x90
___operator_POS__Size:                  # @__operator_POS__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___operator_BIT_NOT__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_NOT__Size
	.align	16, 0x90
___operator_BIT_NOT__Size:              # @__operator_BIT_NOT__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	notl	%eax
	ret

	.def	 ___operator_PRE_INC__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_PRE_INC__Size
	.align	16, 0x90
___operator_PRE_INC__Size:              # @__operator_PRE_INC__Size
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	incl	%eax
	movl	%eax, (%ecx)
	ret

	.def	 ___operator_PRE_DEC__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_PRE_DEC__Size
	.align	16, 0x90
___operator_PRE_DEC__Size:              # @__operator_PRE_DEC__Size
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	decl	%eax
	movl	%eax, (%ecx)
	ret

	.def	 ___operator_POST_INC__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POST_INC__Size
	.align	16, 0x90
___operator_POST_INC__Size:             # @__operator_POST_INC__Size
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	leal	1(%eax), %edx
	movl	%edx, (%ecx)
	ret

	.def	 ___operator_POST_DEC__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_POST_DEC__Size
	.align	16, 0x90
___operator_POST_DEC__Size:             # @__operator_POST_DEC__Size
# BB#0:                                 # %entry
	movl	4(%esp), %ecx
	movl	(%ecx), %eax
	leal	-1(%eax), %edx
	movl	%edx, (%ecx)
	ret

	.def	 ___operator_ADD__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__Size__Size
	.align	16, 0x90
___operator_ADD__Size__Size:            # @__operator_ADD__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	addl	8(%esp), %eax
	ret

	.def	 ___operator_SUB__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_SUB__Size__Size
	.align	16, 0x90
___operator_SUB__Size__Size:            # @__operator_SUB__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	subl	8(%esp), %eax
	ret

	.def	 ___operator_MUL__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MUL__Size__Size
	.align	16, 0x90
___operator_MUL__Size__Size:            # @__operator_MUL__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	imull	8(%esp), %eax
	ret

	.def	 ___operator_DIV__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_DIV__Size__Size
	.align	16, 0x90
___operator_DIV__Size__Size:            # @__operator_DIV__Size__Size
# BB#0:                                 # %entry
	pushl	%esi
Ltmp56:
	subl	$16, %esp
Ltmp57:
	movl	28(%esp), %ecx
	testl	%ecx, %ecx
	je	LBB160_3
# BB#1:                                 # %nonZero
	movl	24(%esp), %eax
	xorl	%edx, %edx
	divl	%ecx
LBB160_2:                               # %nonZero
	addl	$16, %esp
	popl	%esi
	ret
LBB160_3:                               # %zero
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_7, 4(%esp)
	calll	___construct_String__ConstString25
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	xorl	%eax, %eax
	jmp	LBB160_2

	.def	 ___operator_MOD__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_MOD__Size__Size
	.align	16, 0x90
___operator_MOD__Size__Size:            # @__operator_MOD__Size__Size
# BB#0:                                 # %entry
	pushl	%esi
Ltmp58:
	subl	$16, %esp
Ltmp59:
	movl	28(%esp), %ecx
	testl	%ecx, %ecx
	je	LBB161_3
# BB#1:                                 # %nonZero
	movl	24(%esp), %eax
	xorl	%edx, %edx
	divl	%ecx
	movl	%edx, %eax
LBB161_2:                               # %nonZero
	addl	$16, %esp
	popl	%esi
	ret
LBB161_3:                               # %zero
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_8, 4(%esp)
	calll	___construct_String__ConstString25
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	xorl	%eax, %eax
	jmp	LBB161_2

	.def	 ___operator_BIT_OR__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_OR__Size__Size
	.align	16, 0x90
___operator_BIT_OR__Size__Size:         # @__operator_BIT_OR__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	orl	8(%esp), %eax
	ret

	.def	 ___operator_BIT_AND__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_AND__Size__Size
	.align	16, 0x90
___operator_BIT_AND__Size__Size:        # @__operator_BIT_AND__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	andl	8(%esp), %eax
	ret

	.def	 ___operator_BIT_XOR__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_BIT_XOR__Size__Size
	.align	16, 0x90
___operator_BIT_XOR__Size__Size:        # @__operator_BIT_XOR__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	xorl	8(%esp), %eax
	ret

	.def	 ___operator_EQ__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__Size__Size
	.align	16, 0x90
___operator_EQ__Size__Size:             # @__operator_EQ__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	sete	%al
	ret

	.def	 ___operator_NE__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__Size__Size
	.align	16, 0x90
___operator_NE__Size__Size:             # @__operator_NE__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setne	%al
	ret

	.def	 ___operator_GT__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GT__Size__Size
	.align	16, 0x90
___operator_GT__Size__Size:             # @__operator_GT__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	seta	%al
	ret

	.def	 ___operator_GE__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GE__Size__Size
	.align	16, 0x90
___operator_GE__Size__Size:             # @__operator_GE__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setae	%al
	ret

	.def	 ___operator_LT__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LT__Size__Size
	.align	16, 0x90
___operator_LT__Size__Size:             # @__operator_LT__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setb	%al
	ret

	.def	 ___operator_LE__Size__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LE__Size__Size
	.align	16, 0x90
___operator_LE__Size__Size:             # @__operator_LE__Size__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	setbe	%al
	ret

	.def	 ___method_dataSize__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Size
	.align	16, 0x90
___method_dataSize__Size:               # @__method_dataSize__Size
# BB#0:                                 # %entry
	movl	$4, %eax
	ret

	.def	 ___method_data__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Size
	.align	16, 0x90
___method_data__Size:                   # @__method_data__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	ret

	.def	 ___String__Retain;
	.scl	2;
	.type	32;
	.endef
	.globl	___String__Retain
	.align	16, 0x90
___String__Retain:                      # @__String__Retain
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	LBB173_2
# BB#1:                                 # %addRef
	lock
	incl	(%eax)
LBB173_2:                               # %done
	ret

	.def	 ___String__RefCount;
	.scl	2;
	.type	32;
	.endef
	.globl	___String__RefCount
	.align	16, 0x90
___String__RefCount:                    # @__String__RefCount
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	LBB174_2
# BB#1:                                 # %nonNull
	movl	(%eax), %eax
	ret
LBB174_2:                               # %null
	xorl	%eax, %eax
	ret

	.def	 ___String__Length;
	.scl	2;
	.type	32;
	.endef
	.globl	___String__Length
	.align	16, 0x90
___String__Length:                      # @__String__Length
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	LBB175_2
# BB#1:                                 # %nonNull
	movl	8(%eax), %eax
	ret
LBB175_2:                               # %null
	xorl	%eax, %eax
	ret

	.def	 ___construct_Boolean__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__String
	.align	16, 0x90
___construct_Boolean__String:           # @__construct_Boolean__String
# BB#0:                                 # %entry
	pushl	%eax
Ltmp60:
	movl	12(%esp), %eax
	movl	%eax, (%esp)
	calll	___String__Length
	testl	%eax, %eax
	movl	8(%esp), %eax
	setne	(%eax)
	popl	%eax
	ret

	.def	 ___method_ASSIGN_OP_ADD__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_ASSIGN_OP_ADD__String__String
	.align	16, 0x90
___method_ASSIGN_OP_ADD__String__String: # @__method_ASSIGN_OP_ADD__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp61:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___String__Append
	addl	$12, %esp
	ret

	.def	 ___operator_ADD__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__String__String
	.align	16, 0x90
___operator_ADD__String__String:        # @__operator_ADD__String__String
# BB#0:                                 # %entry
	pushl	%esi
Ltmp62:
	subl	$16, %esp
Ltmp63:
	movl	$0, 12(%esp)
	movl	24(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Retain
	movl	12(%esp), %eax
	movl	%esi, 12(%esp)
	movl	%eax, (%esp)
	calll	___String__Release
	movl	28(%esp), %eax
	movl	%eax, 4(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_ASSIGN_OP_ADD__String__String
	movl	12(%esp), %eax
	addl	$16, %esp
	popl	%esi
	ret

	.def	 ___method_refCount__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_refCount__String
	.align	16, 0x90
___method_refCount__String:             # @__method_refCount__String
# BB#0:                                 # %entry
	pushl	%eax
Ltmp64:
	movl	8(%esp), %eax
	movl	%eax, (%esp)
	calll	___String__RefCount
	popl	%edx
	ret

	.def	 ___method_length__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_length__String
	.align	16, 0x90
___method_length__String:               # @__method_length__String
# BB#0:                                 # %entry
	pushl	%eax
Ltmp65:
	movl	8(%esp), %eax
	movl	%eax, (%esp)
	calll	___String__Length
	popl	%edx
	ret

	.def	 ___method_dataSize__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__String
	.align	16, 0x90
___method_dataSize__String:             # @__method_dataSize__String
# BB#0:                                 # %entry
	pushl	%eax
Ltmp66:
	movl	8(%esp), %eax
	movl	%eax, (%esp)
	calll	___String__Length
	incl	%eax
	popl	%edx
	ret

	.def	 ___method_data__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__String
	.align	16, 0x90
___method_data__String:                 # @__method_data__String
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	jne	LBB182_2
# BB#1:                                 # %null
	ret
LBB182_2:                               # %nonNull
	addl	$12, %eax
	ret

	.def	 ___method_compare__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_compare__String__String
	.align	16, 0x90
___method_compare__String__String:      # @__method_compare__String__String
# BB#0:                                 # %entry
	pushl	%ebp
Ltmp67:
	movl	%esp, %ebp
Ltmp68:
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	pushl	%eax
Ltmp69:
	cmpl	$0, 8(%ebp)
	jne	LBB183_3
# BB#1:                                 # %selfIsNull
	cmpl	$0, 12(%ebp)
	je	LBB183_8
LBB183_2:                               # %lt
	movl	$-1, %eax
	jmp	LBB183_9
LBB183_3:                               # %selfIsNotNull
	cmpl	$0, 12(%ebp)
	je	LBB183_14
# BB#4:                                 # %shallow
	movl	12(%ebp), %eax
	cmpl	%eax, 8(%ebp)
	je	LBB183_8
# BB#5:                                 # %deep
	movl	8(%eax), %esi
	movl	8(%ebp), %eax
	movl	8(%eax), %edi
	movl	$8, %eax
	calll	__chkstk
	movl	%esp, %eax
	movl	$0, (%eax)
	jmp	LBB183_6
	.align	16, 0x90
LBB183_10:                              # %checkOtherLength
                                        #   in Loop: Header=BB183_6 Depth=1
	cmpl	%esi, %ecx
	jae	LBB183_14
# BB#11:                                # %checkChars
                                        #   in Loop: Header=BB183_6 Depth=1
	movl	12(%ebp), %edx
	movb	12(%edx,%ecx), %dl
	movl	8(%ebp), %ebx
	movb	12(%ebx,%ecx), %dh
	cmpb	%dl, %dh
	jb	LBB183_2
# BB#12:                                # %checkCharsGT
                                        #   in Loop: Header=BB183_6 Depth=1
	cmpb	%dl, %dh
	ja	LBB183_14
# BB#13:                                # %next
                                        #   in Loop: Header=BB183_6 Depth=1
	incl	%ecx
	movl	%ecx, (%eax)
LBB183_6:                               # %check
                                        # =>This Inner Loop Header: Depth=1
	movl	(%eax), %ecx
	cmpl	%edi, %ecx
	jb	LBB183_10
# BB#7:                                 # %verifyOtherLength
	cmpl	%esi, %ecx
	jb	LBB183_2
LBB183_8:                               # %eq
	xorl	%eax, %eax
LBB183_9:                               # %eq
	leal	-12(%ebp), %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret
LBB183_14:                              # %gt
	movl	$1, %eax
	jmp	LBB183_9

	.def	 ___operator_EQ__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_EQ__String__String
	.align	16, 0x90
___operator_EQ__String__String:         # @__operator_EQ__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp70:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_compare__String__String
	testl	%eax, %eax
	sete	%al
	addl	$12, %esp
	ret

	.def	 ___operator_NE__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_NE__String__String
	.align	16, 0x90
___operator_NE__String__String:         # @__operator_NE__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp71:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_compare__String__String
	testl	%eax, %eax
	setne	%al
	addl	$12, %esp
	ret

	.def	 ___operator_GT__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GT__String__String
	.align	16, 0x90
___operator_GT__String__String:         # @__operator_GT__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp72:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_compare__String__String
	testl	%eax, %eax
	setg	%al
	addl	$12, %esp
	ret

	.def	 ___operator_GE__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_GE__String__String
	.align	16, 0x90
___operator_GE__String__String:         # @__operator_GE__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp73:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_compare__String__String
	testl	%eax, %eax
	setns	%al
	addl	$12, %esp
	ret

	.def	 ___operator_LT__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LT__String__String
	.align	16, 0x90
___operator_LT__String__String:         # @__operator_LT__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp74:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_compare__String__String
	shrl	$31, %eax
	addl	$12, %esp
	ret

	.def	 ___operator_LE__String__String;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_LE__String__String
	.align	16, 0x90
___operator_LE__String__String:         # @__operator_LE__String__String
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp75:
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_compare__String__String
	testl	%eax, %eax
	setle	%al
	addl	$12, %esp
	ret

	.def	 ___Float64_VA__Retain;
	.scl	2;
	.type	32;
	.endef
	.globl	___Float64_VA__Retain
	.align	16, 0x90
___Float64_VA__Retain:                  # @__Float64_VA__Retain
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	LBB190_2
# BB#1:                                 # %nonNull
	lock
	incl	(%eax)
LBB190_2:                               # %done
	ret

	.def	 ___Float64_VA__Release;
	.scl	2;
	.type	32;
	.endef
	.globl	___Float64_VA__Release
	.align	16, 0x90
___Float64_VA__Release:                 # @__Float64_VA__Release
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp76:
	movl	16(%esp), %eax
	testl	%eax, %eax
	je	LBB191_6
# BB#1:                                 # %nonNull
	movl	$-1, %ecx
	lock
	xaddl	%ecx, (%eax)
	cmpl	$1, %ecx
	jne	LBB191_6
# BB#2:                                 # %free
	movl	8(%eax), %ecx
	movl	$0, 8(%esp)
	jmp	LBB191_3
	.align	16, 0x90
LBB191_4:                               # %loopExec
                                        #   in Loop: Header=BB191_3 Depth=1
	incl	8(%esp)
LBB191_3:                               # %loopCheck
                                        # =>This Inner Loop Header: Depth=1
	cmpl	%ecx, 8(%esp)
	jne	LBB191_4
# BB#5:                                 # %loopDone
	movl	%eax, (%esp)
	calll	_free
LBB191_6:                               # %done
	addl	$12, %esp
	ret

	.def	 ___Float64_VA__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___Float64_VA__Size
	.align	16, 0x90
___Float64_VA__Size:                    # @__Float64_VA__Size
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	LBB192_2
# BB#1:                                 # %nonNull
	movl	8(%eax), %eax
	ret
LBB192_2:                               # %null
	xorl	%eax, %eax
	ret

	.def	 ___Float64_VA__ConstIndex;
	.scl	2;
	.type	32;
	.endef
	.globl	___Float64_VA__ConstIndex
	.align	16, 0x90
___Float64_VA__ConstIndex:              # @__Float64_VA__ConstIndex
# BB#0:                                 # %entry
	pushl	%esi
Ltmp77:
	subl	$16, %esp
Ltmp78:
	movl	24(%esp), %eax
	testl	%eax, %eax
	je	LBB193_4
# BB#1:                                 # %notNull
	movl	28(%esp), %ecx
	cmpl	8(%eax), %ecx
	jae	LBB193_4
# BB#2:                                 # %inRange
	fldl	16(%eax,%ecx,8)
LBB193_3:                               # %inRange
	addl	$16, %esp
	popl	%esi
	ret
LBB193_4:                               # %outOfRange
	movl	$0, 12(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	movl	$___unnamed_9, 4(%esp)
	calll	___construct_String__ConstString33
	movl	12(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	fldz
	jmp	LBB193_3

	.def	 ___construct_Boolean__ConstString33;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__ConstString33
	.align	16, 0x90
___construct_Boolean__ConstString33:    # @__construct_Boolean__ConstString33
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	movb	$1, (%eax)
	ret

	.def	 ___construct_String__ConstString33;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__ConstString33
	.align	16, 0x90
___construct_String__ConstString33:     # @__construct_String__ConstString33
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp79:
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	movl	$___ConstString33__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___Float64_VA__NonConstIndex;
	.scl	2;
	.type	32;
	.endef
	.globl	___Float64_VA__NonConstIndex
	.align	16, 0x90
___Float64_VA__NonConstIndex:           # @__Float64_VA__NonConstIndex
# BB#0:                                 # %entry
	pushl	%edi
Ltmp80:
	pushl	%esi
Ltmp81:
	subl	$12, %esp
Ltmp82:
	movl	24(%esp), %esi
	movl	(%esi), %eax
	testl	%eax, %eax
	je	LBB196_6
# BB#1:                                 # %notNull
	movl	28(%esp), %edi
	cmpl	8(%eax), %edi
	jae	LBB196_6
# BB#2:                                 # %inRange
	cmpl	$1, (%eax)
	jbe	LBB196_4
# BB#3:                                 # %nonUnique
	movl	%esi, 4(%esp)
	movl	$___Float64_VA__Adapter, (%esp)
	calll	___Float64_VA__Split
LBB196_4:                               # %unique
	movl	(%esi), %eax
	leal	16(%eax,%edi,8), %eax
LBB196_5:                               # %unique
	addl	$12, %esp
	popl	%esi
	popl	%edi
	ret
LBB196_6:                               # %outOfRange
	movl	$0, 8(%esp)
	leal	8(%esp), %esi
	movl	%esi, (%esp)
	movl	$___unnamed_10, 4(%esp)
	calll	___construct_String__ConstString33
	movl	8(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	movl	$___Float64__DefaultValue, %eax
	jmp	LBB196_5

	.def	 ___construct_Boolean__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_Boolean__Float64_VA
	.align	16, 0x90
___construct_Boolean__Float64_VA:       # @__construct_Boolean__Float64_VA
# BB#0:                                 # %entry
	pushl	%eax
Ltmp83:
	movl	12(%esp), %eax
	movl	%eax, (%esp)
	calll	___Float64_VA__Size
	testl	%eax, %eax
	movl	8(%esp), %eax
	setne	(%eax)
	popl	%eax
	ret

	.def	 ___construct_String__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___construct_String__Float64_VA
	.align	16, 0x90
___construct_String__Float64_VA:        # @__construct_String__Float64_VA
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp84:
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	leal	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$___Float64_VA__Adapter, 4(%esp)
	calll	___String__Cast
	movl	16(%esp), %ecx
	movl	(%ecx), %edx
	movl	%eax, (%ecx)
	movl	%edx, (%esp)
	calll	___String__Release
	addl	$12, %esp
	ret

	.def	 ___method_ASSIGN_OP_ADD__Float64_VA__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_ASSIGN_OP_ADD__Float64_VA__Float64_VA
	.align	16, 0x90
___method_ASSIGN_OP_ADD__Float64_VA__Float64_VA: # @__method_ASSIGN_OP_ADD__Float64_VA__Float64_VA
# BB#0:                                 # %entry
	subl	$12, %esp
Ltmp85:
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	movl	16(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$___Float64_VA__Adapter, (%esp)
	calll	___Float64_VA__Append
	addl	$12, %esp
	ret

	.def	 ___operator_ADD__Float64_VA__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___operator_ADD__Float64_VA__Float64_VA
	.align	16, 0x90
___operator_ADD__Float64_VA__Float64_VA: # @__operator_ADD__Float64_VA__Float64_VA
# BB#0:                                 # %entry
	pushl	%esi
Ltmp86:
	subl	$16, %esp
Ltmp87:
	movl	$0, 12(%esp)
	movl	24(%esp), %esi
	movl	%esi, (%esp)
	calll	___Float64_VA__Retain
	movl	12(%esp), %eax
	movl	%esi, 12(%esp)
	movl	%eax, (%esp)
	calll	___Float64_VA__Release
	movl	28(%esp), %eax
	movl	%eax, 4(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	calll	___method_ASSIGN_OP_ADD__Float64_VA__Float64_VA
	movl	12(%esp), %eax
	addl	$16, %esp
	popl	%esi
	ret

	.def	 ___method_push__Float64_VA__Float64;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_push__Float64_VA__Float64
	.align	16, 0x90
___method_push__Float64_VA__Float64:    # @__method_push__Float64_VA__Float64
# BB#0:                                 # %entry
	pushl	%edi
Ltmp88:
	pushl	%esi
Ltmp89:
	subl	$12, %esp
Ltmp90:
	movl	24(%esp), %esi
	movl	(%esi), %eax
	movl	%eax, (%esp)
	calll	___Float64_VA__Size
	movl	%eax, %edi
	leal	1(%edi), %eax
	movl	%eax, 8(%esp)
	movl	%esi, 4(%esp)
	movl	$___Float64_VA__Adapter, (%esp)
	calll	___Float64_VA__Resize
	movl	%edi, 4(%esp)
	movl	%esi, (%esp)
	calll	___Float64_VA__NonConstIndex
	movsd	28(%esp), %xmm0
	movsd	%xmm0, (%eax)
	movl	%esi, %eax
	addl	$12, %esp
	popl	%esi
	popl	%edi
	ret

	.def	 ___method_pop__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_pop__Float64_VA
	.align	16, 0x90
___method_pop__Float64_VA:              # @__method_pop__Float64_VA
# BB#0:                                 # %entry
	subl	$28, %esp
Ltmp91:
	leal	16(%esp), %eax
	movl	%eax, 8(%esp)
	movl	32(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$___Float64_VA__Adapter, (%esp)
	calll	___Float64_VA__Pop
	fldl	16(%esp)
	addl	$28, %esp
	ret

	.def	 ___method_resize__Float64_VA__Size;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_resize__Float64_VA__Size
	.align	16, 0x90
___method_resize__Float64_VA__Size:     # @__method_resize__Float64_VA__Size
# BB#0:                                 # %entry
	pushl	%esi
Ltmp92:
	subl	$16, %esp
Ltmp93:
	movl	28(%esp), %eax
	movl	%eax, 8(%esp)
	movl	24(%esp), %esi
	movl	%esi, 4(%esp)
	movl	$___Float64_VA__Adapter, (%esp)
	calll	___Float64_VA__Resize
	movl	%esi, %eax
	addl	$16, %esp
	popl	%esi
	ret

	.def	 ___method_resize__Float64_VA__Integer;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_resize__Float64_VA__Integer
	.align	16, 0x90
___method_resize__Float64_VA__Integer:  # @__method_resize__Float64_VA__Integer
# BB#0:                                 # %entry
	pushl	%esi
Ltmp94:
	subl	$16, %esp
Ltmp95:
	movl	28(%esp), %eax
	movl	%eax, 4(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	calll	___construct_Size__Integer
	movl	12(%esp), %eax
	movl	%eax, 8(%esp)
	movl	24(%esp), %esi
	movl	%esi, 4(%esp)
	movl	$___Float64_VA__Adapter, (%esp)
	calll	___Float64_VA__Resize
	movl	%esi, %eax
	addl	$16, %esp
	popl	%esi
	ret

	.def	 ___method_size__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_size__Float64_VA
	.align	16, 0x90
___method_size__Float64_VA:             # @__method_size__Float64_VA
# BB#0:                                 # %entry
	pushl	%eax
Ltmp96:
	movl	8(%esp), %eax
	movl	%eax, (%esp)
	calll	___Float64_VA__Size
	popl	%edx
	ret

	.def	 ___method_dataSize__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_dataSize__Float64_VA
	.align	16, 0x90
___method_dataSize__Float64_VA:         # @__method_dataSize__Float64_VA
# BB#0:                                 # %entry
	pushl	%eax
Ltmp97:
	movl	8(%esp), %eax
	movl	%eax, (%esp)
	calll	___Float64_VA__Size
	shll	$3, %eax
	popl	%edx
	ret

	.def	 ___method_data__Float64_VA;
	.scl	2;
	.type	32;
	.endef
	.globl	___method_data__Float64_VA
	.align	16, 0x90
___method_data__Float64_VA:             # @__method_data__Float64_VA
# BB#0:                                 # %entry
	movl	4(%esp), %eax
	testl	%eax, %eax
	jne	LBB207_2
# BB#1:                                 # %null
	ret
LBB207_2:                               # %nonNull
	addl	$16, %eax
	ret

	.def	 _entry;
	.scl	2;
	.type	32;
	.endef
	.globl	_entry
	.align	16, 0x90
_entry:                                 # @entry
# BB#0:                                 # %entry
	pushl	%ebp
Ltmp98:
	pushl	%ebx
Ltmp99:
	pushl	%edi
Ltmp100:
	pushl	%esi
Ltmp101:
	subl	$124, %esp
Ltmp102:
	movl	$0, 120(%esp)
	leal	112(%esp), %eax
	movl	%eax, (%esp)
	movl	$0, 4(%esp)
	calll	___construct_Float64__Scalar
	movsd	112(%esp), %xmm0
	movsd	%xmm0, 4(%esp)
	leal	120(%esp), %esi
	movl	%esi, (%esp)
	calll	___method_push__Float64_VA__Float64
	leal	104(%esp), %eax
	movl	%eax, (%esp)
	movl	$1073741824, 4(%esp)    # imm = 0x40000000
	calll	___construct_Float64__Scalar
	movsd	104(%esp), %xmm0
	movsd	%xmm0, 4(%esp)
	movl	%esi, (%esp)
	calll	___method_push__Float64_VA__Float64
	leal	96(%esp), %eax
	movl	%eax, (%esp)
	movl	$1082130432, 4(%esp)    # imm = 0x40800000
	calll	___construct_Float64__Scalar
	movsd	96(%esp), %xmm0
	movsd	%xmm0, 4(%esp)
	movl	%esi, (%esp)
	calll	___method_push__Float64_VA__Float64
	leal	88(%esp), %eax
	movl	%eax, (%esp)
	movl	$1086324736, 4(%esp)    # imm = 0x40C00000
	calll	___construct_Float64__Scalar
	movsd	88(%esp), %xmm0
	movsd	%xmm0, 4(%esp)
	movl	%esi, (%esp)
	calll	___method_push__Float64_VA__Float64
	leal	80(%esp), %eax
	movl	%eax, (%esp)
	movl	$1090519040, 4(%esp)    # imm = 0x41000000
	calll	___construct_Float64__Scalar
	movsd	80(%esp), %xmm0
	movsd	%xmm0, 4(%esp)
	movl	%esi, (%esp)
	calll	___method_push__Float64_VA__Float64
	leal	72(%esp), %eax
	movl	%eax, (%esp)
	movl	$1128660992, 4(%esp)    # imm = 0x43460000
	calll	___construct_Float64__Scalar
	movsd	72(%esp), %xmm0
	movsd	%xmm0, 4(%esp)
	movl	%esi, (%esp)
	calll	___method_push__Float64_VA__Float64
	leal	56(%esp), %eax
	movl	%eax, (%esp)
	movl	$0, 4(%esp)
	calll	___construct_Float64__Scalar
	movsd	56(%esp), %xmm0
	movsd	%xmm0, 64(%esp)
	leal	48(%esp), %eax
	movl	%eax, (%esp)
	movl	$0, 4(%esp)
	calll	___construct_Size__Integer
	movl	48(%esp), %eax
	movl	%eax, 52(%esp)
	movb	$1, %bl
	.align	16, 0x90
LBB208_1:                               # %loopCheckPreCond
                                        # =>This Inner Loop Header: Depth=1
	movl	52(%esp), %esi
	movl	120(%esp), %edi
	movl	%edi, (%esp)
	calll	___Float64_VA__Retain
	movl	%edi, (%esp)
	calll	___method_size__Float64_VA
	movl	%eax, %ebp
	movl	%edi, (%esp)
	calll	___Float64_VA__Release
	movl	%ebp, 4(%esp)
	leal	44(%esp), %ecx
	movl	%ecx, (%esp)
	calll	___construct_Integer__Size
	movl	44(%esp), %eax
	movl	%eax, (%esp)
	movl	$2, 4(%esp)
	calll	___operator_SUB__Integer__Integer
	movl	%eax, %edi
	movl	%esi, 4(%esp)
	leal	40(%esp), %ecx
	movl	%ecx, (%esp)
	calll	___construct_Integer__Size
	movl	40(%esp), %eax
	movl	%edi, 4(%esp)
	movl	%eax, (%esp)
	calll	___operator_LT__Integer__Integer
	testb	$1, %al
	je	LBB208_3
# BB#2:                                 # %loopBody
                                        #   in Loop: Header=BB208_1 Depth=1
	movl	120(%esp), %esi
	movl	%esi, (%esp)
	calll	___Float64_VA__Retain
	movl	52(%esp), %eax
	movl	%eax, 4(%esp)
	movl	%esi, (%esp)
	calll	___Float64_VA__ConstIndex
	fstpt	20(%esp)                # 10-byte Folded Spill
	movl	%esi, (%esp)
	calll	___Float64_VA__Release
	movsd	64(%esp), %xmm0
	fldt	20(%esp)                # 10-byte Folded Reload
	fstpl	8(%esp)
	movsd	%xmm0, (%esp)
	calll	___operator_ADD__Float64__Float64
	fstpl	64(%esp)
	leal	52(%esp), %ecx
	movl	%ecx, (%esp)
	calll	___operator_PRE_INC__Size
	testb	%bl, %bl
	jne	LBB208_1
LBB208_3:                               # %loopEnd
	movsd	64(%esp), %xmm0
	movl	$0, 36(%esp)
	movsd	%xmm0, 4(%esp)
	leal	36(%esp), %ecx
	movl	%ecx, (%esp)
	calll	___construct_String__Float64
	movl	36(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	movl	120(%esp), %esi
	movl	%esi, (%esp)
	calll	___Float64_VA__Retain
	movl	$0, 32(%esp)
	movl	%esi, 4(%esp)
	leal	32(%esp), %ecx
	movl	%ecx, (%esp)
	calll	___construct_String__Float64_VA
	movl	%esi, (%esp)
	calll	___Float64_VA__Release
	movl	32(%esp), %esi
	movl	%esi, (%esp)
	calll	___String__Report
	movl	%esi, (%esp)
	calll	___String__Release
	movl	120(%esp), %eax
	movl	%eax, (%esp)
	calll	___Float64_VA__Release
	addl	$124, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret

