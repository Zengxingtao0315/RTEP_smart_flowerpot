html='''

<!DOCTYPE html>

<html lang="zh-cn">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=0.5">


    <title>Smart Flowerpot Project</title>
    <style>
        #id01 {background-color: rgb(85, 85, 85);
        }

        .main_1 {
            border-radius: 15px;
            /*圆角*/
            margin: 7px;
        }

        .font_1 {
            color: rgb(255, 255, 255);
            text-align: center;
            margin: 7px;
        }
    </style>
</head>

<body style="background-color: rgb(29, 29, 29);background-size: 100% ">
    <div id='id01 ' style="height: 100px;margin: auto">
        <h1 style="text-align:center;line-height:100px;color: rgb(255, 255, 255);font-size: 35px;">Smart Flowerpot</h1>

        <div style="height:560px;width: 648px;margin: auto;">

            <div class='main_1 ' style="width: 574px;height: 150px;padding: 30px;background-color: rgb(59, 59, 59);">
                <h2 class='font_1' style='font-size: 30px;line-height:30px; '>
                    DHT sensor temperature measurement time:

                </h2>
                <h2 class='font_1' style='font-size: 40px;line-height:40px; '>
                    {{time}}
                </h2>

            </div>

            <div class='main_1 ' style="width: 280px;height: 300px;float:left; padding: 15px;background-color: rgb(62, 109, 95);">
                <h2 class='font_1' style='font-size: 40px;line-height:100px; '>
                    Temperature
                </h2>
                <h2 class='font_1' style='font-size: 80px;line-height:150px; '>
                    {{temperature}}°C
                </h2>

            </div>

            <div class='main_1 ' style="width: 280px;height: 300px;float:right;padding: 15px;background-color: rgb(124, 85, 59);">
                <h2 class='font_1' style='font-size: 40px;line-height:100px;'>
                    Humidity
                </h2>
                <h2 class='font_1' style='font-size: 80px;line-height:150px;margin: 7px; '>
                    {{humidity}}%
                </h2>
            </div>

        </div>



    </div>

</body>

</html>
'''

