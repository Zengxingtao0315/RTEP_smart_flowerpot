
#include "mainLoop.hpp"



using namespace std;


   
mainLoop::mainLoop(): loopState(SUCCESS){}
        
mainLoop::~mainLoop(){
    if (loopState.load() == TIMEOUT || loopState.load() == INITERROR||loopState.load() == UNKNOWN_ERROR) {
    stopLoop();
}

void mainLoop::loop()
{
    std::signal(SIGINT, Handler);
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
    if(!OlED.checkInit())
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
    //init Selector
    Selector selector;
    //init Sensors pin
    
    UWORD  digitalValue;
    //UWORD  analogValue;
    struct range temperaRange = {18.5, 40.0};
    struct range humidityRange = {0.2, 0.5};
    struct range lightratioRange = {0.2, 0.7};
    Plant plant_1(temperaRange,humidityRange,lightratioRange);


    
    while (1) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        
        std::cout<<"painting the first page!"<<std::endl;
        //display of internet status
        auto GRB_func1 = std::bind(&Paint::GUI_ReadBmp, &Paint, selector.BmpSelector(), 100, 0);
        thread GRB_Thread1(GRB_func1);
        GRB_Thread1.join();

        auto DT_func = std::bind(&Paint::DrawTime, &Paint, 5, 0, &local_time, &Font12, BLACK, TIME_COLOR);
        thread DT_Thread(DT_func);
        DT_Thread.join();

        //Humidity and Temperature

        plant_1.set_temperature(sensor.getTemperature());
        plant_1.set_humidity(sensor.getHumidity());

        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        auto DS_func1 = std::bind(&Paint::DrawString, &Paint, 5, 13, "Humi(%)", &Font12, BLACK, WHITE);
        thread DS_Thread1(DS_func1);
        DS_Thread1.join();


        auto DN_func = std::bind(&Paint::DrawNum, &Paint, 60, 13,plant_1.get_humidity(), &Font12, 1,  WHITE, BLACK);
        thread DN_Thread(DN_func);
        DN_Thread.join();

        
        auto DS_func2 = std::bind(&Paint::DrawString, &Paint, 5, 25, "Temp(C)", &Font12, BLACK, WHITE);
        thread DS_Thread2(DS_func2);
        DS_Thread2.join();


        auto DN_func2 = std::bind(&Paint::DrawNum, &Paint, 60, 25, plant_1.get_temperature(), &Font12, 1,  WHITE, BLACK);
        thread DN_Thread2(DN_func2);
        DN_Thread2.join();
		
        auto DS_func3 = std::bind(&Paint::DrawString, &Paint, 5, 37, selector.lightSelector(), &Font12, BLACK, WHITE);
        thread DS_Thread3(DS_func3);
        DS_Thread3.join();
        

        //display of the plant emoji
        std::cout<<"painting the emoji page!"<<std::endl;
        
        auto GRB_func2 = std::bind(&Paint::GUI_ReadBmp, &Paint, selector.EmojiSelector(sensor.getTemperature(),sensor.getHumidity()), 32, 64);
        thread GRB_Thread2(GRB_func2);
        GRB_Thread2.join();

        
        OLED.Display(BlackImage);
        dev.Delay_ms(1000);
        
        Paint.Clear(BLACK);	
        dev.Delay_ms(1000);
        
        OLED.Clear();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start; // 计算运行时间

        if (duration.count() >= 2.0) 
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
    if(loopState.load() == TIMEOUT || loopState.load()! == INITERROR||loopState.load()! == UNKNOWN_ERROR)
    {
        stopLoop();
        std::cout << "Function B is not running. Starting a new B..." << std::endl;
        Loopthread = std::thread(&mainLoop::loop, this); // 启动一个新的B函数
    }
    
}


void mainLoop::stopLoop()
{   
    loopState = UNKNOWN_ERROR;
    Loopthread.join();
}




void mainLoop::setLoopState(STATE Lstate){
    loopState = Lstate;
}




STATE mainLoop::getLoopState(){
    return loopState;
}


            
    




