

#include <stdio.h>
// #include <winsock2.h>

void printWelcomeMessage(){
    printf("This is a message from the new function. Welcome!\n");
}


int main(){
    printWelcomeMessage();
    char name[100];
    printf("What is your name?\n");
    // scanf("%s",name);

    sprintf(name,"%s","jping");

    printf("Hello,%s,nice to meet you!\n",name);

    // SOCKET client_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    // printf("Compile successfully!");

    // 打印循环索引值
    for (int i = 0; i < 5; i++) {
        printf("Loop index: %d\n", i);
    }

   return 0;
}
