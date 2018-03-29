#include<stdio.h>
#include<winsock2.h>
#include"common.h"
#include"common_client.h"
char buffer[SERVER_BUFFER_SIZE] = {0};//���ն˵Ļ����С���ٺͷ���˱���һ��
void display(SOCKET clientSocket) {
    int numberOfReceive = 0;
    do {
        numberOfReceive =  recv(clientSocket, buffer, SERVER_BUFFER_SIZE, 0);
        buffer[numberOfReceive] = 0;
        printf("%s\n", buffer);
    } while(numberOfReceive>0);
    puts("�����ʧȥ����\n");
}
int main(void) {
    init();
    SOCKET clientSocket = createSocket();
    struct sockaddr_in clientAddress = createClientAddressWithPort(7777);
    struct sockaddr_in serverAddress = getServerAddress();
    bindSocketWithAddress(clientSocket, clientAddress, sizeof(clientAddress));
    connectToServer(clientSocket, serverAddress);
    display(clientSocket);
    return 0;
}
