package com.group11.MenuPages;

import com.group11.ConsoleHandler;
import com.group11.menuEntries.BaseMenuEntry;

import java.util.ArrayList;

public class MessageMenuPage extends BaseMenuPage {

    private String message;

    public MessageMenuPage(ArrayList<BaseMenuEntry> entries, String message) {
        super(entries);
        this.message = message;
    }

    @Override
    public String textRepresentation() {
        StringBuilder sb = new StringBuilder();
        sb.append(message).append("\n");

        for (BaseMenuEntry entry : entries) {
            sb.append(entry.getTitle()).append("\n");
        }
        return sb.toString();
    }

    @Override
    public void handleUserInput(String input) {

        if (input.equals("back")) {
            ConsoleHandler.shared().loadMenu(MenuPageProvider.getMainMenuPage());
        } else {
            ConsoleHandler.shared().printMessage("Working...");
            for (BaseMenuEntry entry : entries) {
                entry.action(input);
                entry.action();
            }
        }
    }
}
