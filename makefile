IDIR =include
CC=gcc -w

ODIR=obj
SRCDIR=src

start:	start.c $(ODIR)/graph.o $(ODIR)/priority-queue.o $(ODIR)/heap.o $(ODIR)/node.o $(IDIR)/AStar.h $(IDIR)/GUI.h
	$(CC) -o start start.c $(ODIR)/graph.o $(ODIR)/priority-queue.o $(ODIR)/heap.o $(ODIR)/node.o -lm `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

$(ODIR)/graph.o:	$(SRCDIR)/graph.c $(IDIR)/Graph.h
	$(CC) -c -o $(ODIR)/graph.o -c $(SRCDIR)/graph.c

$(ODIR)/priority-queue.o:	$(SRCDIR)/priority-queue.c $(IDIR)/PriorityQueue.h 
	$(CC) -c -o $(ODIR)/priority-queue.o -c $(SRCDIR)/priority-queue.c

$(ODIR)/heap.o:	$(SRCDIR)/heap.c $(IDIR)/Heap.h
	$(CC) -c -o $(ODIR)/heap.o -c $(SRCDIR)/heap.c

$(ODIR)/node.o: $(SRCDIR)/node.c $(IDIR)/Node.h
	$(CC) -c -o $(ODIR)/node.o -c $(SRCDIR)/node.c

clean:
	rm -f $(ODIR)/*.o
	rm start