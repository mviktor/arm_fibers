;@   Copyright 2021 Viktor Mladenovski
;@
;@   Licensed under the Apache License, Version 2.0 (the "License");
;@   you may not use this file except in compliance with the License.
;@   You may obtain a copy of the License at
;@
;@       http://www.apache.org/licenses/LICENSE-2.0
;@
;@   Unless required by applicable law or agreed to in writing, software
;@   distributed under the License is distributed on an "AS IS" BASIS,
;@   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;@   See the License for the specific language governing permissions and
;@   limitations under the License.

.global yield
.global asm_start_fiber
.global asm_jump_to_running_fiber

yield:
    stmfd sp!, {r0-r12, lr}
    mov r0, sp
    b save_and_schedule_next

asm_jump_to_running_fiber:
    mov sp, r0
    ldmfd sp!, {r0-r12, lr}
    bx lr

asm_start_fiber:
    @ r0 - arg
    @ r1 - stack top
    @ r2 - func
    mov sp, r1
    mov r3, #0
    mov r4, #0
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    mov r9, #0
    mov r10, #0
    mov r11, #0
    mov r12, #0
    ldr r14, =fiber_destroy_self
    bx r2
    
