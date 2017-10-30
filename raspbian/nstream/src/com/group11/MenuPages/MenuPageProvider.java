package com.group11.MenuPages;

import com.group11.menuEntries.BaseMenuEntry;
import com.group11.menuEntries.GenerateRandomVideoMenuEntry;
import com.group11.menuEntries.MenuEntriesProvider;
import com.group11.menuEntries.UserInputMenuEntry;

import java.util.ArrayList;

public class MenuPageProvider {

    public static ListMenuPage getMainMenuPage() {

        ArrayList<BaseMenuEntry> list = new ArrayList<>();
//        list.add(MenuEntriesProvider.liveStreamAdv7180Entry());
//        list.add(MenuEntriesProvider.liveStreamRawVideoEntry(""));
        list.add(MenuEntriesProvider.openStreamingPipeToYoutube());
        list.add(MenuEntriesProvider.convertToRawVideoEntry());
        list.add(MenuEntriesProvider.generateRandomRawVideoEntry());

        ListMenuPage listPage = new ListMenuPage(list, "Test Tool Main Menu:");
        return listPage;
    }

    public static BaseMenuPage getConvertToVideoRawSubPage() {

        ArrayList<BaseMenuEntry> entries = new ArrayList<>();
        ArrayList<String> commands = new ArrayList<>();

        commands.add("ffmpeg");
        commands.add("-i");
        commands.add(UserInputMenuEntry.INPUT_PLACEHOLDER);
        commands.add("-s");
        commands.add("256x240");
        commands.add("-c:v");
        commands.add("rawvideo");
        commands.add("-pixel_format");
        commands.add("yuv422p");
        commands.add("-an");
        commands.add("-f");
        commands.add("rawvideo");
        commands.add("rawvideo.raw");

        UserInputMenuEntry entry = new UserInputMenuEntry("enter path to video or \"back\" to get to previous menu:", commands);
        entries.add(entry);
        BaseMenuPage convertToRawVideo = new MessageMenuPage(entries, "Convert digital video to raw format");
        return convertToRawVideo;
    }

    public static BaseMenuPage getGenerateRandomRawVideoSubPage() {

        ArrayList<BaseMenuEntry> entries = new ArrayList<>();
        GenerateRandomVideoMenuEntry entry = new GenerateRandomVideoMenuEntry("Enter path to output file or\"back\" to get to previous menu or\nleave empty to use name \"random_rawvideo.raw\":");
        entries.add(entry);
        BaseMenuPage convertToRawVideo = new MessageMenuPage(entries, "Generate random video in raw format.");
        return convertToRawVideo;
    }
}
