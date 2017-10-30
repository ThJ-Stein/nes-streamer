package com.group11.MenuPages;

import com.group11.ConsoleHandler;
import com.group11.menuEntries.BaseMenuEntry;

import java.util.ArrayList;

public class ListMenuPage extends BaseMenuPage {

    public String pageTitle = "";

    public ListMenuPage(ArrayList<BaseMenuEntry> entries, String pageTitle) {
        super(entries);
        this.pageTitle = pageTitle;
    }

    @Override
    public String textRepresentation() {

        BaseMenuEntry[] entries = this.entries.toArray(new BaseMenuEntry[this.entries.size()]);

        StringBuilder sb = new StringBuilder();
        sb.append(pageTitle).append("\n");

        for (int i = 0; i < entries.length; i++) {
            sb.append(String.format("%d. %s\n", i, entries[i].getTitle()));
        }
        sb.append("\nChoose your input or \"q\" to quit: ");

        return sb.toString();
    }

    @Override
    public void handleUserInput(String input) {
        int index = -1;

        if (input.equals("q")) {
            System.exit(0);
        }

        try { index = Integer.parseInt(input); }
        catch (NumberFormatException e) {
            ConsoleHandler.shared().printMessage(String.format("Wrong Input. Type a number from %d to %d", 0, entries.size() - 1));
            ConsoleHandler.shared().reloadMenuAfter(2000);
            return;
        }

        BaseMenuEntry[] entries = this.entries.toArray(new BaseMenuEntry[this.entries.size()]);

        if (index != -1 && index < entries.length) {
            entries[index].action();
            return;
        }

        ConsoleHandler.shared().printMessage(String.format("Wrong Input. Type a number from %d to %d", 0, entries.length - 1));
        ConsoleHandler.shared().reloadMenuAfter(2000);
    }
}
