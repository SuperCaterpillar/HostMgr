NAME         := HostNetFilter
obj-m        := $(NAME).o
$(NAME)-objs := main.o NetFilter.o NetConnect.o NetLink.o
KERNEL_DIR   := /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS += -D__KMODULE_NAME=$(NAME) -DNDEBUG

ifeq ($(shell uname -m),x86_64)


endif

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean
