# Improving debugging by bat files{#improvedebug}

If you do not want to use your terminal enormous amount of times while you are debugging or testing new features it can help you

You can use .bat files to run server or server & client on your own PC already with arguments and by 2 clicks of mouse

#### 1. Creation of server.start .bat file
1. Create an empty .txt file where you wish
2. Copy the next piece of text and paste it into already created .txt file

	echo "server.start"
	start "" "<your_path>" -p 65001
	exit

3. Find place where your Server.Exec.exe exists and copy that path
4. Replace <your_path> on yours
5. File -> Save as 
6. In the field of the file type change .txt type on any type
7. Add to the end of file name ".bat"
8. Save

#### 2. Creation of server & client .bat file
