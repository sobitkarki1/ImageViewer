# Little bit explaination
I wanted to create a image viewer without using any fancy libraries. Just to know how it all works under the hood. Since it is not possible with console app, gotta use windows header file for gui. Fun little program to play with. Nothing extravagant about it.
Does nothing except opening BMP file named image.bmp in the current folder. 

compile:
```
g++ -municode image_viewer.cpp -mwindows -lgdi32 -lgdiplus -o image_viewer.exe
```

run:
```
image_viewer.exe
```
or
```
./image_viewer.exe
```

have fun, future me ✌️
