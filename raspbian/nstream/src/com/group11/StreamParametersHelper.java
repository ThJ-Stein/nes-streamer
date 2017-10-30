package com.group11;

import java.util.ArrayList;

public class StreamParametersHelper {

    public static String PIPE_NAME = "stream_pipe";

    public static ArrayList<String> openStreamingPipeToAddress(String streamAddress) {
        ArrayList<String> commands = new ArrayList<>();

        commands.add("bash");
        commands.add("-c");
        commands.add(String.format("%s ; %s ; %s", deletePipeCommand(), createPipeCommand(), streamToURL(streamAddress)));

        return commands;
    }

    static String deletePipeCommand() {
        return String.format("rm -rf %s", PIPE_NAME);
    }

    static String createPipeCommand() {
        return String.format("mkfifo %s", PIPE_NAME);
    }

    static String sendToPipeRawVideoFromFile(String inputFile) {
        return String.format("cat %s > %s", inputFile, PIPE_NAME);
    }

    static String streamToURL(String url) {
        return String.format("ffmpeg -f lavfi -i anullsrc=channel_layout=stereo:sample_rate=44100 -c:a aac -s 256x240 -f rawvideo -i %s" +
                " -deinterlace -c:v libx264 -pix_fmt yuv422p -preset veryfast -r 30 -g $((30 * 2)) -b:v 3000k -acodec libmp3lame -ar 44100" +
                " -threads 6 -q:v 3 -b:a 712000 -bufsize 512k -f flv \"%s\"", PIPE_NAME, url);
    }
}

//        // delete the old pipe
//        commands.add("rm");
//        commands.add("-rf");
//        commands.add(PIPE_NAME);
//        commands.add(";");
//
//        // create new pipe
//        commands.add("mkfifo");
//        commands.add(PIPE_NAME);
//        commands.add(";");

// send data to pipe
//        commands.add("cat");
//        commands.add(inputRawVideoFile);
//        commands.add(">");
//        commands.add(PIPE_NAME);
//        commands.add(";");
//
//        // stream from the pipe
//        commands.add("ffmpeg");
//        commands.add("-f");
//        commands.add("lavfi");
//        commands.add("-i");
//        commands.add("anullsrc=channel_layout=stereo:sample_rate=44100");
//        commands.add("-c:a");
//        commands.add("aac");
//        commands.add("-s");
//        commands.add("256x240");
//        commands.add("-f");
//        commands.add("rawvideo");
//        commands.add("-i");
//        commands.add(PIPE_NAME); // important parameter
//        commands.add("-deinterlace");
//        commands.add("-c:v");
//        commands.add("libx264");
//        commands.add("-pix_fmt");
//        commands.add("yuv422p");
//        commands.add("-preset");
//        commands.add("veryfast");
//        commands.add("-r");
//        commands.add("30");
//        commands.add("-g");
//        commands.add("$((30*2))"); // may be troublesome
//        commands.add("-b:v");
//        commands.add("3000k");
//        commands.add("-acodec");
//        commands.add("libmp3lame");
//        commands.add("-ar");
//        commands.add("44100");
//        commands.add("-threads");
//        commands.add("6");
//        commands.add("-q:v");
//        commands.add("712000");
//        commands.add("-bufsize");
//        commands.add("512k");
//        commands.add("-f");
//        commands.add("flv");
//        commands.add(String.format("\"%s\"", streamAddress));
