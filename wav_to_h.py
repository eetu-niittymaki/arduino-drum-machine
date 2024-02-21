#!/usr/bin/env python
#
# A script that formats samples to wavetables for an Arduino drum machine I made.
#
# Shortens .wav files to mono, 2048 - 8192 samples long and 
# uses ffmpeg (https://ffmpeg.org/) to format them to signed 8-bit .raw,
# then creates wavetable header files from them with a samplerate of 16384 or 32768.

import glob
import os, shutil, sys
from array import array
import wave
from tkinter import *
from tkinter import filedialog
from tkinter import ttk
from tkinter import messagebox as msg 

sample_count = None
samplerate = None
channels = None
path = None
ffmpeg_exe = None
run = True

sample_count_options = [
    2048,
    4096,
    8192
]

samplerate_options = [
    16384,
    32768
]

channel_options = [
    1,
    2
]

def write_to_file(file):
    global run
    with open(file, "w") as f:            
        while True:
            ffmpeg_filedialog = filedialog.askdirectory(title="Select Folder With ffmpeg.exe")
            if (os.path.isfile(f"{ffmpeg_filedialog}/ffmpeg.exe")):
                f.write(f"{ffmpeg_filedialog}/ffmpeg.exe")
                break
            elif ffmpeg_filedialog == "": #  Filedialog terminated
                run = False
                break
            else:
                msg.showerror("Error!", "ffmepg.exe not in directory!")

def locate_ffmpeg():
    global ffmpeg_exe
    file = f"{os.path.dirname(sys.argv[0])}/ffmpeg_path.txt"
    if not os.path.exists(file):
        write_to_file(file)
    else:
        with open(file) as f:
            if (os.stat(file).st_size > 0):
                ffmpeg_exe = f.readline().strip("\n")
            else:
                write_to_file(file)

def check_ffmpeg(): #Check if ffmpeg.exe location is in System variables
    global ffmpeg_exe
    try:
        ffmpeg_exe = os.path.realpath(shutil.which("ffmpeg"))
    except TypeError:
        locate_ffmpeg()

def raw_to_h(infile, tablename):
    with open(infile, "rb") as raw_file:
        raw_data = raw_file.read()

    audio_array = array("B", raw_data)
    tablename = tablename.split("\\")[::-1][0].replace(" ", "_")
    output_file_name = os.path.splitext(infile)[0] + ".h"
    
    with open(output_file_name, "w") as output:
        output.write(f"#ifndef {tablename}_H_\n")
        output.write(f"#define {tablename}_H_\n\n")
        output.write("#if ARDUINO >= 100\n")
        output.write("#include \"Arduino.h\"\n")
        output.write("#else\n")
        output.write("#include \"WProgram.h\"\n")
        output.write("#endif\n")
        output.write("#include \"mozzi_pgmspace.h\"\n\n")
        output.write("#define " + tablename + "_NUM_CELLS " + str(len(audio_array)) + "\n")
        output.write("#define " + tablename + "_SAMPLERATE " + str(samplerate) + "\n \n")
        output.write("CONSTTABLE_STORAGE(int8_t) " + tablename + "_DATA [] = {")
        output.write(", ".join(str(frame) for frame in audio_array))
        output.write("};\n\n")
        output.write(f"#endif /* {tablename}_H_ */")
       
def to_raw(file):
    with wave.open(file, "r") as read:
        frames = read.getnframes()
        sampwidth = read.getsampwidth()
        framerate = read.getframerate()

        if frames > sample_count:
            data = read.readframes(sample_count)
        else:
            data = read.readframes(frames)

    with wave.open(file, "w") as write:
        write.setnchannels(channels)
        write.setsampwidth(sampwidth)
        write.setframerate(framerate)
        write.writeframes(data)

    file = file.split(".wav")[0]
    ffmpeg = f'{ffmpeg_exe} -y -i "{file}.wav" -f s8 -acodec pcm_s8 "{file}.raw"' 
    os.system(ffmpeg) # Runs string as a shell command
    raw_to_h(f"{file}.raw", file)

def set_path():
    global path
    path = filedialog.askdirectory(title="Select Folder With .wav Samples")
    if path:
        root.destroy()
    else:
        root.mainloop()

def set_sample_count(count):
    global sample_count
    count = sample_count_set.get()
    sample_count = int(count)

def set_samplerate(rate):
    global samplerate
    rate = samplerate_set.get()  
    samplerate = int(rate)

def set_channels(channel):
    global channels
    channel = channels_set.get()  
    channels = int(channel)

if __name__ == "__main__": 
    while run == True:
        check_ffmpeg()
        if (ffmpeg_exe):
            sample_count = sample_count_options[0]
            samplerate = samplerate_options[0]
            channels = channel_options[0]

            root = Tk()
            root.geometry("300x200")
            root.eval("tk::PlaceWindow . center")
            root.title("Sample Options")
            root.resizable(0, 0)
            root.columnconfigure(0, weight=1)
            root.columnconfigure(1, weight=1)

            path_set = Button(root, text="Select Folder With .wav Samples", command=set_path, height=2)
            path_set.grid(columnspan=2, row=0, padx=5, pady=15)

            sample_count_label = ttk.Label(root, text="Number of samples:")
            sample_count_label.grid(column=0, row=1, sticky="", padx=5, pady=5)
            sample_count_set = StringVar(root)
            sample_count_set.set(sample_count_options[0])
            count_menu = OptionMenu(root, sample_count_set, *sample_count_options, command=set_sample_count)
            count_menu.grid(column=1, row=1, sticky="", padx=5, pady=5)

            samplerate_label = ttk.Label(root, text="Samplerate (Hz):")
            samplerate_label.grid(column=0, row=2, sticky="")
            samplerate_set = StringVar(root)
            samplerate_set.set(samplerate_options[0])
            rate_menu = OptionMenu(root, samplerate_set, *samplerate_options, command=set_samplerate)
            rate_menu.grid(column=1, row=2, sticky="", padx=5, pady=5)

            channels_label = ttk.Label(root, text="Channels (Mono/Stereo):")
            channels_label.grid(column=0, row=3, sticky="", padx=5, pady=5)
            channels_set = StringVar(root)
            channels_set.set(channel_options[0])
            channel_menu = OptionMenu(root, channels_set, *channel_options, command=set_channels)
            channel_menu.grid(column=1, row=3, sticky="", padx=5, pady=5)

            root.mainloop()
            
            if path is not None:
                for file in glob.glob(f"{path}/*.wav"): # Searches folder for all .wav files
                    to_raw(file)
            run = False