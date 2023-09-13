#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

int main() {
    int port = 80;
    string host = "pmk.tversu.ru";
    string path = "/";

    // Connect to the server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *server = gethostbyname(host.c_str());
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error: could not connect to server" << endl;
        return 1;
    }

    // Send a GET request for the webpage
    ostringstream request;
    request << "GET " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    string request_str = request.str();

    if (send(sockfd, request_str.c_str(), request_str.length(), 0) < 0) {
        cerr << "Error: could not send request" << endl;
        return 1;
    }

    // Receive the webpage's source code and print it to the console
    char buffer[1024];
    int n;
    while ((n = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        cout.write(buffer, n);
    }
    if (n < 0) {
        cerr << "Error: could not receive response" << endl;
        return 1;
    }

    // Close the socket
    close(sockfd);

    return 0;
}