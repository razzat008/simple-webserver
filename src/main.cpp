#include "../lib/config.h"
#include <cstring>
// handles messages from clients
void *handle_client(void *client_fd);
// void send_response(int client_fd);

int main(int argc, char *argv[]) {
  int server_fd;
  long port_num = 8000;
  struct sockaddr_in server_addr;
  // create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation failed!");
    exit(1);
  }

  // configuring the server
  server_addr.sin_family =
      AF_INET; // ipv4 address family AF_INET6 for ipv6 address
  server_addr.sin_addr.s_addr = INADDR_ANY; // accept connection on any network
                                            // interface on the local machine
  server_addr.sin_port =
      htons(port_num); // htons converts host byte to network byte

  // binding socket to a specific port and specific address
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Binding failure!! Exiting..");
    exit(1);
  }

  // listen to the specified port
  if (listen(server_fd, 2) < 0) {
    perror("Error while listening on the port!!!");
    exit(1);
  }

  // listen for client
  while (1) {
    // client info
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int *client_fd = static_cast<int *>(
        malloc(sizeof(int))); // allocate memory for client_fd

    if ((*client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                             &client_addr_len)) < 0) {
      perror("Couldn't accept the client connection!");
      // free(client_fd);
      continue;
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_client, (void *)client_fd) !=
        0) {
      perror("Thread couldn't be created!!");
      free(client_fd);
    }
    pthread_detach(thread_id);
  }

  close(server_fd);
  return EXIT_SUCCESS;
}

void *handle_client(void *client_fd_ptr) {
  int client_fd = *((int *)client_fd_ptr);
  // char *myMessage = new char[100]; // this doesn't print all the requests
  // memset(myMessage, 0, 100);
  // obtained from the client idk why
  char myMessage[1024] = {0}; // but this does
  // char *myMessage[1096]; // but this does
  auto bytesRead = recv(client_fd, myMessage, sizeof(myMessage), 0);
  if (bytesRead < 0) {
    perror("\nError reading from client!!\n");
    close(client_fd);
    free(client_fd_ptr);
    return NULL;
  }

  std::cout << myMessage << std::endl;

  std::string response = "HTTP/1.1 200 OK\r\nContent-Type: "
                         "text/html\r\n\r\n<html><body><h1>Hello there babygirl"
                         "🫰</h1></body></html>\r\n";

  send(client_fd, response.c_str(), response.size(), 0);
  // delete[] myMessage;
  close(client_fd);
  return NULL;
}
