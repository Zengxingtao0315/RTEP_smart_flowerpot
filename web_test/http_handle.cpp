#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

void HandleGetRequest(const string& path, const unordered_map<string, string>& params) {
  // 处理GET请求的逻辑
  cout << "Handle GET request: " << path << endl;
  for (const auto& param : params) {
    cout << param.first << " = " << param.second << endl;
  }
}

void HandlePostRequest(const string& path, const unordered_map<string, string>& params, const string& body) {
  // 处理POST请求的逻辑
  cout << "Handle POST request: " << path << endl;
  for (const auto& param : params) {
    cout << param.first << " = " << param.second << endl;
  }
  cout << "Body: " << body << endl;
}

int main() {
  unordered_map<string, function<void(const string&, const unordered_map<string, string>&)>> router = {
    {"/get", HandleGetRequest},
    {"/post", HandlePostRequest},
  };
  // 启动服务器等待请求
  while (true) {
    // 解析HTTP请求并根据请求路由到不同的处理函数
    // ...
  }
  return 0;
}
