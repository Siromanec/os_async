// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For gethostbyname */
#include <netdb.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

std::vector<std::string> get_search_words(const std::string& filename, size_t count){
  std::ifstream inputFile(filename);

  // Check if the file is opened successfully
  if (!inputFile.is_open()) {
    throw std::runtime_error("Error opening the file.");
  }

  // Read n lines from the file
  std::vector<std::string> lines;
  std::string line;

  for (size_t i = 0; i < count && std::getline(inputFile, line); ++i) {
    lines.push_back(line);
  }

  // Close the file
  inputFile.close();

  if (inputFile.is_open()) {
    throw std::runtime_error("Error closing the file.");
  }
  // Display the lines in the vector
  for (const auto& l : lines) {
    std::cout << l << std::endl;
  }
  return lines;
}
int query_example() {
  const char query[] =
      "GET / HTTP/1.0\r\n"
      "HOST: www.google.com\r\n"
      "\r\n";
  const char hostname[] = "www.google.com";
  struct sockaddr_in sin;
  struct hostent *h;
  const char *cp;
  int fd;
  ssize_t n_written, remaining;
  char buf[1024];

  /* Look up the IP address for the hostname.   Watch out; this isn't
     threadsafe on most platforms. */
  h = gethostbyname(hostname);
  if (!h) {
    fprintf(stderr, "Couldn't lookup %s: %s", hostname, hstrerror(h_errno));
    return 1;
  }
  if (h->h_addrtype != AF_INET) {
    fprintf(stderr, "No ipv6 support, sorry.");
    return 1;
  }

  /* Allocate a new socket */
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  /* Connect to the remote host. */
  sin.sin_family = AF_INET;
  sin.sin_port = htons(80);
  sin.sin_addr = *(struct in_addr*)h->h_addr;
  if (connect(fd, (struct sockaddr*) &sin, sizeof(sin))) {
    perror("connect");
    close(fd);
    return 1;
  }

  /* Write the query. */
  /* XXX Can send succeed partially? */
  cp = query;
  remaining = strlen(query);
  while (remaining) {
    n_written = send(fd, cp, remaining, 0);
    if (n_written <= 0) {
      perror("send");
      return 1;
    }
    remaining -= n_written;
    cp += n_written;
  }

  /* Get an answer back. */
  while (1) {
    ssize_t result = recv(fd, buf, sizeof(buf), 0);
    if (result == 0) {
      break;
    } else if (result < 0) {
      perror("recv");
      close(fd);
      return 1;
    }
    fwrite(buf, 1, result, stdout);
  }

  close(fd);
  return 0;
}
int main(int c, char **v)
{
//  auto words = get_search_words("../words.txt", 20);
//  const std::string key {getenv("GOOGLE_API_KEY")};
//  auto keyptr = getenv("GOOGLE_API_KEY");
//  if (keyptr != nullptr)
//    std::cout << keyptr << std::endl;
//  else {
//    std::cerr << "GOOGLE_API_KEY is not set" << std::endl;
//    return -1;
//  }
  query_example();
  return 0;
}