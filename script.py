#!/usr/bin/env python
#
# A script that formats samples to wavetables for an Arduino drum machine I made.
#
# Shortens .wav files to mono, 2048 samples long and 
# uses ffmpeg (https://ffmpeg.org/) to format them to signed 8-bit .raw,
# then creates wavetable header files from them with a samplerate of 16384.

import glob
import os, shutil, subprocess
from array import array
import wave
from tkinter import filedialog
from tkinter import *
from pydub import AudioSegment

sample_count = 2048 * 2
samplerate = 16384

try:    
    ffmpeg_exe = os.path.realpath(shutil.which("ffmpeg"))
except Exception:
    print("Ffmpeg not found, add it to your PATH variables")

def raw_to_h(infile, tablename):
    with open(infile, 'rb') as raw_file:
        raw_data = raw_file.read()

    audio_array = array('B', raw_data)
    tablename = tablename.split("\\")[::-1][0]

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

        if frames > sample_count:   # if wav files sample count is > 2048: read and write the first 2048 samples, else write all of them
            data = read.readframes(sample_count)
        else:
            data = read.readframes(frames)
    
    with wave.open(file, "w") as write:
        write.setnchannels(channels)
        write.setsampwidth(sampwidth)
        write.setframerate(framerate)
        write.writeframes(data)

    file = file.split(".")[0]
    ##ffmpeg = ffmpeg_exe + " -y -i " + file + ".wav" +  " -f s8 -acodec pcm_s8 "  + file + ".raw" 
    ffmpeg = f'{ffmpeg_exe} -y -i "{file}.wav" -f s8 -acodec pcm_s8 "{file}.raw"'
    os.system(ffmpeg)
    raw_to_h(f"{file}.raw", file)

if __name__ == "__main__": 
    root = Tk()
    root.withdraw()

    path = filedialog.askdirectory(title="Select Folder With .wav Samples")
    for file in glob.glob(f"{path}/*.wav"): # Searches folder for all .wav files
        sound = AudioSegment.from_wav(file)
        sound = sound.set_channels(1) # Downsamples file to mono
        sound.export(file, format="wav")
        format(file) 
