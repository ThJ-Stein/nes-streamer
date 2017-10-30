package com.group11.MenuPages;

import com.group11.menuEntries.BaseMenuEntry;

import java.util.ArrayList;

public class BaseMenuPage {

    public ArrayList<BaseMenuEntry> entries;

    public BaseMenuPage(ArrayList<BaseMenuEntry> entries) {
        this.entries = entries;
    }

    public String textRepresentation() { return ""; }
    public void handleUserInput(String input) {}
}
