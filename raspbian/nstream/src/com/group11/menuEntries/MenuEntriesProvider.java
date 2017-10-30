package com.group11.menuEntries;

import com.group11.MenuPages.BaseMenuPage;
import com.group11.MenuPages.MenuPageProvider;
import com.group11.StreamParametersHelper;

import java.util.ArrayList;

public class MenuEntriesProvider {

    public static ProcessStatusMenuEntry liveStreamAdv7180Entry() {
        ArrayList<String> commands = new ArrayList<>();
        commands.add("ping");
        commands.add("google.bg");
        commands.add("-c");
        commands.add("4");

        ProcessStatusMenuEntry entry = new ProcessStatusMenuEntry("Live stream to youtube from ADV7180", commands);
        return entry;
    }

    public static ProcessStatusMenuEntry liveStreamRawVideoEntry(String fileName) {
        ArrayList<String> commands = new ArrayList<>();
        commands.add("ping");
        commands.add("google.bg");
        commands.add("-c");
        commands.add("4");

        ProcessStatusMenuEntry entry = new ProcessStatusMenuEntry("Stream from a raw video file", commands);
        return entry;
    }

    public static ProcessStatusMenuEntry openStreamingPipeToYoutube() {
        ArrayList<String> commands = StreamParametersHelper.openStreamingPipeToAddress("rtmp://a.rtmp.youtube.com/live2/z939-kue9-98z8-fu60");
        ProcessStatusMenuEntry entry = new ProcessStatusMenuEntry("Open streaming pipe \"stream_pipe\" to Youtube", commands);
        return entry;
    }

    public static SubMenuEntry convertToRawVideoEntry() {
        BaseMenuPage subPage = MenuPageProvider.getConvertToVideoRawSubPage();
        SubMenuEntry entry = new SubMenuEntry("Convert digital video to raw format", subPage);
        return entry;
    }

    public static SubMenuEntry generateRandomRawVideoEntry() {
        BaseMenuPage subPage = MenuPageProvider.getGenerateRandomRawVideoSubPage();
        SubMenuEntry entry = new SubMenuEntry("Generate random video in raw format", subPage);
        return entry;
    }
}

