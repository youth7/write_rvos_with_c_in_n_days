

.macro save_reg base
    sw ra, 0(\base)
    sw sp, 4(\base)
    sw gp, 8(\base)
    sw tp, 12(\base)

    sw t0, 16(\base) 
    sw t1, 20(\base) 
    sw t2, 24(\base) 
                 
    sw s0, 28(\base)
    sw s1, 32(\base)

    sw a0, 36(\base) 
    sw a1, 40(\base) 
    sw a2, 44(\base) 
    sw a3, 48(\base)
    sw a4, 52(\base) 
    sw a5, 56(\base) 
    sw a6, 60(\base) 
    sw a7, 64(\base) 
    
    sw s2, 68(\base) 
    sw s3, 72(\base) 
    sw s4, 76(\base) 
    sw s5, 80(\base) 
    sw s6, 84(\base) 
    sw s7, 88(\base) 
    sw s8, 92(\base) 
    sw s9, 96(\base) 
    sw s10, 100(\base)
    sw s11, 104(\base)

    sw t3, 108(\base) 
    sw t4, 112(\base) 
    sw t5, 116(\base) 
    # 无需保存a6，因为a6此时已经被修改过，真正的a6存储在mscratch中，需要后续用额外的手段保存
.endm



.macro resore_reg base
    lw ra, 0(\base)
    lw sp, 4(\base)
    lw gp, 8(\base)
    lw tp, 12(\base)

    lw t0, 16(\base) 
    lw t1, 20(\base) 
    lw t2, 24(\base) 
                 
    lw s0, 28(\base)
    lw s1, 32(\base)

    lw a0, 36(\base) 
    lw a1, 40(\base) 
    lw a2, 44(\base) 
    lw a3, 48(\base)
    lw a4, 52(\base) 
    lw a5, 56(\base) 
    lw a6, 60(\base) 
    lw a7, 64(\base) 
    
    lw s2, 68(\base) 
    lw s3, 72(\base) 
    lw s4, 76(\base) 
    lw s5, 80(\base) 
    lw s6, 84(\base) 
    lw s7, 88(\base) 
    lw s8, 92(\base) 
    lw s9, 96(\base) 
    lw s10, 100(\base)
    lw s11, 104(\base)

    lw t3, 108(\base) 
    lw t4, 112(\base) 
    lw t5, 116(\base)
    lw t6, 120(\base)
    # t6必须作为基寄存器，即最后一个保存的寄存器必定是基寄存器
.endm

# 注意上述宏展开运行时，全程都需要读取base这个寄存器的值，因此base的值全程不能被修改。
# 而base寄存器又必须是32个通用寄存器之一，因此base必须取t6，这样才能达到上述效果



.text
.global switch_to
.align 4
# 主要流程如下：
# mscratch指向当前运行的任务的上下文，它被初始化为0
# 先判断mscratch，以确定当前有无任务正在运行
# 如果mscratch=0，说明当前无任务运行，此时无需保存上下文，直接驱动第一个任务
# 如果mscratch≠0，说明当前有任务运行，此时需要先保存当前任务上下文到内存中，再驱动下一个任务 

# switch_to在C语言中的原型为：void switch_to(struct context *);
# next任务的地址存在寄存器a0中
switch_to:
    # 先判断mscratch的值，因为mscratch无法直接用在分支跳转指令中，因此我们先将它读取到寄存器t6
    # 选t6首先因为它是临时寄存器，同时它位于寄存器列表的最后一个，这对resore_reg来说至关重要！
    csrrw t6, mscratch, t6
    beqz t6, resotre_reg


save_all_reg:
    save_reg t6 # 此时t6指向当前任务的context
    
    # 因为t6被修改了（原来的值此时位于mscratch），因此我们调用save_reg时未保存它，现在手动将它写入到内存中
    csrr t5, mscratch         # 读取原始t8的值到t5中，准备保存
    sw t5, 120(t6)            # t6的偏移量是120

# 把next task的上下文加载到寄存器中
resotre_reg:
    # 设置mscratch为a0，则mscratch指向了next task
    csrw mscratch, a0
    mv	t6, a0  #这一步至关重要，不能直接resore_reg a0，否则a0被覆盖之后，后面依赖a0的指令全都出错
    resore_reg t6
    # ret展开后是：jalr x0, x1, 0，即跳转到x1(ra)寄存器的地址开始运行，并抛弃返回地址的值。
    ret

.end