#!/usr/bin/env python
#
# A script that formats samples to wavetables for an Arduino drum machine I made.
#
# Shortens .wav files to mono, 2048 samples long and 
# uses ffmpeg (https://ffmpeg.org/) to format them to signed 8-bit .raw,
# then creates wavetable header files from them with a samplerate of 16384.

import glob
import os, shutil
from array import array
import wave
from tkinter import *
from tkinter import filedialog
from pydub import AudioSegment
from tkinter import ttk

#sample_count = 2048
#samplerate = 32768

sample_count = None
samplerate = None
path = None

sample_count_options = [
    2048,
    4096,
    8192
]

samplerate_options = [
    16384,
    32768
]

try:    
    ffmpeg_exe = os.path.realpath(shutil.which("ffmpeg"))
except Exception:
    print("Ffmpeg not found, add it to your PATH variables")

def raw_to_h(infile, tablename):
    with open(infile, 'rb') as raw_file:
        raw_data = raw_file.read()

    audio_array = array('B', raw_data)
    tablename = tablename.split("\\")[::-1][0].replace(" ", "_")

    output_file_name = os.path.splitext(infile)[0] + ".h"
    with open(output_file_name, 'w') as output:
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
       
def format(file):
    with wave.open(file, "r") as read:
        frames = read.getnframes()
        sampwidth = read.getsampwidth()
        framerate = read.getframerate()
        channels = read.getnchannels()

        if frames > int(sample_count):   # if wav files sample count is > 2048: read and write the first 2048 samples, else write all of them
            data = read.readframes(int(sample_count))
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
    path = filedialog.askdirectory()
    root.destroy()

def set_sample_count(count):
    global sample_count
    count = sample_count_set.get()
    sample_count = count

def set_samplerate(rate):
    global samplerate
    rate = samplerate_set.get()  
    samplerate = rate

if __name__ == "__main__": 
    sample_count = sample_count_options[0]
    samplerate = samplerate_options[0]

    root = Tk()
    root.geometry("300x150")
    root.eval('tk::PlaceWindow . center')
    root.title("Sample Options")
    root.resizable(0, 0)
    root.columnconfigure(0, weight=5)
    root.columnconfigure(1, weight=1)

    path_set = Button(root, text="Select Folder With .wav Samples", command=set_path, width=30)
    path_set.grid(column=0, row=0, sticky=E, padx=5, pady=5)

    sample_count_label = ttk.Label(root, text="Sample Count:")
    sample_count_label.grid(column=0, row=1, sticky=W, padx=5, pady=5)
    sample_count_set = StringVar(root)
    sample_count_set.set(sample_count_options[0])
    count_menu = OptionMenu(root, sample_count_set, *sample_count_options, command=set_sample_count)
    count_menu.grid(column=1, row=1, sticky=E, padx=5, pady=5)

    samplerate_label = ttk.Label(root, text="Samplerate:")
    samplerate_label.grid(column=0, row=2, sticky=W, padx=5, pady=5)
    samplerate_set = StringVar(root)
    samplerate_set.set(samplerate_options[0])
    rate_menu = OptionMenu(root, samplerate_set, *samplerate_options, command=set_samplerate)
    rate_menu.grid(column=1, row=2, sticky=E, padx=5, pady=5)

    root.mainloop()
    
    if path is not None:
        for file in glob.glob(f"{path}/*.wav"): # Searches folder for all .wav files
            sound = AudioSegment.from_wav(file)
            sound = sound.set_channels(1) # Downsamples file to mono
            sound.export(file, format="wav")
            format(file)
