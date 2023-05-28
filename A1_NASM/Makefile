MAKEFLAGS += -s
TARGET_A := A1A_200101015
TARGET_B := A1B_200101015
.PHONY: clean all

all : 
	$(MAKE) clean
	$(MAKE) $(TARGET_A)
	$(MAKE) $(TARGET_B)

$(TARGET_A): $(TARGET_A).o
	gcc -o $(TARGET_A) $(TARGET_A).o -no-pie

$(TARGET_A).o: $(TARGET_A).asm 
	nasm -f elf64 -o $(TARGET_A).o -l $(TARGET_A).lst $(TARGET_A).asm
	
$(TARGET_B): $(TARGET_B).o
	gcc -o $(TARGET_B) $(TARGET_B).o -no-pie

$(TARGET_B).o: $(TARGET_B).asm 
	nasm -f elf64 -o $(TARGET_B).o -l $(TARGET_B).lst $(TARGET_B).asm

clean: 
	rm $(TARGET_A) $(TARGET_B) *.o *.lst -f
