
CUTEST_TESTBIN := waitress_test


CFLAGS += -Wall -pedantic -g 

all: $(CUTEST_TESTBIN)

run_test: $(CUTEST_TESTBIN)
	./$(CUTEST_TESTBIN)

$(CUTEST_TESTBIN): CuTest.o libwaitressTest.o AllTests.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lgnutls

CuTest.o: CuTest.c CuTest.h

clean:
	rm -f *.o $(CUTEST_TESTBIN)
