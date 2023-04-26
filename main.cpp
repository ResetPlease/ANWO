#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "LHTTPI.h"
#include <ctime>
#include "ANWO/ANWO.h"

int main(int argc, char** argv)
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    int port = 0;
    if(argc == 1){
        port = 3423;
    }
    else{
        port = std::stoi(std::string(argv[1]));
    }
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    listen(listener, 1);
    std::cout << " \033[0;35mStart on " << port << " port\n\n";
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        while(1)
        {
            bytes_read = recv(sock, buf, 2048, 0);
            std::vector<std::string> request_members = split(std::string(buf), "\n");
            std::vector<std::string> request;
            for(int i=0;i<request_members.size(); ++i){
                std::vector<std::string> line = split(request_members[i], " ");
                for(int j=0;j<line.size(); ++j)
                    request.push_back(line[j]);
            }
            std::string url = request[1];
            std::string req_type = "text/html";
            if(url == "/"){
                url = "/templates/main.html";
            }
            std::string rend = "";
            if(url.find("?") != std::string::npos){
                //GET REQUEST FROM JS
                std::string murl = "";
                for(int i=2;i<url.size(); ++i){
                    murl += url[i];
                }
                url = murl;
                req_type = "text/html";
                std::map<std::string, int> param;
                std::cout << "\033[0;36m GET FROM JS: ";
                std::vector<std::string> parametr = split(url, "&");
                for(int i=0;i<parametr.size(); ++i){
                    param[split(parametr[i],"=")[0]] = std::stoi(split(parametr[i],"=")[1]);
                }
                std::cout << std::endl;
                for(auto it : param) {
                    std::cout << it.first << ": " << it.second << std::endl; 
                }
                std::string answ = work(param["day"], param["market"], param["product"], param["percentup"], param["percentdown"]);
                rend = answ;
            }
            else{
                rend = render_template("/home/beregom/Projects/Server" + url );
            }
            std::string mega = "";
            mega =  "HTTP/1.1 200 OK\r\nHost: localhost:3422\r\nContent-Type: ";
            mega +=  req_type +     " charset=utf-8 \r\n" +
                                    "Connection: close\r\n";
            mega += "Content-Length: " + std::to_string(rend.size()) + "\r\n\r\n" + rend;
            time_t now = time(0);
            std::cout << "\033[0;34m Local time: " << std::string(ctime(&now)) << "\033[1;31m --> ";
            std::cout << "GET: " << url <<" " << request[2] << std::endl;
            std::cout << "\n";
            if(bytes_read <= 0) break;
            send(sock, mega.c_str(), mega.size(), 0);
        }
        close(sock);
    }
    
    return 0;
}
// 2.заказчики
// 1.кол-вод дней
// 3.кол-во видов продуктов
// 4.процент наценки
// 89607220085
// 5.процент скидки