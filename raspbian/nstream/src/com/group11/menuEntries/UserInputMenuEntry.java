package com.group11.menuEntries;

import com.group11.ConsoleHandler;
import com.group11.MenuPages.MenuPageProvider;

import java.io.IOException;
import java.util.ArrayList;

public class UserInputMenuEntry extends BaseMenuEntry {

    public static String INPUT_PLACEHOLDER = "inputPlaceHolder";

    private ArrayList<String> command;
    private Process[] mutuallyExclusiveWithProcesses;
    private Process process;

    private boolean isRunning;
    public boolean getIsRunning() { return isRunning; }

    public UserInputMenuEntry(String title, ArrayList<String> commands) {
        super(title);
        this.command = commands;
    }

    public void requiresStoppingOfProcesses(Process[] processes) {
        mutuallyExclusiveWithProcesses = processes;
    }

    @Override
    public void action(String input) {

        if (isRunning) {
            process.destroy();
        } else {
            if (mutuallyExclusiveWithProcesses != null) {
                for (Process process : mutuallyExclusiveWithProcesses) {
                    process.destroy();
                }
            }

            Runnable asyncTask = () -> {
                try {
                    ArrayList<String> readyCommands = prepareCommands(input);
                    process = new ProcessBuilder(readyCommands).inheritIO().start();

                    isRunning = true;
                    ConsoleHandler.shared().printMessage(String.format("executing:%n%s", readyCommands.toString()));
                    process.waitFor();
                    ConsoleHandler.shared().loadMenu(MenuPageProvider.getMainMenuPage(), 4);
                    isRunning = false;
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            };
            new Thread(asyncTask).start();
        }
    }

    private ArrayList<String> prepareCommands(String input) {

        ArrayList<String> preparedCommands = new ArrayList<>(command);
        int index = preparedCommands.indexOf(UserInputMenuEntry.INPUT_PLACEHOLDER);
        if (index != -1) {
            preparedCommands.set(index, input);
        }
        return preparedCommands;
    }
}
