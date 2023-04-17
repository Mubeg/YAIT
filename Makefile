test_fast:
	make fast_install
	make test
	make clear
test_long:
	make long_install
	make test
	make clear

test:
	make test1
	make test2
	make test3
	make test4

install:
	c++ --std=c++14 main.cpp -o TapeSort
	chmod +x TapeSort
fast_install:
	c++ --std=c++14 -DNO_SLEEP -DFAST_SLEEP -DLONG_WAIT main.cpp -o TapeSort
	chmod +x TapeSort
long_install:
	c++ --std=c++14 -DLONG_WAIT main.cpp -o TapeSort
	chmod +x TapeSort

clear:
	rm TapeSort


test1:
	cp tests/01/input.txt tests/01/in.txt
	touch tests/01/out.txt
	./TapeSort 6 0 tests/01/in.txt tests/01/out.txt
	cmp -s tests/01/result.txt tests/01/out.txt; \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            echo "Test 1 Success"; \
			rm tests/01/in.txt tests/01/out.txt;\
    else \
            echo "Test 1 Failed"; \
    fi
test2:
	cp tests/02/input.txt tests/02/in.txt
	touch tests/02/out.txt
	./TapeSort 1000 0 tests/02/in.txt tests/02/out.txt
	cmp -s tests/02/result.txt tests/02/out.txt; \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            echo "Test 2 Success"; \
			rm tests/02/in.txt tests/02/out.txt;\
    else \
            echo "Test 2 Failed"; \
    fi
test3:
	cp tests/03/input.txt tests/03/in.txt
	touch tests/03/out.txt
	./TapeSort 100000 0 tests/03/in.txt tests/03/out.txt
	cmp -s tests/03/result.txt tests/03/out.txt; \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            echo "Test 3 Success"; \
			rm tests/03/in.txt tests/03/out.txt;\
    else \
            echo "Test 3 Failed"; \
    fi
test4:
	cp tests/04/input.txt tests/04/in.txt
	touch tests/04/out.txt
	./TapeSort 30 0 tests/04/in.txt tests/04/out.txt
	cmp -s tests/04/result.txt tests/04/out.txt; \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            echo "Test 4 Success"; \
			rm tests/04/in.txt tests/04/out.txt;\
    else \
            echo "Test 4 Failed"; \
    fi

default:
	make test_fast