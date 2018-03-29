struct sockaddr_in getServerAddress() {
    /**
        ����Զ�̷�������address������
        1��Э���
        2���˿�
        3���������ӽ�����ip
    **/
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( PORT_OF_SERVER );
    address.sin_addr.s_addr = inet_addr(IP_OF_SERVER);
    return address;

}

struct sockaddr_in createClientAddressWithPort(unsigned short port) {
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons( port);
    char hostName[255] = {0};
    gethostname(hostName, 255);
    /*
    struct hostent�ṹ��������
    struct hostent {
        char    *h_name;         //official name of host
        char    **h_aliases;     //alias list
        int     h_addrtype;      //host address type
        int     h_length;        //length of address
        char    **h_addr_list;   //list of addresses
    }
    ��Ϊ�������ݣ������#define h_addr h_addr_list[0]������h_addr��ʵ����h_addr_list[0]

    h_addr_list��man��˵���ǣ�
    An  array  of  pointers  to network addresses for the host (in network byte order), terminated by a NULL pointer.
    ��˿�֪h_addr_list����ָ���������ַ��ָ�룬���struct in_addr�Ľṹ�Ǽ��ݵģ����ǿתû������

    struct in_addr λ��/usr/include/linux/in.h ,�����ǣ�

    struct in_addr {
            __be32  s_addr;//Internet address
    };
    ˵��s_addr��һ�����˳���ʾ�ĵ�ַ����h_addr_list�Ǽ��ݵ�
    */
    struct hostent *host = gethostbyname(hostName);
    printf("ip �� %s\n", inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
    clientAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
    return clientAddress;
};

void connectToServer(SOCKET clientSocket, struct sockaddr_in serverAddress) {
    printf("�������ӵ�������%s\n", inet_ntoa(serverAddress.sin_addr));
    int result = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (result < 0) {
        printf("���ӷ�����ʧ�ܣ���������%d\n", WSAGetLastError());
        exit(0);
    }
    puts("�Ѿ��ɹ����ӷ�����\n");

}
















