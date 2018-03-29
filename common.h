#define CLIENT_BUFFER_SIZE 100//�ͻ�������Ļ�������С
#define SERVER_BUFFER_SIZE 1024//����˻�������С
#define MAX_NICKNAME_LENGTH 10//�ǳƵ���󳤶�
const char IP_OF_SERVER[] = "192.168.0.123";
const unsigned short PORT_OF_SERVER = 8888;
void init() { //windows�����г�ʼ����linux ��û��
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("��ʼ��ʧ�ܣ���������: %d",WSAGetLastError());
        exit(1);
    }
}

int createSocket() {

    /**
        ����һ���հ׵�socket��socket��һ��communication�е�endpoint��
        1��communication����2��endpoint��һ����server�ˣ�һ����client��
        1��������communication�����������ֱ�ǣ�Э������+ Դip+ port�� + ��Э������+ Ŀ��ip+ port��

        ��1��������ʾsocket֧�ֵ�Э���
            Linux�¿��õ���������:
            AF_UNIX, AF_LOCAL   Local communication              unix(7)
            AF_INET             IPv4 Internet protocols          ip(7)
            AF_INET6            IPv6 Internet protocols          ipv6(7)
            AF_IPX              IPX - Novell protocols
            AF_NETLINK          Kernel user interface device     netlink(7)
            AF_X25              ITU-T X.25 / ISO-8208 protocol   x25(7)
            AF_AX25             Amateur radio AX.25 protocol
            AF_ATMPVC           Access to raw ATM PVCs
            AF_APPLETALK        AppleTalk                        ddp(7)
            AF_PACKET           Low level packet interface       packet(7)
            AF_ALG              Interface to kernel crypto API

            windows�������Ͳμ�����ĵ���
            https://msdn.microsoft.com/en-us/library/windows/desktop/ms740506(v=vs.85).aspx

        ��2��������ʾsocket�����ͣ�����ʽ�ġ����ݱ�ʽ�ĵȵ�
            Linux�����������£�
            windows�����������£�
    **/
    SOCKET serverSocket =  socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == INVALID_SOCKET) {
        printf("���������socketʧ�ܣ���������%d", WSAGetLastError());
        exit(0);
    }
    return serverSocket;
}

void bindSocketWithAddress(SOCKET socket, struct sockaddr_in address, unsigned int length) {
    /**
        bind������һ��socket��һ��address����������
        ��csapp�еĽ��ܿ��Կ�����
        socket_in�� sockaddr�����ƣ�����ǰ�߻�ר������8��0�ֽ�������Լ�ʹ�ü��ݺ���
    **/
    int result = bind(socket, (struct sockaddr *)&address, length);
    if(result == SOCKET_ERROR) {
        printf("��addressʧ�ܣ���������%d", WSAGetLastError());
        exit(0);
    }
}
