VERBOSE ?= 0
V := 
ifeq ($(VERBOSE), 0)
	V := @
endif

NO_COL  := \033[0m
RED     := \033[0;31m
GREEN   := \033[0;32m
BLUE    := \033[0;34m
YELLOW  := \033[0;33m
BLINK   := \033[33;5m

ifeq ($(VERBOSE), 0)
define printtask
	@printf "$(1) $(YELLOW)$(2)$(NO_COL)...$(NO_COL)\n"
endef
endif