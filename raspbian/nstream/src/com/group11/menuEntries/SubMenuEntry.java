package com.group11.menuEntries;

import com.group11.ConsoleHandler;
import com.group11.MenuPages.BaseMenuPage;

public class SubMenuEntry extends BaseMenuEntry {

    private BaseMenuPage subMenuPage;

    public SubMenuEntry(String title, BaseMenuPage subMenuPage) {
        super(title);
        this.subMenuPage = subMenuPage;
    }

    @Override
    public void action() {
        ConsoleHandler.shared().loadMenu(subMenuPage);
    }
}
