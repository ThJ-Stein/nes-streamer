package com.group11;

import com.group11.MenuPages.ListMenuPage;
import com.group11.MenuPages.MenuPageProvider;

public class Main {

    public static void main(String[] args) {
	    // write your code here

        ListMenuPage mainPage = MenuPageProvider.getMainMenuPage();
        ConsoleHandler.shared().loadMenu(mainPage);
    }
}
