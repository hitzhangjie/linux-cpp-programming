#include <stdio.h>

typedef struct
{
    char sex;
    int age;
} Person;

// because it's packed, so sizeof is 5 bytes
// 1 + 4 = 5 bytes
typedef struct __attribute__((packed))
{
    char sex;
    int age;
} Student;

// this way: 1 + 4 + 3padding + 4 = 12 bytes
struct StudentX
{
    char sex __attribute__((aligned(1)));
    int age __attribute__((packed));
    int xxx __attribute__((aligned(4)));
};

// this way, the sizeof StudentY will be 16 bytes
// 8 + 8 = 16 bytes
struct StudentY
{
    char sex __attribute__((aligned(8)));
    int age __attribute__((aligned(8)));
};

// this way, add attributes to the struct means this struct:
// - aligned(4) : sizeof is 8
// - aligned(8) : sizeof is 8
// - aligned(16) : sizeof is 16
// - aligned(32) : sizeof is 32
//
// i don't know how aligned affects struct members, it looks like
// telling the compiler to try to align the struct members in this way:
// - if aligned (n) is too small, use default value, like char:1 int:4
// - if aligned (n) is bigger than default values, try to align to bigger boundary.
typedef struct __attribute__((aligned(4)))
{
    char sex;
    int age;
} StudentZ;

int main(int argc, char **argv)
{
    int n = 100;
    printf("n: %d\n", n);
    printf("sizeof(int): %lu, address: %p\n", sizeof(n), &n);

    Person p;
    printf("sizeof(person): %lu\n", sizeof(p));
    printf("person address: %p\n", &p);
    printf("person.sex address: %p\n", &p.sex);
    printf("person.age address: %p\n", &p.age);

    Student s;
    printf("sizeof(student): %lu\n", sizeof(s));
    struct StudentX x;
    printf("sizeof(studentx): %lu\n", sizeof(x));

    struct StudentY y;
    printf("sizeof(studenty): %lu\n", sizeof(y));

    StudentZ z;
    printf("sizeof(studentz): %lu\n", sizeof(z));
    printf("address of z: %p\n", &z);

    return 0;
}