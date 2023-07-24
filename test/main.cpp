extern "C" {

	#include <sys/socket.h>
	#include <netdb.h>
	#include <time.h>
}
#include "../lib/Config/DEV_Config.hpp"
#include "../lib/Config/Debug.hpp"
#include "../lib/GUI/GUI_Paint.hpp"
#include "../lib/Function/sensor.hpp"
#include "../lib/Function/GetTime.hpp"
#include "../lib/OLED/OLED.hpp"
#include "../lib/Function/checkInternet.hpp"

#include <ctime> 
#include <cstdlib> 
#include <cmath>
#include <csignal> 
#include <cstring> 
#include <ctime>
#include <iostream>
#include <boost/asio.hpp>
#include <memory>

#include <wiringPi.h>

#include <iostream> // C++标准头文件，其中包含cout和endl的定义。

#include <string>
#include <thread>


#define OLED_WIDTH 128
#define OLED_HEIGHT 128


using namespace boost::asio;
using ip::tcp;
using namespace std;
extern DEV DEV;
Sensor Sensor(DIGITALPIN,  DHTPIN);
class HttpServerSession : public std::enable_shared_from_this<HttpServerSession> {
public:
    HttpServerSession(boost::asio::io_service& io_service)
        : socket_(io_service) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(socket_, request_, "\r\n\r\n",
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                self->handle_read(error, bytes_transferred);
            });
    }

    // 新增一个公有函数，用于发送响应数据
    void sendResponse(const std::string& response) {
        async_write(socket_, boost::asio::buffer(response),
            [self = shared_from_this()](const boost::system::error_code& error, size_t /*bytes_transferred*/) {
                self->handle_write(error);
            });
    }
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
        if (!error) {
            std::istream request_stream(&request_);
            std::string http_request;
            std::getline(request_stream, http_request);
            std::cout << "Received HTTP request: " << http_request << std::endl;

            // 在这里不再处理响应，仅输出请求信息

            // 注意：在这里不再主动关闭socket，留待后续处理

            // 此处需要调用 handle_write 函数，但它是私有函数，无法直接访问
            // 可以通过新增公有函数 sendResponse 来发送响应数据

            // 注意：需要处理异常情况，但在此处暂时省略错误处理
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }
    }

    tcp::socket socket_;
    boost::asio::streambuf request_;
};

class HttpServer {
public:
    HttpServer(boost::asio::io_service& io_service)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8080)),
          io_service_(io_service) {
        start_accept();
    }

private:
    void start_accept() {
        auto new_session = std::make_shared<HttpServerSession>(io_service_);

        acceptor_.async_accept(new_session->socket(),
            [this, new_session](const boost::system::error_code& error) {
                handle_accept(new_session, error);
            });
    }

    void handle_accept(std::shared_ptr<HttpServerSession> session, const boost::system::error_code& error) {
        if (!error) {
            session->start();
        }

        start_accept();
    }

    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

void updateSensorData(HttpServerSession& session) {
    // 获取传感器数据
    double temp = Sensor.getTemperature();
    double hum = Sensor.getHumidity();

    // 设置响应数据
    std::string response = "HTTP/1.1 200 OK\r\nTemperature: " + std::to_string(temp) + "\r\n\r\nHumidity: " + std::to_string(hum);

    // 发送响应
    session.sendResponse(response);
}


//expression plants emotion or status
const char* EmojiSelector(double temperature, double humidity, int digital, float light_duration ){
	//When everything is fine
	if(temperature <= 38.0 && temperature >= 15.0 && digital == 0 && humidity >= 30.0 && humidity <= 50.0)
	{
		return "./pic/happy.bmp";		
	}//Too many hours of sunlight when the temperature is too high
	else if(temperature > 38.0  && digital == 0 && humidity >= 30.0 && humidity <= 50.0 && light_duration > 8.0){
		return "./pic/too_much_light.bmp";
	}//Insufficient sunshine hours when the temperature is too high
	else if(temperature > 38.0  && digital == 0 && humidity >= 30.0 && humidity <= 50.0 && light_duration <= 8.0){
		return "./pic/too_hot.bmp";
	}//When plants are not exposed to light and have insufficient hours of sunlight
	else if(temperature <= 38.0 && temperature >= 15.0  && digital == 1 && humidity >= 30.0 && humidity <= 50.0 && light_duration <= 8.0){
		return "./pic/lack_of_sunlight.bmp";
	}//Any time the temperature is too low
	else if(temperature < 15.0  && humidity >= 30.0 && humidity <= 50.0 ){
		return "./pic/cold.bmp";
	}//Any time the humidity is too low
	else if(humidity < 30.0 ){
		return "./pic/need_water.bmp";
	}//Any time the humidity is too high
	else if(humidity > 50.0 ){
		return "./pic/overwatered.bmp";
	}

	
	
}

void serverThreadFunc() {
    boost::asio::io_service io_service;
    HttpServer server(io_service);

    io_service.run();
}

void Handler(int signo)
{
	// System Exit
	std::cout << "Handler: exit" << std::endl;
	DEV.ModuleExit();

	std::exit(0);
}

int main()
{
	std::signal(SIGINT, Handler);
    
	std::cout << "OLED showing" << std::endl;
    if (DEV.ModuleInit() != 0) {
        return -1;
    }
    if (USE_IIC) {
        std::cout << "Only USE_SPI, Please revise DEV_Config.h !!!" << std::endl;
        return -1;
    }
	//OLED Init...
    std::cout << "OLED Init..." << std::endl;
	
	wiringPiSetup();
	Paint Paint;
	OLED OLED;
	OLED.Init();
	DEV.Delay_ms(500);	
	
    // Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = (OLED_WIDTH * 2) * OLED_HEIGHT;
    if ((BlackImage = new UBYTE[Imagesize + 300]) == NULL) {
        std::cout << "Failed to apply for black memory..." << std::endl;
        return -1;
    }
    std::cout << "Paint_NewImage" << std::endl;
	
    Paint.NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, BLACK);
    std::cout << "Drawing" << std::endl;
    
	//Select Image
    Paint.SelectImage(BlackImage);

	DEV.Delay_ms(500);

    Paint.Clear(BLACK);

    // initialise the whole display
    //GUI_ReadBmp("./pic/OLED.bmp", 0, 0);
    // Show image on page
    //OLED_Display(BlackImage);
    //DEV_Delay_ms(500);
    //Paint_Clear(BLACK);

	// init time funciton, get local time.
	Time time;
	PAINT_TIME local_time;
	//init InternetConnectionChecker
	InternetConnectionChecker checker;
	bool connected ; //Determining network connection status
	//init Sensors pin
	
	UWORD  digitalValue;
	//UWORD  analogValue;
	float light_duration;
	SunlightDurationRecorder duration;
	double temp ;
	double hum ;
	
	std::thread serverThread(serverThreadFunc);
	
	std::thread sensorThread([&]() {
        while (true) {
            // 更新传感器数据
            double temp = Sensor.getTemperature();
            double hum = Sensor.getHumidity();

            // 设置响应数据
            std::string response = "HTTP/1.1 200 OK\r\nTemperature: " + std::to_string(temp) + "\r\n\r\nHumidity: " + std::to_string(hum);

            // 发送响应
            server.io_service().post([&, response]() {
                HttpServerSession temp_session(server.io_service());
                temp_session.sendResponse(response);
            });

            // 休眠1秒
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
	
    while (1) {
		std::cout<<"painting the first page!"<<std::endl;
		//display of internet status
		connected = checker.CheckInternetConnection();
		connected ? Paint.GUI_ReadBmp("./pic/internet_up.bmp", 100, 0) : Paint.GUI_ReadBmp("./pic/internet_down.bmp", 100, 0);
		local_time = time.getLocalTime();
		Paint.DrawTime(5, 0, &local_time, &Font12, BLACK, TIME_COLOR);
		//Humidity and Temperature
		
		temp = Sensor.getTemperature();
        hum = Sensor.getHumidity();
		updateSensorData(session);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Paint.DrawString(5, 13, "Humi(%)", &Font12, BLACK, WHITE);
		Paint.DrawNum(60, 13,hum, &Font12, 1,  WHITE, BLACK);
		Paint.DrawString(5, 25, "Temp(C)", &Font12, BLACK, WHITE);
		Paint.DrawNum(60, 25, temp, &Font12, 1,  WHITE, BLACK);
		
		//Digital reading of the light emitx`ing diode, 1 for almost no light, 0 for light		
		digitalValue = Sensor.readDigitalValue();
		//Calculate the duration of the reading at 0, which is also the duration of daylight
		light_duration = duration.getSunlightDurationInHours(digitalValue);
		if (digitalValue == 0){

			Paint.DrawString(5, 37, "light", &Font12, BLACK, WHITE);
			std::cout<<"light"<<std::endl;

		}else{
			std::cout<<"dark"<<std::endl;
			Paint.DrawString(5, 37, "Dark", &Font12, BLACK, WHITE);

		}
		OLED.Display(BlackImage);
		Paint.Clear(BLACK);	
		/**********************************
		if(light_duration < 8.0){
			Paint.DrawString(10, 40, "need more light", &Font12, BLACK, WHITE);
		}
		else{
			Paint.DrawString(10, 40, "light sufficient", &Font12, BLACK, WHITE);
		}
		**********************************/
		
		//display of the plant emoji
		std::cout<<"painting the emoji page!"<<std::endl;
		Paint.GUI_ReadBmp(EmojiSelector(temp, hum,digitalValue, light_duration), 32, 64);
		
		OLED.Display(BlackImage);
		DEV.Delay_ms(10000);
		Paint.Clear(BLACK);	

		std::this_thread::sleep_for(std::chrono::seconds(10));
		OLED.Clear();

	}
	serverThread.join();
    sensorThread.join();

    return 0;
}
