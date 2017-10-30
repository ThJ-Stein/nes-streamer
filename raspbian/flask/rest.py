#!bin/python
from flask import Flask, json, request
import subprocess
import signal

app = Flask(__name__, static_url_path='/home/nes-streamer/raspbian/flask')

youtube_url = "rtmp://a.rtmp.youtube.com/live2"
# THOMAS DEFIANTLY DOESTNS PRIRATE source = "\"/home/thomas/Shared Disk/Shared Disk/Downloads/_Torrents/Rick and Morty Season 1 [1080p] [HEVC]/[pseudo] Rick and Morty S01E01 - Pilot [1080p] [h.265].mkv\""
source = "../streamFile"

process = None

@app.route('/')
def index():
        return app.send_static_file('home.html')

@app.route('/start', methods=['POST'])
def api_start_stream():
        global settings;
        print('starting stream')
        settings = request.get_json()
        print('got json')
        start_stream()
        return "yay"

@app.route('/stop', methods=['POST'])
def api_stop_stream():
        print('stopping stream')
        stop_stream()
        return "aww"

#the methods to start the stream
def start_stream():
        global process
        global settings
        command = "exec ffmpeg -i %s -deinterlace -vcodec libx264 -pix_fmt yuv420p" \
        " -preset %s -r %s -g $((%s * 2)) -b:v %s -acodec libmp3lame -ar" \
        " 44100 -threads 6 -qscale 3 -b:a 712000 -bufsize 512k -f flv" \
        " \"%s/%s\"" % (source,settings["quality"], settings["fps"], settings["fps"], settings["bitrate"], youtube_url, settings["key"])
        process = subprocess.Popen(command, shell=True)

def stop_stream():
        global process
        print(process.pid)
        process.kill()


if __name__ == '__main__':
        app.run(debug=True,  host='0.0.0.0')