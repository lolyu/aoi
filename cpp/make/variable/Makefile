# Recursive variable. This will print "one later" below
one = one ${later_variable}
# Simply expanded variable. This will print "two" below
two := two ${later_variable}

later_variable = later

all: 
	@echo $(one)
	@echo $(two)