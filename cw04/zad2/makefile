all:
	make rm
	make main
	make test

rm:
	rm main_siginfo

main_siginfo:
	gcc -Wall -fsanitize=address main_siginfo.c -o main_siginfo

test_sigill:
	./main_siginfo SIGILL

test_child:
	./main_siginfo SIGCHLD

#test_SIGFPE:
#	./main_siginfo SIGFPE

test_SIGINT:
	./main_siginfo SIGINT

rm_nodefer:
	rm main_nodefer

main_nodefer:
	gcc -Wall -fsanitize=address main_nodefer.c -o main_nodefer

test_nodefer:
	./main_nodefer

rm_resethand:
	rm main_resethand

main_resethand:
	gcc -Wall main_resethand.c -o main_resethand
test_resethand:
	./main_resethand