package com.group11.menuEntries;

public class BaseMenuEntry {

    private String title;

    public BaseMenuEntry(String title) {
        this.title = title;
    }

    public void action() {}

    public void action(String input) {}

    public String getTitle() {
        return title;
    }
}
