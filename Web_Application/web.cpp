#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

// Simulated sensor data for demonstration purposes
double getTemperature() {
    // Replace this with actual code to read temperature from the sensor
    return 25.5;
}

double getHumidity() {
    // Replace this with actual code to read humidity from the sensor
    return 50.0;
}

class SensorApp : public cppcms::application {
public:
    SensorApp(cppcms::service &srv) : cppcms::application(srv) {}

    void index() {
        // Set the content type to JSON
		std::ostringstream json;
        response().content_type("application/json");

        while (true) {
            // Get temperature and humidity data
            double temperature = getTemperature();
            double humidity = getHumidity();

            // Construct JSON response
            std::ostringstream json;
            json << "{ \"temperature\": " << temperature << ", \"humidity\": " << humidity << " }";

            // Send JSON response
            response().out() << json.str() << std::flush;

            // Sleep for a short time before updating data again
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main(int argc, char **argv) {
    try {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<SensorApp>(),"/sensor");
        srv.run();
    }
    catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
