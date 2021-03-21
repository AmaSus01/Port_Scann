// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h> // различные типы данных
// #include <sys/socket.h> //Самый главный файл, в нем находятся базовые функции сокетов и структуры данных.
// #include <netinet/in.h> //Семейства адресов/протоколов PF_INET (для IPv4) и (PF_INET6 для IPv6). Включают в себя IP-адреса, а также номера портов TCP и UDP.
// #include <arpa/inet.h> //Функции для работы с числовыми IP-адресами.
// #include <netdb.h> //Функции для преобразования протокольных имен и имен хостов в числовые адреса.
// int sock (const char * transport){
//     struct protoent *ppe; //указатель на запись с информацией о протоколе
//     int s, type;  //дескриптор и тип сокета


// ppe = getprotobyname(transport); //преобразую имя транспортного протокола в корректный номер протокола

// //определяю тип сокета, согласно имени транспортного протокола
// if (strcmp(transport, "udp") == 0){
//     type = SOCK_DGRAM; //если udp - указывается SOCK_DGRAM
// } else {
//     type = SOCK_STREAM; // в ином случае, если tcp указывется SOCK_STREAM
// }
// //создаем новый сокет, передав необходимые параметры
// s = socket (PF_INET, type, ppe->p_proto);
// return s;
//}


#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc , char **argv)
{
	struct hostent *host;
	int err, i , sock ,start , end;
	char hostname[80];
	struct sockaddr_in sa;
	
	//Get the hostname to scan
	printf("Enter hostname or IP : ");
	fgets(hostname, sizeof(hostname), stdin);
	
	//Get start port number
	printf("\nEnter start port number : ");
	scanf("%d" , &start);
	
	//Get end port number
	printf("Enter end port number : ");
	scanf("%d" , &end);

	//Initialise the sockaddr_in structure
	strncpy((char*)&sa , "" , sizeof sa);
	sa.sin_family = AF_INET;
	
	//direct ip address, use it
	if(isdigit(hostname[0]))
	{
		printf("Doing inet_addr...");
		sa.sin_addr.s_addr = inet_addr(hostname);
		printf("Done\n");
	}
	//Resolve hostname to ip address
	else if( (host = gethostbyname(hostname)) != 0)
	{
		printf("Doing gethostbyname...");
		strncpy((char*)&sa.sin_addr , (char*)host->h_addr , sizeof sa.sin_addr);
		printf("Done\n");
	}
	else
	{
		herror(hostname);
		exit(2);
	}
	
	//Start the port scan loop
	printf("Starting the portscan loop : \n");
	for( i = start ; i <= end ; i++) 
	{
		//Fill in the port number
		sa.sin_port = htons(i);
		//Create a socket of type internet
		sock = socket(AF_INET , SOCK_STREAM , 0);
		
		//Check whether socket created fine or not
		if(sock < 0) 
		{
			perror("\nSocket");
			exit(1);
		}
		//Connect using that socket and sockaddr structure
		err = connect(sock , (struct sockaddr*)&sa , sizeof sa);
		
		//not connected
		if( err < 0 )
		{
			//printf("%s %-5d %s\r" , hostname , i, strerror(errno));
			fflush(stdout);
		}
		//connected
		else
		{
			printf("%-5d open\n",  i);
		}
		close(sock);
	}
	
	printf("\r");
	fflush(stdout);
	return(0);
}