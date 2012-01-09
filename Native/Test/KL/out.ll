; ModuleID = 'kl'

%ConstString = type <{ i8*, i64 }>
%Integer_AP = type i8**
%Integer_VA = type <{ i64, i64, i32* }>
%Scalar_AP = type i8**
%String = type %StringBits*
%StringBits = type { i64, i64, i64, [0 x i8] }

@__Data__Adapter = external constant i8*
@__Scalar__Adapter = external constant i8*
@__ConstString__Adapter = external constant i8*
@__Byte__Adapter = external constant i8*
@0 = internal constant [25 x i8] c"KL: Byte division by zero"
@1 = internal constant %ConstString <{ i8* getelementptr inbounds ([25 x i8]* @0, i32 0, i32 0), i64 25 }>
@2 = internal constant [25 x i8] c"KL: Byte division by zero"
@3 = internal constant %ConstString <{ i8* getelementptr inbounds ([25 x i8]* @2, i32 0, i32 0), i64 25 }>
@__Float64__Adapter = external constant i8*
@__Size__Adapter = external constant i8*
@4 = internal constant [25 x i8] c"KL: Size division by zero"
@5 = internal constant %ConstString <{ i8* getelementptr inbounds ([25 x i8]* @4, i32 0, i32 0), i64 25 }>
@6 = internal constant [25 x i8] c"KL: Size division by zero"
@7 = internal constant %ConstString <{ i8* getelementptr inbounds ([25 x i8]* @6, i32 0, i32 0), i64 25 }>
@8 = internal constant [4 x i8] c"true"
@9 = internal constant %ConstString <{ i8* getelementptr inbounds ([4 x i8]* @8, i32 0, i32 0), i64 4 }>
@10 = internal constant [5 x i8] c"false"
@11 = internal constant %ConstString <{ i8* getelementptr inbounds ([5 x i8]* @10, i32 0, i32 0), i64 5 }>
@__Integer__Adapter = external constant i8*
@12 = internal constant [28 x i8] c"KL: Integer division by zero"
@13 = internal constant %ConstString <{ i8* getelementptr inbounds ([28 x i8]* @12, i32 0, i32 0), i64 28 }>
@14 = internal constant [28 x i8] c"KL: Integer division by zero"
@15 = internal constant %ConstString <{ i8* getelementptr inbounds ([28 x i8]* @14, i32 0, i32 0), i64 28 }>
@16 = internal constant [12 x i8] c"map2: input="
@17 = internal constant %ConstString <{ i8* getelementptr inbounds ([12 x i8]* @16, i32 0, i32 0), i64 12 }>
@18 = internal constant [5 x i8] c"index"
@19 = internal constant %ConstString <{ i8* getelementptr inbounds ([5 x i8]* @18, i32 0, i32 0), i64 5 }>
@20 = internal constant [2 x i8] c"KL"
@21 = internal constant %ConstString <{ i8* getelementptr inbounds ([2 x i8]* @20, i32 0, i32 0), i64 2 }>
@22 = internal constant [2 x i8] c": "
@23 = internal constant %ConstString <{ i8* getelementptr inbounds ([2 x i8]* @22, i32 0, i32 0), i64 2 }>
@24 = internal constant [2 x i8] c" ("
@25 = internal constant %ConstString <{ i8* getelementptr inbounds ([2 x i8]* @24, i32 0, i32 0), i64 2 }>
@26 = internal constant [16 x i8] c") out of range ("
@27 = internal constant %ConstString <{ i8* getelementptr inbounds ([16 x i8]* @26, i32 0, i32 0), i64 16 }>
@28 = internal constant [1 x i8] c")"
@29 = internal constant %ConstString <{ i8* getelementptr inbounds ([1 x i8]* @28, i32 0, i32 0), i64 1 }>
@__String__Adapter = external constant i8*
@30 = internal constant [5 x i8] c"index"
@31 = internal constant %ConstString <{ i8* getelementptr inbounds ([5 x i8]* @30, i32 0, i32 0), i64 5 }>
@__Integer__DefaultValue = internal constant i32 0
@__Integer_VA__Adapter = external constant i8*
@32 = internal constant [2 x i8] c"KL"
@33 = internal constant %ConstString <{ i8* getelementptr inbounds ([2 x i8]* @32, i32 0, i32 0), i64 2 }>
@34 = internal constant [2 x i8] c"KL"
@35 = internal constant %ConstString <{ i8* getelementptr inbounds ([2 x i8]* @34, i32 0, i32 0), i64 2 }>
@36 = internal constant [2 x i8] c"KL"
@37 = internal constant %ConstString <{ i8* getelementptr inbounds ([2 x i8]* @36, i32 0, i32 0), i64 2 }>
@38 = internal constant [22 x i8] c"ArrayProducer<Integer>"
@39 = internal constant %ConstString <{ i8* getelementptr inbounds ([22 x i8]* @38, i32 0, i32 0), i64 22 }>
@__Integer_AP__Adapter = external constant i8*
@40 = internal constant [17 x i8] c"cap.getCount() = "
@41 = internal constant %ConstString <{ i8* getelementptr inbounds ([17 x i8]* @40, i32 0, i32 0), i64 17 }>
@42 = internal constant [21 x i8] c"ArrayProducer<Scalar>"
@43 = internal constant %ConstString <{ i8* getelementptr inbounds ([21 x i8]* @42, i32 0, i32 0), i64 21 }>
@__Scalar_AP__Adapter = external constant i8*

define private void @__construct_Boolean__Data(i1* noalias nocapture %booleanLValue, i8* %opaqueRValue) inlinehint {
entry:
  %0 = icmp ne i8* %opaqueRValue, null
  store i1 %0, i1* %booleanLValue
  ret void
}

define private void @__construct_String__Data(%String* noalias nocapture %stringLValue, i8* %opaqueRValue) inlinehint {
entry:
  %opaqueRValue_tempLValue = alloca i8*
  store i8* %opaqueRValue, %Integer_AP %opaqueRValue_tempLValue
  call void @__String__Cast(i8* bitcast (%Integer_AP @__Data__Adapter to i8*), %Integer_AP %opaqueRValue_tempLValue, %String* %stringLValue)
  ret void
}

declare void @__String__Cast(i8*, %Integer_AP, %String*)

define private i64 @__method_dataSize__Data(i8* %thisRValue) inlinehint {
entry:
  ret i64 8
}

define private i8* @__method_data__Data(%Integer_AP noalias nocapture %thisLValue) inlinehint {
entry:
  %0 = bitcast %Integer_AP %thisLValue to i8*
  ret i8* %0
}

declare void @report(i8*, i64) nounwind inlinehint

define private void @__String__Report(%String* noalias nocapture %string) inlinehint {
entry:
  %0 = load %String* %string
  %1 = icmp ne %String %0, null
  br i1 %1, label %notNull, label %done

notNull:                                          ; preds = %entry
  %2 = getelementptr inbounds %String %0, i32 0, i32 3
  %3 = getelementptr [0 x i8]* %2, i32 0, i32 0
  %4 = getelementptr inbounds %String %0, i32 0, i32 2
  %5 = load i64* %4
  call void @report(i8* %3, i64 %5)
  br label %done

done:                                             ; preds = %notNull, %entry
  ret void
}

define private void @__construct_Boolean__String(i1* noalias nocapture %booleanLValue, %String* noalias nocapture %stringRValue) inlinehint {
entry:
  %0 = call i64 @__method_length__String(%String* %stringRValue)
  %1 = icmp ne i64 %0, 0
  store i1 %1, i1* %booleanLValue
  ret void
}

define private i64 @__method_length__String(%String* noalias nocapture %string) inlinehint {
entry:
  %0 = load %String* %string
  %1 = icmp ne %String %0, null
  br i1 %1, label %nonNull, label %null

nonNull:                                          ; preds = %entry
  %2 = getelementptr inbounds %String %0, i32 0, i32 2
  %3 = load i64* %2
  ret i64 %3

null:                                             ; preds = %entry
  ret i64 0
}

declare void @__String__Append(%String*, %String*)

define private void @__method_ASSIGN_OP_ADD__String__String(%String* noalias nocapture %lhsLValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  call void @__String__Append(%String* %lhsLValue, %String* %rhsRValue)
  ret void
}

define private void @__operator_ADD__String__String(%String* noalias nocapture sret %returnVal, %String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = load %String* %returnVal
  %1 = load %String* %lhsRValue
  call void @__String_Retain(%String %1)
  store %String %1, %String* %returnVal
  call void @__String_Release(%String %0)
  call void @__method_ASSIGN_OP_ADD__String__String(%String* %returnVal, %String* %rhsRValue)
  ret void
}

define private void @__String_Retain(%String nocapture %bitsLValue) nounwind inlinehint {
entry:
  %0 = icmp ne %String %bitsLValue, null
  br i1 %0, label %nonNull, label %done

nonNull:                                          ; preds = %entry
  %1 = getelementptr inbounds %String %bitsLValue, i32 0, i32 0
  %2 = call i64 @llvm.atomic.load.add.i64.p0i64(i64* %1, i64 1)
  br label %done

done:                                             ; preds = %nonNull, %entry
  ret void
}

declare i64 @llvm.atomic.load.add.i64.p0i64(i64* nocapture, i64) nounwind

define private void @__String_Release(%String nocapture %bitsLValue) nounwind inlinehint {
entry:
  %0 = icmp ne %String %bitsLValue, null
  br i1 %0, label %nonNull, label %done

nonNull:                                          ; preds = %entry
  %1 = getelementptr inbounds %String %bitsLValue, i32 0, i32 0
  %2 = call i64 @llvm.atomic.load.sub.i64.p0i64(i64* %1, i64 1)
  %3 = icmp eq i64 %2, 1
  br i1 %3, label %free, label %done

free:                                             ; preds = %nonNull
  %4 = bitcast %String %bitsLValue to i8*
  call void @free(i8* %4)
  br label %done

done:                                             ; preds = %free, %nonNull, %entry
  ret void
}

declare i64 @llvm.atomic.load.sub.i64.p0i64(i64* nocapture, i64) nounwind

declare void @free(i8*) nounwind

define private i64 @__method_dataSize__String(%String* noalias nocapture %thisRValue) inlinehint {
entry:
  %0 = call i64 @__method_length__String(%String* %thisRValue)
  %1 = add i64 %0, 1
  ret i64 %1
}

define private i8* @__method_data__String(%String* noalias nocapture %thisRValue) inlinehint {
entry:
  %0 = load %String* %thisRValue
  %1 = icmp eq %String %0, null
  br i1 %1, label %null, label %nonNull

null:                                             ; preds = %entry
  %2 = bitcast %String %0 to i8*
  ret i8* %2

nonNull:                                          ; preds = %entry
  %3 = getelementptr inbounds %String %0, i32 0, i32 3
  %4 = getelementptr [0 x i8]* %3, i32 0, i32 0
  %5 = getelementptr i8* %4, i32 0
  ret i8* %5
}

define private i32 @__method_compare__String__String(%String* noalias nocapture %lhs, %String* noalias nocapture %rhs) inlinehint {
entry:
  %0 = load %String* %lhs
  %1 = load %String* %rhs
  br label %checkPointers

checkPointers:                                    ; preds = %entry
  %2 = icmp eq %String %0, null
  br i1 %2, label %thisIsNull, label %thisIsNotNull

thisIsNull:                                       ; preds = %checkPointers
  %3 = icmp eq %String %1, null
  br i1 %3, label %eq, label %lt

thisIsNotNull:                                    ; preds = %checkPointers
  %4 = icmp eq %String %1, null
  br i1 %4, label %gt, label %shallow

shallow:                                          ; preds = %thisIsNotNull
  %5 = icmp eq %String %0, %1
  br i1 %5, label %eq, label %deep

deep:                                             ; preds = %shallow
  %thisLengthPtr = getelementptr inbounds %String %0, i32 0, i32 2
  %thisLength = load i64* %thisLengthPtr
  %6 = getelementptr inbounds %String %0, i32 0, i32 3
  %thisCStr = getelementptr [0 x i8]* %6, i32 0, i32 0
  %otherLengthPtr = getelementptr inbounds %String %1, i32 0, i32 2
  %otherLength = load i64* %otherLengthPtr
  %7 = getelementptr inbounds %String %1, i32 0, i32 3
  %otherCStr = getelementptr [0 x i8]* %7, i32 0, i32 0
  %indexPtr = alloca i64
  store i64 0, i64* %indexPtr
  br label %check

check:                                            ; preds = %next, %deep
  %index = load i64* %indexPtr
  br label %checkSelfLength

checkSelfLength:                                  ; preds = %check
  %8 = icmp ult i64 %index, %thisLength
  br i1 %8, label %checkOtherLength, label %verifyOtherLength

verifyOtherLength:                                ; preds = %checkSelfLength
  %9 = icmp ult i64 %index, %otherLength
  br i1 %9, label %lt, label %eq

checkOtherLength:                                 ; preds = %checkSelfLength
  %10 = icmp ult i64 %index, %otherLength
  br i1 %10, label %checkChars, label %gt

checkChars:                                       ; preds = %checkOtherLength
  %thisCharPtr = getelementptr i8* %thisCStr, i64 %index
  %thisChar = load i8* %thisCharPtr
  %otherCharPtr = getelementptr i8* %otherCStr, i64 %index
  %otherChar = load i8* %otherCharPtr
  br label %checkCharsLT

checkCharsLT:                                     ; preds = %checkChars
  %11 = icmp ult i8 %thisChar, %otherChar
  br i1 %11, label %lt, label %checkCharsGT

checkCharsGT:                                     ; preds = %checkCharsLT
  %12 = icmp ugt i8 %thisChar, %otherChar
  br i1 %12, label %gt, label %next

next:                                             ; preds = %checkCharsGT
  %13 = add i64 %index, 1
  store i64 %13, i64* %indexPtr
  br label %check

eq:                                               ; preds = %verifyOtherLength, %shallow, %thisIsNull
  ret i32 0

lt:                                               ; preds = %checkCharsLT, %verifyOtherLength, %thisIsNull
  ret i32 -1

gt:                                               ; preds = %checkCharsGT, %checkOtherLength, %thisIsNotNull
  ret i32 1
}

define private i64 @__method_hash__String(%String* noalias nocapture %rValue) inlinehint {
entry:
  %result = alloca i64
  %index = alloca i64
  store i64 5381, i64* %result
  %0 = load %String* %rValue
  %1 = icmp eq %String %0, null
  br i1 %1, label %done, label %notNull

notNull:                                          ; preds = %entry
  %2 = getelementptr inbounds %String %0, i32 0, i32 3
  %cStr = getelementptr [0 x i8]* %2, i32 0, i32 0
  %3 = call i64 @__method_length__String(%String* %rValue)
  store i64 0, i64* %index
  br label %loopCheck

loopCheck:                                        ; preds = %loopStep, %notNull
  %4 = load i64* %index
  %5 = icmp ult i64 %4, %3
  br i1 %5, label %loopStep, label %done

loopStep:                                         ; preds = %loopCheck
  %charLValue = getelementptr i8* %cStr, i64 %4
  %charRValue = load i8* %charLValue
  %6 = load i64* %result
  %7 = zext i8 %charRValue to i64
  %8 = mul i64 %6, 33
  %9 = xor i64 %8, %7
  store i64 %9, i64* %result
  %10 = add i64 %4, 1
  store i64 %10, i64* %index
  br label %loopCheck

done:                                             ; preds = %loopCheck, %entry
  %11 = load i64* %result
  ret i64 %11
}

define private i1 @__operator_EQ__String__String(%String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = call i32 @__method_compare__String__String(%String* %lhsRValue, %String* %rhsRValue)
  %1 = icmp eq i32 %0, 0
  ret i1 %1
}

define private i1 @__operator_NE__String__String(%String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = call i32 @__method_compare__String__String(%String* %lhsRValue, %String* %rhsRValue)
  %1 = icmp ne i32 %0, 0
  ret i1 %1
}

define private i1 @__operator_GT__String__String(%String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = call i32 @__method_compare__String__String(%String* %lhsRValue, %String* %rhsRValue)
  %1 = icmp sgt i32 %0, 0
  ret i1 %1
}

define private i1 @__operator_GE__String__String(%String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = call i32 @__method_compare__String__String(%String* %lhsRValue, %String* %rhsRValue)
  %1 = icmp sge i32 %0, 0
  ret i1 %1
}

define private i1 @__operator_LT__String__String(%String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = call i32 @__method_compare__String__String(%String* %lhsRValue, %String* %rhsRValue)
  %1 = icmp slt i32 %0, 0
  ret i1 %1
}

define private i1 @__operator_LE__String__String(%String* noalias nocapture %lhsRValue, %String* noalias nocapture %rhsRValue) inlinehint {
entry:
  %0 = call i32 @__method_compare__String__String(%String* %lhsRValue, %String* %rhsRValue)
  %1 = icmp sle i32 %0, 0
  ret i1 %1
}

define private void @__construct_Boolean__Scalar(i1* noalias nocapture %booleanLValue, float %scalarRValue) inlinehint {
entry:
  %0 = fcmp one float %scalarRValue, 0.000000e+00
  store i1 %0, i1* %booleanLValue
  ret void
}

define private void @__construct_Byte__Scalar(i8* noalias nocapture %byteLValue, float %scalarRValue) inlinehint {
entry:
  %0 = fptoui float %scalarRValue to i8
  store i8 %0, i8* %byteLValue
  ret void
}

define private void @__construct_Integer__Scalar(i32* noalias nocapture %integerLValue, float %scalarRValue) inlinehint {
entry:
  %0 = fptosi float %scalarRValue to i32
  store i32 %0, i32* %integerLValue
  ret void
}

define private void @__construct_Size__Scalar(i64* noalias nocapture %sizeLValue, float %scalarRValue) inlinehint {
entry:
  %0 = fptoui float %scalarRValue to i64
  store i64 %0, i64* %sizeLValue
  ret void
}

define private void @__construct_Float64__Scalar(double* noalias nocapture %fp64LValue, float %scalarRValue) inlinehint {
entry:
  %0 = fpext float %scalarRValue to double
  store double %0, double* %fp64LValue
  ret void
}

define private void @__construct_String__Scalar(%String* noalias nocapture %stringLValue, float %scalarRValue) inlinehint {
entry:
  %scalarRValue_tempLValue = alloca float
  store float %scalarRValue, float* %scalarRValue_tempLValue
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, float*, %String*)*)(i8* bitcast (%Integer_AP @__Scalar__Adapter to i8*), float* %scalarRValue_tempLValue, %String* %stringLValue)
  ret void
}

define private float @__operator_POS__Scalar(float %rValue) inlinehint {
entry:
  ret float %rValue
}

define private float @__operator_NEG__Scalar(float %rValue) inlinehint {
entry:
  %0 = fsub float -0.000000e+00, %rValue
  ret float %0
}

define private float @sin(float %x) inlinehint {
entry:
  %0 = call float @llvm.sin.f32(float %x)
  ret float %0
}

declare float @llvm.sin.f32(float) nounwind readonly

define private float @cos(float %x) inlinehint {
entry:
  %0 = call float @llvm.cos.f32(float %x)
  ret float %0
}

declare float @llvm.cos.f32(float) nounwind readonly

declare float @fp32_tan(float) inlinehint

define private float @tan(float %x) inlinehint {
entry:
  %0 = call float @fp32_tan(float %x)
  ret float %0
}

declare float @fp32_log10(float) inlinehint

define private float @log10(float %x) inlinehint {
entry:
  %0 = call float @fp32_log10(float %x)
  ret float %0
}

declare float @fp32_log(float) inlinehint

define private float @log(float %x) inlinehint {
entry:
  %0 = call float @fp32_log(float %x)
  ret float %0
}

declare float @fp32_exp(float) inlinehint

define private float @exp(float %x) inlinehint {
entry:
  %0 = call float @fp32_exp(float %x)
  ret float %0
}

declare float @fp32_acos(float) inlinehint

define private float @acos(float %x) inlinehint {
entry:
  %0 = call float @fp32_acos(float %x)
  ret float %0
}

declare float @fp32_asin(float) inlinehint

define private float @asin(float %x) inlinehint {
entry:
  %0 = call float @fp32_asin(float %x)
  ret float %0
}

declare float @fp32_atan(float) inlinehint

define private float @atan(float %x) inlinehint {
entry:
  %0 = call float @fp32_atan(float %x)
  ret float %0
}

declare float @fp32_atan2(float, float) inlinehint

define private float @atan2(float %y, float %x) inlinehint {
entry:
  %0 = call float @fp32_atan2(float %y, float %x)
  ret float %0
}

define private float @sqrt(float %x) inlinehint {
entry:
  %0 = call float @llvm.sqrt.f32(float %x)
  ret float %0
}

declare float @llvm.sqrt.f32(float) nounwind readonly

define private float @abs(float %x) inlinehint {
entry:
  %0 = bitcast float %x to i32
  %1 = and i32 %0, 2147483647
  %2 = bitcast i32 %1 to float
  ret float %2
}

declare float @fp32_round(float) inlinehint

define private float @round(float %x) inlinehint {
entry:
  %0 = call float @fp32_round(float %x)
  ret float %0
}

declare float @fp32_ceil(float) inlinehint

define private float @ceil(float %x) inlinehint {
entry:
  %0 = call float @fp32_ceil(float %x)
  ret float %0
}

declare float @fp32_floor(float) inlinehint

define private float @floor(float %x) inlinehint {
entry:
  %0 = call float @fp32_floor(float %x)
  ret float %0
}

define private float @pow(float %x, float %y) inlinehint {
entry:
  %0 = call float @llvm.pow.f32(float %x, float %y)
  ret float %0
}

declare float @llvm.pow.f32(float, float) nounwind readonly

define private float @__operator_ADD__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fadd float %lhsRValue, %rhsRValue
  ret float %0
}

define private float @__operator_SUB__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fsub float %lhsRValue, %rhsRValue
  ret float %0
}

define private float @__operator_MUL__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fmul float %lhsRValue, %rhsRValue
  ret float %0
}

define private float @__operator_DIV__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fdiv float %lhsRValue, %rhsRValue
  ret float %0
}

define private float @__operator_MOD__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = frem float %lhsRValue, %rhsRValue
  ret float %0
}

define private i1 @__operator_EQ__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp oeq float %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_NE__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp one float %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GT__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp ogt float %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GE__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp oge float %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LT__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp olt float %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LE__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp ole float %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i64 @__method_dataSize__Scalar(float %thisRValue) inlinehint {
entry:
  ret i64 4
}

define private i8* @__method_data__Scalar(float* noalias nocapture %thisLValue) inlinehint {
entry:
  %0 = bitcast float* %thisLValue to i8*
  ret i8* %0
}

define private i64 @__method_hash__Scalar(float %rValue) inlinehint {
entry:
  %0 = bitcast float %rValue to i32
  %1 = zext i32 %0 to i64
  ret i64 %1
}

define private i32 @__method_compare__Scalar__Scalar(float %lhsRValue, float %rhsRValue) inlinehint {
entry:
  %0 = fcmp olt float %lhsRValue, %rhsRValue
  br i1 %0, label %lt, label %ge

lt:                                               ; preds = %entry
  ret i32 -1

ge:                                               ; preds = %entry
  %1 = fcmp oeq float %lhsRValue, %rhsRValue
  br i1 %1, label %eq, label %gt

eq:                                               ; preds = %ge
  ret i32 0

gt:                                               ; preds = %ge
  ret i32 1
}

define private void @__construct_Boolean__ConstString(i1* noalias nocapture %booleanLValue, %ConstString* noalias nocapture %constStringRValue) inlinehint {
entry:
  %0 = getelementptr inbounds %ConstString* %constStringRValue, i32 0, i32 1
  %1 = load i64* %0
  %2 = icmp ne i64 %1, 0
  store i1 %2, i1* %booleanLValue
  ret void
}

define private void @__construct_String__ConstString(%String* noalias nocapture %stringLValue, %ConstString* noalias nocapture %constStringRValue) inlinehint {
entry:
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, %ConstString*, %String*)*)(i8* bitcast (%Integer_AP @__ConstString__Adapter to i8*), %ConstString* %constStringRValue, %String* %stringLValue)
  ret void
}

define private void @__construct_Boolean__Byte(i1* noalias nocapture %booleanLValue, i8 %byteRValue) inlinehint {
entry:
  %0 = icmp ne i8 %byteRValue, 0
  store i1 %0, i1* %booleanLValue
  ret void
}

define private void @__construct_Integer__Byte(i32* noalias nocapture %integerLValue, i8 %byteRValue) inlinehint {
entry:
  %0 = zext i8 %byteRValue to i32
  store i32 %0, i32* %integerLValue
  ret void
}

define private void @__construct_Size__Byte(i64* noalias nocapture %sizeLValue, i8 %byteRValue) inlinehint {
entry:
  %0 = zext i8 %byteRValue to i64
  store i64 %0, i64* %sizeLValue
  ret void
}

define private void @__construct_Scalar__Byte(float* noalias nocapture %scalarLValue, i8 %byteRValue) inlinehint {
entry:
  %0 = uitofp i8 %byteRValue to float
  store float %0, float* %scalarLValue
  ret void
}

define private void @__construct_String__Byte(%String* noalias nocapture %stringLValue, i8 %byteRValue) inlinehint {
entry:
  %byteRValue_tempLValue = alloca i8
  store i8 %byteRValue, i8* %byteRValue_tempLValue
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, i8*, %String*)*)(i8* bitcast (%Integer_AP @__Byte__Adapter to i8*), i8* %byteRValue_tempLValue, %String* %stringLValue)
  ret void
}

define private i8 @__operator_POS__Byte(i8 %rValue) inlinehint {
entry:
  ret i8 %rValue
}

define private i8 @__operator_BIT_NOT__Byte(i8 %rValue) inlinehint {
entry:
  %0 = xor i8 %rValue, -1
  ret i8 %0
}

define private i8 @__operator_PRE_INC__Byte(i8* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i8* %lValue
  %1 = add i8 %0, 1
  store i8 %1, i8* %lValue
  ret i8 %1
}

define private i8 @__operator_PRE_DEC__Byte(i8* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i8* %lValue
  %1 = sub i8 %0, 1
  store i8 %1, i8* %lValue
  ret i8 %1
}

define private i8 @__operator_POST_INC__Byte(i8* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i8* %lValue
  %1 = add i8 %0, 1
  store i8 %1, i8* %lValue
  ret i8 %0
}

define private i8 @__operator_POST_DEC__Byte(i8* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i8* %lValue
  %1 = sub i8 %0, 1
  store i8 %1, i8* %lValue
  ret i8 %0
}

define private i8 @__operator_ADD__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = add i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_SUB__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = sub i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_MUL__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = mul i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_DIV__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %castToString = alloca %String
  %0 = icmp ne i8 %rhsRValue, 0
  br i1 %0, label %nonZero, label %zero

nonZero:                                          ; preds = %entry
  %1 = udiv i8 %lhsRValue, %rhsRValue
  ret i8 %1

zero:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @1)
  call void @__String__Report(%String* %castToString)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret i8 0
}

define private i8 @__operator_MOD__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %castToString = alloca %String
  %0 = icmp ne i8 %rhsRValue, 0
  br i1 %0, label %nonZero, label %zero

nonZero:                                          ; preds = %entry
  %1 = urem i8 %lhsRValue, %rhsRValue
  ret i8 %1

zero:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @3)
  call void @__String__Report(%String* %castToString)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret i8 0
}

define private i8 @__operator_BIT_OR__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = or i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_BIT_AND__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = and i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_BIT_XOR__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = xor i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_SHL__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = shl i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i8 @__operator_SHR__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = lshr i8 %lhsRValue, %rhsRValue
  ret i8 %0
}

define private i1 @__operator_EQ__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = icmp eq i8 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_NE__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = icmp ne i8 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GT__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = icmp ugt i8 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GE__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = icmp uge i8 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LT__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = icmp ult i8 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LE__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = icmp ule i8 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i64 @__method_dataSize__Byte(i8 %thisRValue) inlinehint {
entry:
  ret i64 1
}

define private i8* @__method_data__Byte(i8* noalias nocapture %thisLValue) inlinehint {
entry:
  ret i8* %thisLValue
}

define private i64 @__method_hash__Byte(i8 %rValue) inlinehint {
entry:
  %0 = zext i8 %rValue to i64
  ret i64 %0
}

define private i32 @__method_compare__Byte__Byte(i8 %lhsRValue, i8 %rhsRValue) inlinehint {
entry:
  %0 = zext i8 %lhsRValue to i32
  %1 = zext i8 %rhsRValue to i32
  %2 = sub i32 %0, %1
  ret i32 %2
}

define private void @__construct_Boolean__Float64(i1* noalias nocapture %booleanLValue, double %scalarRValue) inlinehint {
entry:
  %0 = fcmp one double %scalarRValue, 0.000000e+00
  store i1 %0, i1* %booleanLValue
  ret void
}

define private void @__construct_Byte__Float64(i8* noalias nocapture %byteLValue, double %scalarRValue) inlinehint {
entry:
  %0 = fptoui double %scalarRValue to i8
  store i8 %0, i8* %byteLValue
  ret void
}

define private void @__construct_Integer__Float64(i32* noalias nocapture %integerLValue, double %scalarRValue) inlinehint {
entry:
  %0 = fptosi double %scalarRValue to i32
  store i32 %0, i32* %integerLValue
  ret void
}

define private void @__construct_Size__Float64(i64* noalias nocapture %sizeLValue, double %scalarRValue) inlinehint {
entry:
  %0 = fptoui double %scalarRValue to i64
  store i64 %0, i64* %sizeLValue
  ret void
}

define private void @__construct_Scalar__Float64(float* noalias nocapture %fp32LValue, double %scalarRValue) inlinehint {
entry:
  %0 = fptrunc double %scalarRValue to float
  store float %0, float* %fp32LValue
  ret void
}

define private void @__construct_String__Float64(%String* noalias nocapture %stringLValue, double %scalarRValue) inlinehint {
entry:
  %scalarRValue_tempLValue = alloca double
  store double %scalarRValue, double* %scalarRValue_tempLValue
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, double*, %String*)*)(i8* bitcast (%Integer_AP @__Float64__Adapter to i8*), double* %scalarRValue_tempLValue, %String* %stringLValue)
  ret void
}

define private double @__operator_POS__Float64(double %rValue) inlinehint {
entry:
  ret double %rValue
}

define private double @__operator_NEG__Float64(double %rValue) inlinehint {
entry:
  %0 = fsub double -0.000000e+00, %rValue
  ret double %0
}

define private double @sin64(double %x) inlinehint {
entry:
  %0 = call double @llvm.sin.f64(double %x)
  ret double %0
}

declare double @llvm.sin.f64(double) nounwind readonly

define private double @cos64(double %x) inlinehint {
entry:
  %0 = call double @llvm.cos.f64(double %x)
  ret double %0
}

declare double @llvm.cos.f64(double) nounwind readonly

declare double @fp64_tan(double) inlinehint

define private double @tan64(double %x) inlinehint {
entry:
  %0 = call double @fp64_tan(double %x)
  ret double %0
}

declare double @fp64_log10(double) inlinehint

define private double @log1064(double %x) inlinehint {
entry:
  %0 = call double @fp64_log10(double %x)
  ret double %0
}

declare double @fp64_log(double) inlinehint

define private double @log64(double %x) inlinehint {
entry:
  %0 = call double @fp64_log(double %x)
  ret double %0
}

declare double @fp64_exp(double) inlinehint

define private double @exp64(double %x) inlinehint {
entry:
  %0 = call double @fp64_exp(double %x)
  ret double %0
}

declare double @fp64_acos(double) inlinehint

define private double @acos64(double %x) inlinehint {
entry:
  %0 = call double @fp64_acos(double %x)
  ret double %0
}

declare double @fp64_asin(double) inlinehint

define private double @asin64(double %x) inlinehint {
entry:
  %0 = call double @fp64_asin(double %x)
  ret double %0
}

declare double @fp64_atan(double) inlinehint

define private double @atan64(double %x) inlinehint {
entry:
  %0 = call double @fp64_atan(double %x)
  ret double %0
}

declare double @fp64_atan2(double, double) inlinehint

define private double @atan264(double %y, double %x) inlinehint {
entry:
  %0 = call double @fp64_atan2(double %y, double %x)
  ret double %0
}

define private double @sqrt64(double %x) inlinehint {
entry:
  %0 = call double @llvm.sqrt.f64(double %x)
  ret double %0
}

declare double @llvm.sqrt.f64(double) nounwind readonly

define private double @abs64(double %x) inlinehint {
entry:
  %0 = bitcast double %x to i64
  %1 = and i64 %0, 9223372036854775807
  %2 = bitcast i64 %1 to double
  ret double %2
}

declare double @fp64_round(double) inlinehint

define private double @round64(double %x) inlinehint {
entry:
  %0 = call double @fp64_round(double %x)
  ret double %0
}

declare double @fp64_ceil(double) inlinehint

define private double @ceil64(double %x) inlinehint {
entry:
  %0 = call double @fp64_ceil(double %x)
  ret double %0
}

declare double @fp64_floor(double) inlinehint

define private double @floor64(double %x) inlinehint {
entry:
  %0 = call double @fp64_floor(double %x)
  ret double %0
}

define private double @pow64(double %x, double %y) inlinehint {
entry:
  %0 = call double @llvm.pow.f64(double %x, double %y)
  ret double %0
}

declare double @llvm.pow.f64(double, double) nounwind readonly

define private double @__operator_ADD__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fadd double %lhsRValue, %rhsRValue
  ret double %0
}

define private double @__operator_SUB__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fsub double %lhsRValue, %rhsRValue
  ret double %0
}

define private double @__operator_MUL__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fmul double %lhsRValue, %rhsRValue
  ret double %0
}

define private double @__operator_DIV__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fdiv double %lhsRValue, %rhsRValue
  ret double %0
}

define private double @__operator_MOD__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = frem double %lhsRValue, %rhsRValue
  ret double %0
}

define private i1 @__operator_EQ__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp oeq double %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_NE__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp one double %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GT__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp ogt double %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GE__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp oge double %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LT__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp olt double %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LE__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp ole double %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i64 @__method_dataSize__Float64(double %thisRValue) inlinehint {
entry:
  ret i64 8
}

define private i8* @__method_data__Float64(double* noalias nocapture %thisLValue) inlinehint {
entry:
  %0 = bitcast double* %thisLValue to i8*
  ret i8* %0
}

define private i64 @__method_hash__Float64(double %rValue) inlinehint {
entry:
  %0 = bitcast double %rValue to i64
  ret i64 %0
}

define private i32 @__method_compare__Float64__Float64(double %lhsRValue, double %rhsRValue) inlinehint {
entry:
  %0 = fcmp olt double %lhsRValue, %rhsRValue
  br i1 %0, label %lt, label %ge

lt:                                               ; preds = %entry
  ret i32 -1

ge:                                               ; preds = %entry
  %1 = fcmp oeq double %lhsRValue, %rhsRValue
  br i1 %1, label %eq, label %gt

eq:                                               ; preds = %ge
  ret i32 0

gt:                                               ; preds = %ge
  ret i32 1
}

define private void @__construct_Boolean__Size(i1* noalias nocapture %booleanLValue, i64 %sizeRValue) inlinehint {
entry:
  %0 = icmp ne i64 %sizeRValue, 0
  store i1 %0, i1* %booleanLValue
  ret void
}

define private void @__construct_Byte__Size(i8* noalias nocapture %byteLValue, i64 %sizeRValue) inlinehint {
entry:
  %0 = trunc i64 %sizeRValue to i8
  store i8 %0, i8* %byteLValue
  ret void
}

define private void @__construct_Integer__Size(i32* noalias nocapture %integerLValue, i64 %sizeRValue) inlinehint {
entry:
  %0 = trunc i64 %sizeRValue to i32
  store i32 %0, i32* %integerLValue
  ret void
}

define private void @__construct_Scalar__Size(float* noalias nocapture %scalarLValue, i64 %sizeRValue) inlinehint {
entry:
  %0 = uitofp i64 %sizeRValue to float
  store float %0, float* %scalarLValue
  ret void
}

define private void @__construct_Float64__Size(double* noalias nocapture %scalarLValue, i64 %sizeRValue) inlinehint {
entry:
  %0 = uitofp i64 %sizeRValue to double
  store double %0, double* %scalarLValue
  ret void
}

define private void @__construct_String__Size(%String* noalias nocapture %stringLValue, i64 %sizeRValue) inlinehint {
entry:
  %sizeRValue_tempLValue = alloca i64
  store i64 %sizeRValue, i64* %sizeRValue_tempLValue
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, i64*, %String*)*)(i8* bitcast (%Integer_AP @__Size__Adapter to i8*), i64* %sizeRValue_tempLValue, %String* %stringLValue)
  ret void
}

define private i64 @__operator_POS__Size(i64 %rValue) inlinehint {
entry:
  ret i64 %rValue
}

define private i64 @__operator_BIT_NOT__Size(i64 %rValue) inlinehint {
entry:
  %0 = xor i64 %rValue, -1
  ret i64 %0
}

define private i64 @__operator_PRE_INC__Size(i64* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i64* %lValue
  %1 = add i64 %0, 1
  store i64 %1, i64* %lValue
  ret i64 %1
}

define private i64 @__operator_PRE_DEC__Size(i64* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i64* %lValue
  %1 = sub i64 %0, 1
  store i64 %1, i64* %lValue
  ret i64 %1
}

define private i64 @__operator_POST_INC__Size(i64* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i64* %lValue
  %1 = add i64 %0, 1
  store i64 %1, i64* %lValue
  ret i64 %0
}

define private i64 @__operator_POST_DEC__Size(i64* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i64* %lValue
  %1 = sub i64 %0, 1
  store i64 %1, i64* %lValue
  ret i64 %0
}

define private i64 @__operator_ADD__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = add i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_SUB__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = sub i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_MUL__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = mul i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_DIV__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %castToString = alloca %String
  %0 = icmp ne i64 %rhsRValue, 0
  br i1 %0, label %nonZero, label %zero

nonZero:                                          ; preds = %entry
  %1 = udiv i64 %lhsRValue, %rhsRValue
  ret i64 %1

zero:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @5)
  call void @__String__Report(%String* %castToString)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret i64 0
}

define private i64 @__operator_MOD__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %castToString = alloca %String
  %0 = icmp ne i64 %rhsRValue, 0
  br i1 %0, label %nonZero, label %zero

nonZero:                                          ; preds = %entry
  %1 = urem i64 %lhsRValue, %rhsRValue
  ret i64 %1

zero:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @7)
  call void @__String__Report(%String* %castToString)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret i64 0
}

define private i64 @__operator_BIT_OR__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = or i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_BIT_AND__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = and i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_BIT_XOR__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = xor i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_SHL__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = shl i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i64 @__operator_SHR__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = lshr i64 %lhsRValue, %rhsRValue
  ret i64 %0
}

define private i1 @__operator_EQ__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp eq i64 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_NE__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp ne i64 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GT__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp ugt i64 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GE__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp uge i64 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LT__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp ult i64 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LE__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp ule i64 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i64 @__method_dataSize__Size(i64 %thisRValue) inlinehint {
entry:
  ret i64 8
}

define private i8* @__method_data__Size(i64* noalias nocapture %thisLValue) inlinehint {
entry:
  %0 = bitcast i64* %thisLValue to i8*
  ret i8* %0
}

define private i64 @__method_hash__Size(i64 %rValue) inlinehint {
entry:
  ret i64 %rValue
}

define private i32 @__method_compare__Size__Size(i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %0 = icmp ult i64 %lhsRValue, %rhsRValue
  br i1 %0, label %lt, label %ge

lt:                                               ; preds = %entry
  ret i32 -1

ge:                                               ; preds = %entry
  %1 = icmp eq i64 %lhsRValue, %rhsRValue
  br i1 %1, label %eq, label %gt

eq:                                               ; preds = %ge
  ret i32 0

gt:                                               ; preds = %ge
  ret i32 1
}

define private void @__construct_String__Boolean(%String* noalias nocapture %stringLValue, i1 %booleanRValue) inlinehint {
entry:
  %castToString = alloca %String
  %castToString1 = alloca %String
  br i1 %booleanRValue, label %true, label %false

true:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @9)
  %0 = load %String* %stringLValue
  %1 = load %String* %castToString
  call void @__String_Retain(%String %1)
  store %String %1, %String* %stringLValue
  call void @__String_Release(%String %0)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret void

false:                                            ; preds = %entry
  store %String null, %String* %castToString1
  call void @__construct_String__ConstString(%String* %castToString1, %ConstString* @11)
  %3 = load %String* %stringLValue
  %4 = load %String* %castToString1
  call void @__String_Retain(%String %4)
  store %String %4, %String* %stringLValue
  call void @__String_Release(%String %3)
  %5 = load %String* %castToString1
  call void @__String_Release(%String %5)
  ret void
}

define private i1 @__operator_BIT_NOT__Boolean(i1 %rValue) inlinehint {
entry:
  %0 = xor i1 %rValue, true
  ret i1 %0
}

define private i1 @__operator_BIT_OR__Boolean__Boolean(i1 %lhsRValue, i1 %rhsRValue) inlinehint {
entry:
  %0 = or i1 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_BIT_AND__Boolean__Boolean(i1 %lhsRValue, i1 %rhsRValue) inlinehint {
entry:
  %0 = and i1 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_BIT_XOR__Boolean__Boolean(i1 %lhsRValue, i1 %rhsRValue) inlinehint {
entry:
  %0 = xor i1 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_EQ__Boolean__Boolean(i1 %lhsRValue, i1 %rhsRValue) inlinehint {
entry:
  %0 = icmp eq i1 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_NE__Boolean__Boolean(i1 %lhsRValue, i1 %rhsRValue) inlinehint {
entry:
  %0 = icmp ne i1 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i64 @__method_hash__Boolean(i1 %rValue) inlinehint {
entry:
  %0 = zext i1 %rValue to i64
  ret i64 %0
}

define private i32 @__method_compare__Boolean__Boolean(i1 %lhsRValue, i1 %rhsRValue) inlinehint {
entry:
  %0 = zext i1 %lhsRValue to i32
  %1 = zext i1 %rhsRValue to i32
  %2 = sub i32 %0, %1
  ret i32 %2
}

define private void @__construct_Boolean__Integer(i1* noalias nocapture %booleanLValue, i32 %integerRValue) inlinehint {
entry:
  %0 = icmp ne i32 %integerRValue, 0
  store i1 %0, i1* %booleanLValue
  ret void
}

define private void @__construct_Byte__Integer(i8* noalias nocapture %byteLValue, i32 %integerRValue) inlinehint {
entry:
  %0 = trunc i32 %integerRValue to i8
  store i8 %0, i8* %byteLValue
  ret void
}

define private void @__construct_Size__Integer(i64* noalias nocapture %sizeLValue, i32 %integerRValue) inlinehint {
entry:
  %0 = sext i32 %integerRValue to i64
  store i64 %0, i64* %sizeLValue
  ret void
}

define private void @__construct_Scalar__Integer(float* noalias nocapture %scalarLValue, i32 %integerRValue) inlinehint {
entry:
  %0 = sitofp i32 %integerRValue to float
  store float %0, float* %scalarLValue
  ret void
}

define private void @__construct_Float64__Integer(double* noalias nocapture %scalarLValue, i32 %integerRValue) inlinehint {
entry:
  %0 = sitofp i32 %integerRValue to double
  store double %0, double* %scalarLValue
  ret void
}

define private void @__construct_String__Integer(%String* noalias nocapture %stringLValue, i32 %integerRValue) inlinehint {
entry:
  %integerRValue_tempLValue = alloca i32
  store i32 %integerRValue, i32* %integerRValue_tempLValue
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, i32*, %String*)*)(i8* bitcast (%Integer_AP @__Integer__Adapter to i8*), i32* %integerRValue_tempLValue, %String* %stringLValue)
  ret void
}

define private i32 @__operator_POS__Integer(i32 %rValue) inlinehint {
entry:
  ret i32 %rValue
}

define private i32 @__operator_NEG__Integer(i32 %rValue) inlinehint {
entry:
  %0 = sub i32 0, %rValue
  ret i32 %0
}

define private i32 @__operator_BIT_NOT__Integer(i32 %rValue) inlinehint {
entry:
  %0 = xor i32 %rValue, -1
  ret i32 %0
}

define private i32 @__operator_PRE_INC__Integer(i32* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i32* %lValue
  %1 = add i32 %0, 1
  store i32 %1, i32* %lValue
  ret i32 %1
}

define private i32 @__operator_PRE_DEC__Integer(i32* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i32* %lValue
  %1 = sub i32 %0, 1
  store i32 %1, i32* %lValue
  ret i32 %1
}

define private i32 @__operator_POST_INC__Integer(i32* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i32* %lValue
  %1 = add i32 %0, 1
  store i32 %1, i32* %lValue
  ret i32 %0
}

define private i32 @__operator_POST_DEC__Integer(i32* noalias nocapture %lValue) inlinehint {
entry:
  %0 = load i32* %lValue
  %1 = sub i32 %0, 1
  store i32 %1, i32* %lValue
  ret i32 %0
}

define private i32 @__operator_ADD__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = add i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_SUB__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = sub i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_MUL__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = mul i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_DIV__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %castToString = alloca %String
  %0 = icmp ne i32 %rhsRValue, 0
  br i1 %0, label %nonZero, label %zero

nonZero:                                          ; preds = %entry
  %1 = sdiv i32 %lhsRValue, %rhsRValue
  ret i32 %1

zero:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @13)
  call void @__String__Report(%String* %castToString)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret i32 0
}

define private i32 @__operator_MOD__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %castToString = alloca %String
  %0 = icmp ne i32 %rhsRValue, 0
  br i1 %0, label %nonZero, label %zero

nonZero:                                          ; preds = %entry
  %1 = srem i32 %lhsRValue, %rhsRValue
  ret i32 %1

zero:                                             ; preds = %entry
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @15)
  call void @__String__Report(%String* %castToString)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret i32 0
}

define private i32 @__operator_BIT_OR__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = or i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_BIT_AND__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = and i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_BIT_XOR__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = xor i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_SHL__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = shl i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i32 @__operator_SHR__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = ashr i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define private i1 @__operator_EQ__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = icmp eq i32 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_NE__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = icmp ne i32 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GT__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = icmp sgt i32 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_GE__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = icmp sge i32 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LT__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = icmp slt i32 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i1 @__operator_LE__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = icmp sle i32 %lhsRValue, %rhsRValue
  ret i1 %0
}

define private i64 @__method_dataSize__Integer(i32 %thisRValue) inlinehint {
entry:
  ret i64 4
}

define private i8* @__method_data__Integer(i32* noalias nocapture %thisLValue) inlinehint {
entry:
  %0 = bitcast i32* %thisLValue to i8*
  ret i8* %0
}

define private i64 @__method_hash__Integer(i32 %rValue) inlinehint {
entry:
  %0 = zext i32 %rValue to i64
  ret i64 %0
}

define private i32 @__method_compare__Integer__Integer(i32 %lhsRValue, i32 %rhsRValue) inlinehint {
entry:
  %0 = sub i32 %lhsRValue, %rhsRValue
  ret i32 %0
}

define void @map2(i32 %input, float* noalias nocapture %output) inlinehint {
entry:
  %castToString = alloca %String
  %castToString1 = alloca %String
  %resultLValue = alloca %String
  %castToScalar = alloca float
  %castToScalar2 = alloca float
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @17)
  store %String null, %String* %castToString1
  call void @__construct_String__Integer(%String* %castToString1, i32 %input)
  store %String null, %String* %resultLValue
  call void @__operator_ADD__String__String(%String* %resultLValue, %String* %castToString, %String* %castToString1)
  call void @__String__Report(%String* %resultLValue)
  %0 = load %String* %castToString
  call void @__String_Release(%String %0)
  %1 = load %String* %castToString1
  call void @__String_Release(%String %1)
  %2 = load %String* %resultLValue
  call void @__String_Release(%String %2)
  %3 = call i32 @__operator_ADD__Integer__Integer(i32 %input, i32 1)
  store float 0.000000e+00, float* %castToScalar
  call void @__construct_Scalar__Integer(float* %castToScalar, i32 %3)
  %4 = load float* %castToScalar
  %5 = call float @__operator_DIV__Scalar__Scalar(float 1.000000e+00, float %4)
  %6 = call i32 @__operator_ADD__Integer__Integer(i32 %input, i32 1)
  store float 0.000000e+00, float* %castToScalar2
  call void @__construct_Scalar__Integer(float* %castToScalar2, i32 %6)
  %7 = load float* %castToScalar2
  %8 = call float @__operator_MUL__Scalar__Scalar(float %5, float %7)
  store float %8, float* %output
  %9 = load float* %castToScalar
  %10 = load float* %castToScalar2
  ret void
}

define void @entry() inlinehint {
entry:
  %va = alloca %Integer_VA
  %i = alloca i32
  %cap = alloca %Integer_AP
  %result = alloca %Integer_AP
  %castToString = alloca %String
  %castToString1 = alloca %String
  %resultLValue = alloca %String
  %am2 = alloca %Integer_AP
  %result2 = alloca %Integer_AP
  %castToString3 = alloca %String
  %castToString4 = alloca %String
  %i10 = alloca i64
  %castToSize = alloca i64
  %castToSize11 = alloca i64
  %castToString12 = alloca %String
  call void @__Integer_VA__Init(%Integer_VA* %va)
  store i32 0, i32* %i
  store i32 0, i32* %i
  br label %loopCheckPreCond

loopCheckPreCond:                                 ; preds = %loopCheckPostCond, %entry
  %0 = load i32* %i
  %1 = call i1 @__operator_LT__Integer__Integer(i32 %0, i32 10)
  br i1 %1, label %loopBody, label %loopEnd

loopBody:                                         ; preds = %loopCheckPreCond
  %2 = load i32* %i
  call void @__method_push__Integer_VA__Integer(%Integer_VA* %va, i32 %2)
  br label %loopStep

loopStep:                                         ; preds = %loopBody
  %3 = call i32 @__operator_PRE_INC__Integer(i32* %i)
  br label %loopCheckPostCond

loopCheckPostCond:                                ; preds = %loopStep
  br i1 true, label %loopCheckPreCond, label %loopEnd

loopEnd:                                          ; preds = %loopCheckPostCond, %loopCheckPreCond
  %4 = load i32* %i
  store %Integer_AP null, %Integer_AP* %cap
  store %Integer_AP null, %Integer_AP* %result
  %5 = bitcast %Integer_VA* %va to i8*
  call void @__MR_CreateConstArray(i8* bitcast (%Integer_AP @__Integer_VA__Adapter to i8*), i8* %5, i8* bitcast (%Integer_AP @__Integer_AP__Adapter to i8*), %Integer_AP* %result)
  call void @__Integer_AP__DefaultAssign(%Integer_AP* %result, %Integer_AP* %cap)
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @41)
  %6 = call i64 @__method_getCount__Integer_AP(%Integer_AP* %cap)
  store %String null, %String* %castToString1
  call void @__construct_String__Size(%String* %castToString1, i64 %6)
  store %String null, %String* %resultLValue
  call void @__operator_ADD__String__String(%String* %resultLValue, %String* %castToString, %String* %castToString1)
  call void @__String__Report(%String* %resultLValue)
  %7 = load %String* %castToString
  call void @__String_Release(%String %7)
  %8 = load %String* %castToString1
  call void @__String_Release(%String %8)
  %9 = load %String* %resultLValue
  call void @__String_Release(%String %9)
  store %Integer_AP null, %Integer_AP* %am2
  store %Integer_AP null, %Integer_AP* %result2
  call void @__MR_CreateArrayMap_4(i8* bitcast (void (i32, float*)* @map2 to i8*), i64 2, %Integer_AP* %cap, i8* bitcast (%Integer_AP @__Scalar_AP__Adapter to i8*), %Integer_AP* %result2)
  call void @__Scalar_AP__DefaultAssign(%Integer_AP* %result2, %Integer_AP* %am2)
  store %String null, %String* %castToString3
  call void @__construct_String__Scalar_AP(%String* %castToString3, %Integer_AP* %am2)
  call void @__String__Report(%String* %castToString3)
  %10 = load %String* %castToString3
  call void @__String_Release(%String %10)
  %11 = call i64 @__method_getCount__Scalar_AP(%Integer_AP* %am2)
  store %String null, %String* %castToString4
  call void @__construct_String__Size(%String* %castToString4, i64 %11)
  call void @__String__Report(%String* %castToString4)
  %12 = load %String* %castToString4
  call void @__String_Release(%String %12)
  store i64 0, i64* %i10
  store i64 0, i64* %castToSize
  call void @__construct_Size__Integer(i64* %castToSize, i32 0)
  %13 = load i64* %castToSize
  store i64 %13, i64* %i10
  br label %loopCheckPreCond5

loopCheckPreCond5:                                ; preds = %loopCheckPostCond8, %loopEnd
  %14 = load i64* %i10
  store i64 0, i64* %castToSize11
  call void @__construct_Size__Integer(i64* %castToSize11, i32 10)
  %15 = load i64* %castToSize11
  %16 = call i1 @__operator_LT__Size__Size(i64 %14, i64 %15)
  br i1 %16, label %loopBody6, label %loopEnd9

loopBody6:                                        ; preds = %loopCheckPreCond5
  %17 = load i64* %i10
  %18 = call float @__method_produce__Scalar_AP__Size(%Integer_AP* %am2, i64 %17)
  store %String null, %String* %castToString12
  call void @__construct_String__Scalar(%String* %castToString12, float %18)
  call void @__String__Report(%String* %castToString12)
  %19 = load %String* %castToString12
  call void @__String_Release(%String %19)
  br label %loopStep7

loopStep7:                                        ; preds = %loopBody6
  %20 = call i64 @__operator_PRE_INC__Size(i64* %i10)
  br label %loopCheckPostCond8

loopCheckPostCond8:                               ; preds = %loopStep7
  br i1 true, label %loopCheckPreCond5, label %loopEnd9

loopEnd9:                                         ; preds = %loopCheckPostCond8, %loopCheckPreCond5
  %21 = load i64* %i10
  %22 = load i64* %castToSize
  %23 = load i64* %castToSize11
  call void @__Integer_VA__Dispose(%Integer_VA* %va)
  call void @__Integer_AP__Release(%Integer_AP* %cap)
  call void @__Scalar_AP__Release(%Integer_AP* %am2)
  ret void
}

define private i64 @__method_size__Integer_VA(%Integer_VA* noalias nocapture %array) inlinehint {
entry:
  %0 = getelementptr inbounds %Integer_VA* %array, i32 0, i32 1
  %1 = load i64* %0
  ret i64 %1
}

define private void @__Integer_VA__DefaultAssign(%Integer_VA* noalias nocapture %dstLValue, %Integer_VA* noalias nocapture %srcRValue) inlinehint {
entry:
  %indexLValue = alloca i64
  %0 = getelementptr inbounds %Integer_VA* %dstLValue, i32 0, i32 1
  %1 = load i64* %0
  %2 = getelementptr inbounds %Integer_VA* %srcRValue, i32 0, i32 1
  %3 = load i64* %2
  %4 = getelementptr inbounds %Integer_VA* %dstLValue, i32 0, i32 0
  %5 = getelementptr inbounds %Integer_VA* %dstLValue, i32 0, i32 2
  %6 = load i32** %5
  store i64 %3, i64* %indexLValue
  br label %disposeCheckBB

disposeCheckBB:                                   ; preds = %disposeNextBB, %entry
  %7 = load i64* %indexLValue
  %8 = icmp ult i64 %7, %1
  br i1 %8, label %disposeNextBB, label %disposeDoneBB

disposeNextBB:                                    ; preds = %disposeCheckBB
  %9 = getelementptr i32* %6, i64 %7
  %10 = add i64 %7, 1
  store i64 %10, i64* %indexLValue
  br label %disposeCheckBB

disposeDoneBB:                                    ; preds = %disposeCheckBB
  store i64 %3, i64* %0
  %11 = icmp eq i64 %3, 0
  br i1 %11, label %zeroBB, label %nonZeroBB

zeroBB:                                           ; preds = %disposeDoneBB
  store i64 0, i64* %4
  %12 = icmp ne i32* %6, null
  br i1 %12, label %freeBB, label %doneBB

nonZeroBB:                                        ; preds = %disposeDoneBB
  %13 = call i64 @__NextPowTwoMinusOne(i64 %3)
  %14 = call i64 @__Max(i64 %13, i64 15)
  store i64 %14, i64* %4
  %15 = mul i64 %14, 4
  %16 = bitcast i32* %6 to i8*
  %17 = call i8* @realloc(i8* %16, i64 %15)
  %18 = bitcast i8* %17 to i32*
  store i32* %18, i32** %5
  store i64 0, i64* %indexLValue
  %19 = getelementptr inbounds %Integer_VA* %srcRValue, i32 0, i32 2
  %20 = load i32** %19
  br label %copyCheckBB

freeBB:                                           ; preds = %zeroBB
  %21 = bitcast i32* %6 to i8*
  call void @free(i8* %21)
  store i32* null, i32** %5
  br label %doneBB

copyCheckBB:                                      ; preds = %copyNextBB, %nonZeroBB
  %22 = load i64* %indexLValue
  %23 = icmp ult i64 %22, %3
  br i1 %23, label %copyNextBB, label %copyDoneBB

copyNextBB:                                       ; preds = %copyCheckBB
  %24 = getelementptr i32* %20, i64 %22
  %25 = load i32* %24
  %26 = getelementptr i32* %18, i64 %22
  store i32 0, i32* %26
  store i32 %25, i32* %26
  %27 = add i64 %22, 1
  store i64 %27, i64* %indexLValue
  br label %copyCheckBB

copyDoneBB:                                       ; preds = %copyCheckBB
  br label %doneBB

doneBB:                                           ; preds = %copyDoneBB, %freeBB, %zeroBB
  ret void
}

define private i64 @__NextPowTwoMinusOne(i64 %rValue) nounwind inlinehint {
entry:
  %0 = lshr i64 %rValue, 1
  %1 = or i64 %rValue, %0
  %2 = lshr i64 %1, 2
  %3 = or i64 %1, %2
  %4 = lshr i64 %3, 4
  %5 = or i64 %3, %4
  %6 = lshr i64 %5, 8
  %7 = or i64 %5, %6
  %8 = lshr i64 %7, 16
  %9 = or i64 %7, %8
  %10 = lshr i64 %9, 32
  %11 = or i64 %9, %10
  ret i64 %11
}

define private i64 @__Max(i64 %lhsRValue, i64 %rhsRValue) nounwind inlinehint {
entry:
  %0 = icmp uge i64 %lhsRValue, %rhsRValue
  br i1 %0, label %lhs, label %rhs

lhs:                                              ; preds = %entry
  ret i64 %lhsRValue

rhs:                                              ; preds = %entry
  ret i64 %rhsRValue
}

declare i8* @realloc(i8*, i64) nounwind

define private void @__method_resize__Integer_VA__Size(%Integer_VA* noalias nocapture %arrayLValue, i64 %newSizeRValue) inlinehint {
entry:
  %indexLValue = alloca i64
  %0 = getelementptr inbounds %Integer_VA* %arrayLValue, i32 0, i32 1
  %1 = load i64* %0
  %2 = icmp ne i64 %newSizeRValue, %1
  br i1 %2, label %resizeBB, label %doneBB

resizeBB:                                         ; preds = %entry
  store i64 %newSizeRValue, i64* %0
  %3 = getelementptr inbounds %Integer_VA* %arrayLValue, i32 0, i32 0
  %memberDatasLValue = getelementptr inbounds %Integer_VA* %arrayLValue, i32 0, i32 2
  %memberDatasRValue = load i32** %memberDatasLValue
  store i64 %newSizeRValue, i64* %indexLValue
  br label %disposeCheckBB

disposeCheckBB:                                   ; preds = %disposeNextBB, %resizeBB
  %4 = load i64* %indexLValue
  %5 = icmp ult i64 %4, %1
  br i1 %5, label %disposeNextBB, label %disposeDoneBB

disposeNextBB:                                    ; preds = %disposeCheckBB
  %6 = getelementptr i32* %memberDatasRValue, i64 %4
  %7 = add i64 %4, 1
  store i64 %7, i64* %indexLValue
  br label %disposeCheckBB

disposeDoneBB:                                    ; preds = %disposeCheckBB
  %8 = icmp eq i64 %newSizeRValue, 0
  br i1 %8, label %zeroBB, label %nonZeroBB

zeroBB:                                           ; preds = %disposeDoneBB
  store i64 0, i64* %3
  %9 = icmp ne i32* %memberDatasRValue, null
  br i1 %9, label %freeBB, label %doneBB

freeBB:                                           ; preds = %zeroBB
  %10 = bitcast i32* %memberDatasRValue to i8*
  call void @free(i8* %10)
  store i32* null, i32** %memberDatasLValue
  br label %zeroDoneBB

zeroDoneBB:                                       ; preds = %freeBB
  br label %resizeDoneBB

nonZeroBB:                                        ; preds = %disposeDoneBB
  %11 = load i64* %3
  %12 = icmp ugt i64 %newSizeRValue, %11
  br i1 %12, label %expandBB, label %expandDoneBB

expandBB:                                         ; preds = %nonZeroBB
  %13 = call i64 @__NextPowTwoMinusOne(i64 %newSizeRValue)
  %14 = call i64 @__Max(i64 %13, i64 15)
  store i64 %14, i64* %3
  %15 = mul i64 %14, 4
  %16 = bitcast i32* %memberDatasRValue to i8*
  %17 = call i8* @realloc(i8* %16, i64 %15)
  %18 = bitcast i8* %17 to i32*
  store i32* %18, i32** %memberDatasLValue
  br label %expandDoneBB

expandDoneBB:                                     ; preds = %expandBB, %nonZeroBB
  %expandMemberDatasRValue = phi i32* [ %18, %expandBB ], [ %memberDatasRValue, %nonZeroBB ]
  store i64 %1, i64* %indexLValue
  br label %initCheckBB

initCheckBB:                                      ; preds = %initNextBB, %expandDoneBB
  %19 = load i64* %indexLValue
  %20 = icmp ult i64 %19, %newSizeRValue
  br i1 %20, label %initNextBB, label %initDoneBB

initNextBB:                                       ; preds = %initCheckBB
  %21 = getelementptr i32* %expandMemberDatasRValue, i64 %19
  store i32 0, i32* %21
  %22 = add i64 %19, 1
  store i64 %22, i64* %indexLValue
  br label %initCheckBB

initDoneBB:                                       ; preds = %initCheckBB
  br label %resizeDoneBB

resizeDoneBB:                                     ; preds = %initDoneBB, %zeroDoneBB
  br label %doneBB

doneBB:                                           ; preds = %resizeDoneBB, %zeroBB, %entry
  ret void

entry1:                                           ; No predecessors!
  call void @__method_resize__Integer_VA__Size(%Integer_VA* %arrayLValue, i64 %newSizeRValue)
  ret void
}

define private i32 @__Integer_VA__ConstIndex(%Integer_VA* noalias nocapture %array, i64 %index, %ConstString* noalias nocapture %errorDesc) inlinehint {
entry:
  %0 = call i64 @__method_size__Integer_VA(%Integer_VA* %array)
  %1 = icmp ult i64 %index, %0
  br i1 %1, label %inRange, label %outOfRange

inRange:                                          ; preds = %entry
  %2 = call i32 @__Integer_VA__ConstIndexNoCheck(%Integer_VA* %array, i64 %index)
  ret i32 %2

outOfRange:                                       ; preds = %entry
  call void @__ThrowOutOfRangeException(%ConstString* @19, i64 %index, i64 %0, %ConstString* %errorDesc)
  ret i32 0
}

define private i32 @__Integer_VA__ConstIndexNoCheck(%Integer_VA* noalias nocapture %array, i64 %index) inlinehint {
entry:
  %0 = call i32* @__Integer_VA__NonConstIndexNoCheck(%Integer_VA* %array, i64 %index)
  %1 = load i32* %0
  ret i32 %1
}

define private void @__ThrowOutOfRangeException(%ConstString* nocapture %itemRValue, i64 %indexRValue, i64 %sizeRValue, %ConstString* nocapture %errorDescRValue) {
entry:
  %castToString = alloca %String
  %castToString1 = alloca %String
  %errorMsgA = alloca %String
  %castToString2 = alloca %String
  %errorMsgB = alloca %String
  %castToString3 = alloca %String
  %errorMsgC = alloca %String
  %castToString4 = alloca %String
  %errorMsg1PlusIndex = alloca %String
  %castToString5 = alloca %String
  %errorMsg1PlusIndexPlusErrorMsg2 = alloca %String
  %castToString6 = alloca %String
  %errorMsg1PlusIndexPlusErrorMsg2PlusSize = alloca %String
  %castToString7 = alloca %String
  %errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3 = alloca %String
  %0 = icmp ne %ConstString* %errorDescRValue, null
  br i1 %0, label %haveErrorDescBB, label %noErrorDescBB

haveErrorDescBB:                                  ; preds = %entry
  br label %goBB

noErrorDescBB:                                    ; preds = %entry
  br label %goBB

goBB:                                             ; preds = %noErrorDescBB, %haveErrorDescBB
  %errorDescConstStringRValue = phi %ConstString* [ %errorDescRValue, %haveErrorDescBB ], [ @21, %noErrorDescBB ]
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* %errorDescConstStringRValue)
  store %String null, %String* %castToString1
  call void @__construct_String__ConstString(%String* %castToString1, %ConstString* @23)
  store %String null, %String* %errorMsgA
  call void @__operator_ADD__String__String(%String* %errorMsgA, %String* %castToString, %String* %castToString1)
  store %String null, %String* %castToString2
  call void @__construct_String__ConstString(%String* %castToString2, %ConstString* %itemRValue)
  store %String null, %String* %errorMsgB
  call void @__operator_ADD__String__String(%String* %errorMsgB, %String* %errorMsgA, %String* %castToString2)
  %1 = load %String* %errorMsgA
  call void @__String_Release(%String %1)
  store %String null, %String* %castToString3
  call void @__construct_String__ConstString(%String* %castToString3, %ConstString* @25)
  store %String null, %String* %errorMsgC
  call void @__operator_ADD__String__String(%String* %errorMsgC, %String* %errorMsgB, %String* %castToString3)
  %2 = load %String* %errorMsgB
  call void @__String_Release(%String %2)
  store %String null, %String* %castToString4
  call void @__construct_String__Size(%String* %castToString4, i64 %indexRValue)
  store %String null, %String* %errorMsg1PlusIndex
  call void @__operator_ADD__String__String(%String* %errorMsg1PlusIndex, %String* %errorMsgC, %String* %castToString4)
  %3 = load %String* %errorMsgC
  call void @__String_Release(%String %3)
  store %String null, %String* %castToString5
  call void @__construct_String__ConstString(%String* %castToString5, %ConstString* @27)
  store %String null, %String* %errorMsg1PlusIndexPlusErrorMsg2
  call void @__operator_ADD__String__String(%String* %errorMsg1PlusIndexPlusErrorMsg2, %String* %errorMsg1PlusIndex, %String* %castToString5)
  %4 = load %String* %errorMsg1PlusIndex
  call void @__String_Release(%String %4)
  store %String null, %String* %castToString6
  call void @__construct_String__Size(%String* %castToString6, i64 %sizeRValue)
  store %String null, %String* %errorMsg1PlusIndexPlusErrorMsg2PlusSize
  call void @__operator_ADD__String__String(%String* %errorMsg1PlusIndexPlusErrorMsg2PlusSize, %String* %errorMsg1PlusIndexPlusErrorMsg2, %String* %castToString6)
  %5 = load %String* %errorMsg1PlusIndexPlusErrorMsg2
  call void @__String_Release(%String %5)
  store %String null, %String* %castToString7
  call void @__construct_String__ConstString(%String* %castToString7, %ConstString* @29)
  store %String null, %String* %errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3
  call void @__operator_ADD__String__String(%String* %errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3, %String* %errorMsg1PlusIndexPlusErrorMsg2PlusSize, %String* %castToString7)
  %6 = load %String* %errorMsg1PlusIndexPlusErrorMsg2PlusSize
  call void @__String_Release(%String %6)
  %7 = load %String* %castToString
  call void @__String_Release(%String %7)
  %8 = load %String* %castToString1
  call void @__String_Release(%String %8)
  %9 = load %String* %castToString2
  call void @__String_Release(%String %9)
  %10 = load %String* %castToString3
  call void @__String_Release(%String %10)
  %11 = load %String* %castToString4
  call void @__String_Release(%String %11)
  %12 = load %String* %castToString5
  call void @__String_Release(%String %12)
  %13 = load %String* %castToString6
  call void @__String_Release(%String %13)
  %14 = load %String* %castToString7
  call void @__String_Release(%String %14)
  call void @__KL__throwException(%Integer_AP @__String__Adapter, %String* %errorMsg1PlusIndexPlusErrorMsg2PlusSizePlusErrorMsg3)
  ret void
}

declare void @__KL__throwException(%Integer_AP, %String*)

define private i32* @__Integer_VA__NonConstIndex(%Integer_VA* noalias nocapture %array, i64 %index, %ConstString* noalias nocapture %errorDesc) inlinehint {
entry:
  %0 = call i64 @__method_size__Integer_VA(%Integer_VA* %array)
  %1 = icmp ult i64 %index, %0
  br i1 %1, label %inRange, label %outOfRange

inRange:                                          ; preds = %entry
  %2 = call i32* @__Integer_VA__NonConstIndexNoCheck(%Integer_VA* %array, i64 %index)
  ret i32* %2

outOfRange:                                       ; preds = %entry
  call void @__ThrowOutOfRangeException(%ConstString* @31, i64 %index, i64 %0, %ConstString* %errorDesc)
  ret i32* @__Integer__DefaultValue
}

define private i32* @__Integer_VA__NonConstIndexNoCheck(%Integer_VA* noalias nocapture %array, i64 %index) inlinehint {
entry:
  %0 = getelementptr inbounds %Integer_VA* %array, i32 0, i32 2
  %1 = load i32** %0
  %2 = getelementptr i32* %1, i64 %index
  ret i32* %2
}

define private void @__construct_Boolean__Integer_VA(i1* noalias nocapture %booleanLValue, %Integer_VA* noalias nocapture %arrayRValue) inlinehint {
entry:
  %0 = call i64 @__method_size__Integer_VA(%Integer_VA* %arrayRValue)
  %1 = icmp ne i64 %0, 0
  store i1 %1, i1* %booleanLValue
  ret void
}

define private void @__construct_String__Integer_VA(%String* noalias nocapture %stringLValue, %Integer_VA* noalias nocapture %arrayRValue) inlinehint {
entry:
  call void bitcast (void (i8*, %Integer_AP, %String*)* @__String__Cast to void (i8*, %Integer_VA*, %String*)*)(i8* bitcast (%Integer_AP @__Integer_VA__Adapter to i8*), %Integer_VA* %arrayRValue, %String* %stringLValue)
  ret void
}

declare void @__Integer_VA__Append(i8*, %Integer_VA*, %Integer_VA*)

define private void @__method_ASSIGN_OP_ADD__Integer_VA__Integer_VA(%Integer_VA* noalias nocapture %lhsLValue, %Integer_VA* noalias nocapture %rhsRValue) inlinehint {
entry:
  call void @__Integer_VA__Append(i8* bitcast (%Integer_AP @__Integer_VA__Adapter to i8*), %Integer_VA* %lhsLValue, %Integer_VA* %rhsRValue)
  ret void
}

define private void @__operator_ADD__Integer_VA__Integer_VA(%Integer_VA* noalias nocapture sret %returnVal, %Integer_VA* noalias nocapture %lhsRValue, %Integer_VA* noalias nocapture %rhsRValue) inlinehint {
entry:
  call void @__Integer_VA__DefaultAssign(%Integer_VA* %returnVal, %Integer_VA* %lhsRValue)
  call void @__method_ASSIGN_OP_ADD__Integer_VA__Integer_VA(%Integer_VA* %returnVal, %Integer_VA* %rhsRValue)
  ret void
}

define private void @__method_swap__Integer_VA__Size__Size(%Integer_VA* noalias nocapture %thisLValue, i64 %lhsRValue, i64 %rhsRValue) inlinehint {
entry:
  %tmp = alloca i32
  %0 = icmp eq i64 %lhsRValue, %rhsRValue
  br i1 %0, label %done, label %unequal

unequal:                                          ; preds = %entry
  store i32 0, i32* %tmp
  %1 = call i32* @__Integer_VA__NonConstIndex(%Integer_VA* %thisLValue, i64 %lhsRValue, %ConstString* @33)
  %2 = load i32* %1
  %3 = call i32* @__Integer_VA__NonConstIndex(%Integer_VA* %thisLValue, i64 %rhsRValue, %ConstString* @35)
  %4 = load i32* %3
  store i32 %2, i32* %tmp
  store i32 %4, i32* %1
  %5 = load i32* %tmp
  store i32 %5, i32* %3
  br label %done

done:                                             ; preds = %unequal, %entry
  ret void
}

define private void @__method_push__Integer_VA__Integer(%Integer_VA* noalias nocapture %thisLValue, i32 %memberRValue) inlinehint {
entry:
  %0 = call i64 @__method_size__Integer_VA(%Integer_VA* %thisLValue)
  %1 = add i64 %0, 1
  call void @__method_resize__Integer_VA__Size(%Integer_VA* %thisLValue, i64 %1)
  %2 = call i32* @__Integer_VA__NonConstIndex(%Integer_VA* %thisLValue, i64 %0, %ConstString* @37)
  store i32 %memberRValue, i32* %2
  ret void
}

define private i32 @__method_pop__Integer_VA(%Integer_VA* noalias nocapture %thisLValue) inlinehint {
entry:
  %result = alloca i32
  store i32 0, i32* %result
  call void @__Integer_VA__Pop(i8* bitcast (%Integer_AP @__Integer_VA__Adapter to i8*), %Integer_VA* %thisLValue, i32* %result)
  %0 = load i32* %result
  ret i32 %0
}

declare void @__Integer_VA__Pop(i8* nocapture, %Integer_VA* nocapture, i32* nocapture) nounwind inlinehint

define private void @__method_resize__Integer_VA__Integer(%Integer_VA* noalias nocapture %thisLValue, i32 %integerRValue) inlinehint {
entry:
  %castToSize = alloca i64
  store i64 0, i64* %castToSize
  call void @__construct_Size__Integer(i64* %castToSize, i32 %integerRValue)
  %0 = load i64* %castToSize
  call void @__method_resize__Integer_VA__Size(%Integer_VA* %thisLValue, i64 %0)
  ret void
}

define private i64 @__method_dataSize__Integer_VA(%Integer_VA* noalias nocapture %selfRValue) inlinehint {
entry:
  %0 = call i64 @__method_size__Integer_VA(%Integer_VA* %selfRValue)
  %1 = mul i64 %0, 4
  ret i64 %1
}

define private i8* @__method_data__Integer_VA(%Integer_VA* noalias nocapture %selfRValue) inlinehint {
entry:
  %0 = getelementptr inbounds %Integer_VA* %selfRValue, i32 0, i32 2
  %1 = load i32** %0
  %2 = bitcast i32* %1 to i8*
  ret i8* %2
}

define private void @__Integer_VA__Init(%Integer_VA* noalias nocapture %selfLValue) nounwind inlinehint {
entry:
  %0 = getelementptr inbounds %Integer_VA* %selfLValue, i32 0, i32 0
  store i64 0, i64* %0
  %1 = getelementptr inbounds %Integer_VA* %selfLValue, i32 0, i32 1
  store i64 0, i64* %1
  %2 = getelementptr inbounds %Integer_VA* %selfLValue, i32 0, i32 2
  store i32* null, i32** %2
  ret void
}

define private void @__construct_String__Integer_AP(%String* noalias nocapture %stringLValue, %Integer_AP* noalias nocapture %rValue) inlinehint {
entry:
  %castToString = alloca %String
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @39)
  %0 = load %String* %stringLValue
  %1 = load %String* %castToString
  call void @__String_Retain(%String %1)
  store %String %1, %String* %stringLValue
  call void @__String_Release(%String %0)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret void
}

define private void @__construct_Boolean__Integer_AP(i1* noalias nocapture %booleanLValue, %Integer_AP* noalias nocapture %rValue) inlinehint {
entry:
  %0 = load %Integer_AP* %rValue
  %1 = icmp ne %Integer_AP %0, null
  store i1 %1, i1* %booleanLValue
  ret void
}

define private i64 @__method_getCount__Integer_AP(%Integer_AP* noalias nocapture %rValue) inlinehint {
entry:
  %0 = call i64 @__Integer_AP__GetCount(%Integer_AP* %rValue)
  ret i64 %0
}

declare i64 @__Integer_AP__GetCount(%Integer_AP*)

define private i32 @__method_produce__Integer_AP__Size(%Integer_AP* noalias nocapture %rValue, i64 %rValue1) inlinehint {
entry:
  %result = alloca i32
  call void @__Integer_AP__Produce(%Integer_AP* %rValue, i64 %rValue1, i32* %result)
  %0 = load i32* %result
  ret i32 %0
}

declare void @__Integer_AP__Produce(%Integer_AP*, i64, i32*)

declare void @__MR_CreateConstArray(i8*, i8*, i8*, %Integer_AP*)

declare void @__Integer_AP__DefaultAssign(%Integer_AP*, %Integer_AP*)

define private void @__construct_String__Scalar_AP(%String* noalias nocapture %stringLValue, %Integer_AP* noalias nocapture %rValue) inlinehint {
entry:
  %castToString = alloca %String
  store %String null, %String* %castToString
  call void @__construct_String__ConstString(%String* %castToString, %ConstString* @43)
  %0 = load %String* %stringLValue
  %1 = load %String* %castToString
  call void @__String_Retain(%String %1)
  store %String %1, %String* %stringLValue
  call void @__String_Release(%String %0)
  %2 = load %String* %castToString
  call void @__String_Release(%String %2)
  ret void
}

define private void @__construct_Boolean__Scalar_AP(i1* noalias nocapture %booleanLValue, %Integer_AP* noalias nocapture %rValue) inlinehint {
entry:
  %0 = load %Integer_AP* %rValue
  %1 = icmp ne %Integer_AP %0, null
  store i1 %1, i1* %booleanLValue
  ret void
}

define private i64 @__method_getCount__Scalar_AP(%Integer_AP* noalias nocapture %rValue) inlinehint {
entry:
  %0 = call i64 @__Scalar_AP__GetCount(%Integer_AP* %rValue)
  ret i64 %0
}

declare i64 @__Scalar_AP__GetCount(%Integer_AP*)

define private float @__method_produce__Scalar_AP__Size(%Integer_AP* noalias nocapture %rValue, i64 %rValue1) inlinehint {
entry:
  %result = alloca float
  call void @__Scalar_AP__Produce(%Integer_AP* %rValue, i64 %rValue1, float* %result)
  %0 = load float* %result
  ret float %0
}

declare void @__Scalar_AP__Produce(%Integer_AP*, i64, float*)

declare void @__MR_CreateArrayMap_4(i8*, i64, %Integer_AP*, i8*, %Integer_AP*)

declare void @__Scalar_AP__DefaultAssign(%Integer_AP*, %Integer_AP*)

define private void @__Integer_VA__Dispose(%Integer_VA* noalias nocapture %selfLValue) nounwind inlinehint {
entry:
  %index = alloca i64
  %0 = getelementptr inbounds %Integer_VA* %selfLValue, i32 0, i32 1
  %1 = load i64* %0
  %2 = getelementptr inbounds %Integer_VA* %selfLValue, i32 0, i32 2
  %3 = load i32** %2
  store i64 0, i64* %index
  br label %membersCheck

membersCheck:                                     ; preds = %membersNext, %entry
  %4 = load i64* %index
  %5 = icmp ult i64 %4, %1
  br i1 %5, label %membersNext, label %membersDone

membersNext:                                      ; preds = %membersCheck
  %6 = getelementptr i32* %3, i64 %4
  %7 = add i64 %4, 1
  store i64 %7, i64* %index
  br label %membersCheck

membersDone:                                      ; preds = %membersCheck
  %8 = icmp ne i32* %3, null
  br i1 %8, label %free, label %done

free:                                             ; preds = %membersDone
  %9 = bitcast i32* %3 to i8*
  call void @free(i8* %9)
  br label %done

done:                                             ; preds = %free, %membersDone
  ret void
}

declare void @__Integer_AP__Release(%Integer_AP*)

declare void @__Scalar_AP__Release(%Integer_AP*)
