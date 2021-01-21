mkdir build

cd build

cmake ..

#To prevent the console from closing
read -n1 -s -r -p $'Press space to close console window...\n' key

if [ "$key" = ' ' ]; then
    #Space pressed, do something
    #Echo [$key] is empty when SPACE is pressed # uncomment to trace
else
    #Anything else pressed, do whatever else.
    #echo [$key] not empty
fi