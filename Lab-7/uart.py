from tkinter import *
import tkinter
import time
import matplotlib.pyplot as plt
from PIL import ImageTk, Image
import serial
from time import sleep
#from stream_engine.stream import Stream, StreamAnimation


ser = serial.Serial ("COM6", 9600)    #Open port with baud rate


def blue():
    ser.write(bytes(b'b\r'))
def white():
    ser.write(bytes(b'w\r'))
def red():
    ser.write(bytes(b'r\r'))
def green():
    ser.write(bytes(b'g\r'))



def begWrite():
    ser.write(bytes(b'v\r'))

y = []

def plot():
    window.destroy()
    begWrite()

    #global x,y
    plt.ion()
    fig,ax = plt.subplots()
    ax.set_ylim(0,4)
  

    while True:
        line = ser.readline().decode("utf-8")
        num = 0.0
        try:
            num = float(line)
        except ValueError:
            num = 0.0

        ax.set_xlim(int(len(y)/20)*20,int(len(y)/20)*20+20)
        y.append(num)
        ax.plot(y,c='b')
        fig.canvas.draw()
        fig.canvas.flush_events()

        print(num)

window = Tk() 

# setting the title 
window.title('UART COMMUNICATIONS')   
window.configure(bg='black')

whitebutton = Button(window,text = 'WHITE',command = white,bg=  'white')  
whitebutton.place(x=100,y=75)  

greenbutton = Button(window,text = 'GREEN',command = green,bg=  'lightgreen')  
greenbutton.place(x=200,y=75) 

redbutton = Button(window,text = 'RED',command = red,bg=  'red')  
redbutton.place(x=300,y=75) 

bluebutton = Button(window,text = 'BLUE',command = blue,bg=  'lightblue')  
bluebutton.place(x=400,y=75) 

  
# dimensions of the main window 
window.geometry("600x150") 

# # button that displays the plot 
plot_button = Button(master = window, command = plot, text = "GRAPH") 
plot_button.place(x=500,y=75) 

# run the gui 
window.mainloop()