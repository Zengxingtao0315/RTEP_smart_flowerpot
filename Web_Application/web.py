html='''

<!DOCTYPE html>

<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=0.5">


    <title>Intelligent Flower Pot</title>
    <style>
        #id01 {background-color: rgb(85, 85, 85);
        }

        .main_1 {
            border-radius: 15px;
            margin: 7px;
        }

        .font_1 {
            color: rgb(255,255,255);
            text-align: center;
            margin: 7px;
        }
    </style>
</head>

<body style="background-color: rgb(29, 29, 29);background-size: 100% ">
    <!-- main body -->
	<div style="height:800px;width: 648px;margin: auto;">
		<!-- top -->
        <div style="background-color: green; height: 50px;  text-align: center; border-radius: 15px;margin:0 auto; margin:  7px 7px 14px 7px;">
            <h2 style="color: white; line-height: 50px;">Real-time Reflections of Your Lovely Plant</h2>
        </div>
		<!-- middle -->
		<!-- middle-top -->
		<div class='main_1 ' style="height: fit-content; padding: 10px;background-color: rgb(59, 59, 59); text-align:center;">

			<h1 class='font_1' style='font-size: 50px;line-height:40px; '>
				{{time}}
			</h1>
			<button onclick="location.reload()" style = "background-color: rgb(59, 59, 59); cursor: pointer;  color: white;border: none; font-size: 20px; margin-top:30px;">Refresh</button>
		</div>
		<!-- middle-bottom -->
		<div class='main_1 block Temperature' style="width: 170px;height: 300px;float:right;padding: 15px;background-color: rgb(100, 200, 100);">
			<h2 class='font_1' style='font-size: 30px;line-height:100px; '>
				Temperature
			</h2>
			<h2 class='font_1' style='font-size: 30px;line-height:30px; '>
				{{temperature}}<br>°C
			</h2>
		</div>

		<div class='main_1 block Humidity' style="width: 170px;height: 300px;float:right;padding: 15px;background-color: rgb(0, 0, 255);">
			<h2 class='font_1' style='font-size: 30px;line-height:100px;'>
				Humidity
			</h2>
			<h2 class='font_1' style='font-size: 30px;line-height:30px;margin: 7px; '>
				{{humidity}}<br>%
			</h2>
		</div>

		<div class='main_1 block light' style="width: 170px;height: 300px;float:right;padding: 15px;background-color: rgb(255, 165, 0);">
			<a class='font_1' style='font-size: 30px;line-height:30px; '>
				{{lightFlag}}
			</a>
		</div>
		<!-- bottom -->
		<div style='height: 50;margin: 7px;margin: auto;'>
			<p style='color:rgb(151, 255, 255);text-align: center;'>Copyright©G27_Real Time Embedded Programming(2022-23)
				<br>This website is only for academic demo purpose.<br></p>
		</div>
	</div>

</body>

'''

