#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include <thread>

void start_server(double temperature, double humidity, float lightDuration) {
  // Create the IO context and endpoint
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::tcp::v4(), 8080};

  // Create and bind the acceptor
  boost::asio::ip::tcp::acceptor acceptor{io_context, endpoint};
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.listen();

  // Accept WebSocket connections
  boost::asio::spawn(io_context, [&](boost::asio::yield_context yield) {
    while (true) {
      boost::asio::ip::tcp::socket socket{io_context};
      acceptor.async_accept(socket, yield);

      boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws{
          std::move(socket)};
      ws.async_accept(yield);

      // Send real-time data to the WebSocket connection
      while (true) {
        // 假设获取到的实时数据分别保存在 temperature、humidity 和 lightDuration 变量中
		std::string data = std::to_string(temperature) + "," + std::to_string(humidity) + "," + std::to_string(lightDuration);

        boost::beast::websocket::frame frame{boost::beast::websocket::opcode::text, boost::beast::websocket::frame::create(data)};
        ws.async_write(frame, yield);
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }
  });

  // Run the IO context
  io_context.run();
}


