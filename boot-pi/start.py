import os
import signal
import subprocess
import serial

print("hello")

base_note = 65
videos = [
    "/usbflash/0_Tine_Surel_Lange_Arctic_Creatures_Repparfjord_2019_Video_and_stereo_sound.mp4",
    "/usbflash/1_Tine_Surel_Lange_Desert_Creatures_The_Salton_Sea_2020_Video_and_stereo_sound.m4v"
]

ser = serial.Serial("/dev/ttyAMA0", 9600)
current_video = None

while True:
    ch = ord(ser.read())
    note = ch & 0x7F

    if ch & (1 << 7) > 0:
        print("note on", note)
    else:
        print("note off", note)
    
    video_idx = note - base_note
    
    if video_idx < 0 or video_idx > len(videos) - 1:
        continue
    
    if ch & (1 << 7) > 0:
        if current_video is not None and current_video["id"] != video_idx:
            print("kill prev omx")
            # new video, kill omxplayer
            os.killpg(os.getpgid(current_video["omx"].pid), signal.SIGTERM)
            current_video = None
        # run video
        print("run",  videos[video_idx])
        current_video = {
            "id": video_idx,
            "omx": subprocess.Popen(["omxplayer", "-o", "local", videos[video_idx]], stdout=subprocess.PIPE, preexec_fn=os.setsid)
        }
    else:
        # just stop video
        if current_video is not None:
            print("kill omx")
            os.killpg(os.getpgid(current_video["omx"].pid), signal.SIGTERM)
            current_video = None
