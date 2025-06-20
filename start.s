.global	_start
# 声明全局符号start_kernel，它定义在kernel.c中
.global start_kernel

_start:
	# 初始化栈，使sp指向栈顶
	la sp, stack_top
	j start_kernel

# 不加这句对齐的话，stack_bottom不会对齐，而stack_top为了对齐，会强行增加栈的大小，最终使得栈不等于1024，要注意
.balign 16
stack_bottom:
	# 开辟一段空间作为栈
    .skip 1024
stack_top:
