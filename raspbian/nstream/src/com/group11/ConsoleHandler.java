package com.group11;

import com.group11.MenuPages.BaseMenuPage;
import java.io.Console;

public class ConsoleHandler {

    private static ConsoleHandler instance = null;
    protected ConsoleHandler () { }

    public static ConsoleHandler shared() {
        if(instance == null) {
            instance = new ConsoleHandler();
        }
        return instance;
    }

    private String header =
            "    _   _____________            __                                         \n" +
                    "   / | / / ____/ ___/      _____/ /_________  ____ _____ ___  ___  _____    \n" +
                    "  /  |/ / __/  \\__ \\______/ ___/ __/ ___/ _ \\/ __ `/ __ `__ \\/ _ \\/ ___/    \n" +
                    " / /|  / /___ ___/ /_____(__  / /_/ /  /  __/ /_/ / / / / / /  __/ /        \n" +
                    "/_/ |_/_____//____/     /____/\\__/_/   \\___/\\__,_/_/ /_/ /_/\\___/_/         \n" +
                    "\n" +
                    "NOW YOU ARE PLAYING WITH POWER (of the internet)\n";

    private BaseMenuPage presentedMenu;

    public BaseMenuPage getPresentedMenu() {
        return presentedMenu;
    }

    public void loadMenu(BaseMenuPage menu) {
        clearScreen();
        System.out.println(header);
        System.out.print(menu.textRepresentation());
        presentedMenu = menu;
        processUserInput();
    }

    public void loadMenu(BaseMenuPage menu, long after) {

        new java.util.Timer().schedule(
                new java.util.TimerTask() {
                    @Override
                    public void run() {
                        loadMenu(menu);
                    }
                },
                after
        );
    }

    public void printMessage(String message) {
        System.out.println(message);
    }

    public void reloadMenuAfter(long delay) {


        new java.util.Timer().schedule(
                new java.util.TimerTask() {
                    @Override
                    public void run() {
                        loadMenu(presentedMenu);
                    }
                },
                delay
        );
    }

    public void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    public void processUserInput() {

        Console console = System.console();
        if (console == null) {
            System.err.println("Run this tool inside interactive terminal!");
            System.exit(-1);
        }

        presentedMenu.handleUserInput(console.readLine());
    }
}
