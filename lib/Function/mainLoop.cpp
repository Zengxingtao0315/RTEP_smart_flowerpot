
#include "mainLoop.hpp"



using namespace std;


   
mainLoop::mainLoop(): loopState(SUCCESS),sensor(DIGITALPIN, DHTPIN),stopRequested(false){
    
    Checkthread = std::thread(&mainLoop::StateChecker, this);
}
        
mainLoop::~mainLoop(){
    if (loopState.load() == TIMEOUT || loopState.load() == INITERROR||loopState.load() == UNKNOWN_ERROR) {
    stopLoop();
    }
}

void mainLoop::loop()
{
   
    if (dev.ModuleInit() != 0)
    {
        std::cerr << "Failed to initialize components." << std::endl;
        loopState = INITERROR;
        
    }
    
    //OLED Init...
    std::cout << "OLED Init..." << std::endl;
    
    wiringPiSetup();
    Paint Paint;
    OLED OLED(&dev);
    if(!OLED.checkInit())
    {
        std::cout << "ERROR ON OLED Init" << std::endl;
        loopState = INITERROR;
        
    }
    dev.Delay_ms(500);	
    
    // Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = (OLED_WIDTH * 2) * OLED_HEIGHT;
    if ((BlackImage = new UBYTE[Imagesize + 300]) == NULL)
    {
        std::cout << "Failed to apply for black memory..." << std::endl;
        loopState = INITERROR;
    }
    std::cout << "Paint_NewImage" << std::endl;
    
    Paint.NewImage(BlackImage, OLED_WIDTH, OLED_HEIGHT, BLACK);
    std::cout << "Drawing" << std::endl;
    
    //Select Image
    Paint.SelectImage(BlackImage);

    dev.Delay_ms(500);

    Paint.Clear(BLACK);


    // init time funciton, get local time.
    Time time;
    PAINT_TIME local_time;
    
    
    //init Sensors pin
    
    UWORD  digitalValue;
    //UWORD  analogValue;
    struct range temperaRange = {18.5, 40.0};
    struct range humidityRange = {0.2, 0.5};
    Plant plant_1(temperaRange,humidityRange);
    //init Selector
    Selector selector(&sensor, temperaRange, humidityRange);

    
    while (!stopRequested) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        
        std::cout<<"painting the first page!"<<std::endl;
        //display of internet status
        Paint.GUI_ReadBmp(selector.BmpSelector(), 100, 0);

        Paint.DrawTime(5, 0, &local_time, &Font12, BLACK, TIME_COLOR);


        //Humidity and Temperature

        plant_1.set_temperature(sensor.getTemperature());
        plant_1.set_humidity(sensor.getHumidity());

        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        Paint.DrawString(5, 13, "Humi(%)", &Font12, BLACK, WHITE);
    

        Paint.DrawNum(60, 13,plant_1.get_humidity(), &Font12, 1,  WHITE, BLACK);
       
        
        Paint.DrawString(5, 25, "Temp(C)", &Font12, BLACK, WHITE);
    

        Paint.DrawNum(60, 25, plant_1.get_temperature(), &Font12, 1,  WHITE, BLACK);
        
		
        Paint.DrawString(5, 37, selector.lightSelector(), &Font12, BLACK, WHITE);
        

        //display of the plant emoji
        std::cout<<"painting the emoji page!"<<std::endl;
        
        Paint.GUI_ReadBmp(selector.EmojiSelector(sensor.getTemperature(),sensor.getHumidity()), 32, 64);
        
        OLED.Display(BlackImage);
        dev.Delay_ms(10000);
        
        Paint.Clear(BLACK);	
        dev.Delay_ms(10000);
        
        OLED.Clear();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime; // 计算运行时间

        if (duration.count() >= 10.0) 
        {
            loopState = TIMEOUT; // 如果运行时间超过2秒，返回超时状态
    
        }
        else
        {
            loopState = SUCCESS;
        }

    }

}


void mainLoop::StateChecker()
{
    if(loopState.load() == TIMEOUT || loopState.load() == INITERROR||loopState.load() == UNKNOWN_ERROR)
    {
        stopLoop();
        std::cout << "Function B is not running. Starting a new B..." << std::endl;
        Loopthread = std::thread(&mainLoop::loop, this); // 启动一个新的B函数
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
}


void mainLoop::stopLoop()
{   
    stopRequested = true;
    
}




void mainLoop::setLoopState(STATE Lstate){
    loopState = Lstate;
}




STATE mainLoop::getLoopState(){
    return loopState;
}


            
    




