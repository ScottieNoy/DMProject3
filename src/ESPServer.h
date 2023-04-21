#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <WebServer.h>

class ESPServer {
public:
  ESPServer(int port);
  void begin();
  void handleClient();
  void updateNum(int amount);
  void updateMax(int amount);
  void addVisitor(String name);
  void removeVisitor(String name);
  
  
  
private:
  WebServer *_server;
  int _currentVisitors;
  int _maxVisitors;
  String _visitors[1000];
  int _numVisitors;
  
  void generateHTML();
  String getMonthName(int month);
  String getTimeString(int hour, int minute, int second);
  
};

#endif // ESPSERVER_H
