import os

vbr = "2500k"
fps = "30"
qual = "medium"
youtube_url = "rtmp://a.rtmp.youtube.com/live2"

source = "\"/home/thomas/Shared Disk/Shared Disk/Downloads/_Torrents/Rick and Morty Season 1 [1080p] [HEVC]/[pseudo] Rick and Morty S01E01 - Pilot [1080p] [h.265].mkv\""
key = "4u3d-d39c-cp1c-bruj" 

command = "ffmpeg -i %s -deinterlace -vcodec libx264 -pix_fmt yuv420p -preset %s -r %s -g $((%s * 2)) -b:v %s -acodec libmp3lame -ar 44100 -threads 6 -qscale 3 -b:a 712000 -bufsize 512k -f flv \"%s/%s\"" % (source, qual, fps, fps, vbr, youtube_url, key)

print(command)
os.system(command)
