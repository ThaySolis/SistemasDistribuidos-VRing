all: tempo tempo1 tempo2 tempo3 tempo4

tempo: tempo.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tempo.o smpl.o rand.o -lm
	
tempo1: tempo1.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tempo1.o smpl.o rand.o -lm
	
tempo2: tempo2.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tempo2.o smpl.o rand.o -lm
	
tempo3: tempo3.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tempo3.o smpl.o rand.o -lm
	
tempo4: tempo4.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tempo4.o smpl.o rand.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

tempo.o: tempo.c smpl.h
	$(COMPILE.c) -g  tempo.c
	
tempo1.o: tempo1.c smpl.h
	$(COMPILE.c) -g  tempo1.c
	
tempo2.o: tempo2.c smpl.h
	$(COMPILE.c) -g  tempo2.c
	
tempo3.o: tempo3.c smpl.h
	$(COMPILE.c) -g  tempo3.c
	
tempo4.o: tempo4.c smpl.h
	$(COMPILE.c) -g  tempo4.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

clean:
	$(RM) *.o tempo tempo1 tempo2 tempo3 tempo4 relat saida

