package com.group11.menuEntries;

import com.group11.ConsoleHandler;
import com.group11.MenuPages.MenuPageProvider;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Random;

public class GenerateRandomVideoMenuEntry extends BaseMenuEntry {

    public GenerateRandomVideoMenuEntry(String title) {
        super(title);
    }

    @Override
    public void action(String input) {

        String fileName = input.isEmpty() ? "random_rawvideo.raw" : input;

        try {
            Random rand = new Random();
            byte[] allBytes = new byte[100000000]; // size of random raw video
            rand.nextBytes(allBytes);
            Files.write(Paths.get(fileName), allBytes);
            ConsoleHandler.shared().loadMenu(MenuPageProvider.getMainMenuPage());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
